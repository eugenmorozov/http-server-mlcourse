#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <pthread.h>


#ifndef HTTP_SERVER_MLCOURSE_WORKERQUEUE_H
#define HTTP_SERVER_MLCOURSE_WORKERQUEUE_H

typedef struct worker{
    pthread_t thread;
    int terminate;
    struct workerqueue *workerqueue;
    struct worker *next;
    struct worker *prev;
} worker_t;

typedef struct job{
    void (*job_function)(job *job);
    void *user_data;
    struct job *prev;
    struct job *next;
} job_t;

typedef struct workerqueue{
    struct worker *workers;
    struct job *waiting_jobs;
    pthread_mutex_t jobs_mutex;
    pthread_cond_t jobs_cond;
} workerqueue_t;


class WorkerQueue {
public:
    ~WorkerQueue();
    static int workerqueueInit(workerqueue_t *workerqueue, int numWorkers);
    static void *workerFunciton(void *);
    static void workerqueueAddJob(workerqueue_t *workerqueue, job_t *job);
    static void workerqueueShutdown(workerqueue_t *workerqueue);
private:
    WorkerQueue() {};
};


#endif //HTTP_SERVER_MLCOURSE_WORKERQUEUE_H
