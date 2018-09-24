
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include "WorkerQueue.h"

#define LL_ADD(item, list) { \
	item->prev = NULL; \
	item->next = list; \
	list = item; \
}

#define LL_REMOVE(item, list) { \
	if (item->prev != NULL) item->prev->next = item->next; \
	if (item->next != NULL) item->next->prev = item->prev; \
	if (list == item) list = item->next; \
	item->prev = item->next = NULL; \
}

void* WorkerQueue::workerFunciton(void *ptr) {
    worker_t *worker = (worker_t *)ptr;
    job_t *job;
    while (1) {
        /* ждем уведомлений */
        pthread_mutex_lock(&worker->workerqueue->jobs_mutex);
        while (worker->workerqueue->waiting_jobs == NULL) {
            /* выходим из цикла ожидания */
            if (worker->terminate) break;

            pthread_cond_wait(&worker->workerqueue->jobs_cond, &worker->workerqueue->jobs_mutex);
        }

        /* Выходим из внешнего цикла ожидания */
        if (worker->terminate) break;

        job = worker->workerqueue->waiting_jobs;
        if (job != NULL) {
            LL_REMOVE(job, worker->workerqueue->waiting_jobs);
        }
        pthread_mutex_unlock(&worker->workerqueue->jobs_mutex);

        /* If we didn't get a job, then there's nothing to do at this time. */
        if (job == NULL) continue;

        /* Выполняем */
        job->job_function(job);
    }

    free(worker);
    pthread_exit(NULL);
}

int WorkerQueue::workerqueueInit(workerqueue_t *workerqueue, int numWorkers) {
    int i;
    worker_t *worker;
    pthread_cond_t blank_cond = PTHREAD_COND_INITIALIZER;
    pthread_mutex_t blank_mutex = PTHREAD_MUTEX_INITIALIZER;

    if (numWorkers < 1) numWorkers = 1;
    memset(workerqueue, 0, sizeof(*workerqueue));
    memcpy(&workerqueue->jobs_mutex, &blank_mutex, sizeof(workerqueue->jobs_mutex));
    memcpy(&workerqueue->jobs_cond, &blank_cond, sizeof(workerqueue->jobs_cond));

    for (i = 0; i < numWorkers; i++) {
        if ((worker = (worker_t*)malloc(sizeof(worker_t))) == NULL) {
            perror("Failed to allocate all workers");
            return 1;
        }
        memset(worker, 0, sizeof(*worker));
        worker->workerqueue = workerqueue;
        if (pthread_create(&worker->thread, NULL, workerFunciton, (void *)worker)) {
            perror("Failed to start all worker threads");
            free(worker);
            return 1;
        }
        LL_ADD(worker, worker->workerqueue->workers);
    }

    return 0;
}

void WorkerQueue::workerqueueShutdown(workerqueue_t *workerqueue) {
    worker_t *worker = NULL;

    /* Ставим флаг terminate для всех воркеров. */
    for (worker = workerqueue->workers; worker != NULL; worker = worker->next) {
        worker->terminate = 1;
    }

    /* Удаляем воркеров и задания из очереди
     * Триггерим все воркеры для завершения. */
    pthread_mutex_lock(&workerqueue->jobs_mutex);
    workerqueue->workers = NULL;
    workerqueue->waiting_jobs = NULL;
    pthread_cond_broadcast(&workerqueue->jobs_cond);
    pthread_mutex_unlock(&workerqueue->jobs_mutex);
}

void WorkerQueue::workerqueueAddJob(workerqueue_t *workerqueue, job_t *job) {
    /* Добавляем задание в очеред, и уведомляем воркер. */
    pthread_mutex_lock(&workerqueue->jobs_mutex);
    LL_ADD(job, workerqueue->waiting_jobs);
    pthread_cond_signal(&workerqueue->jobs_cond);
    pthread_mutex_unlock(&workerqueue->jobs_mutex);
}

WorkerQueue::~WorkerQueue() {}
