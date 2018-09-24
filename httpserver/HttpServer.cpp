#include <stdexcept>
#include <cstring>
#include <iostream>
#include <event2/event.h>
#include <event2/listener.h>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/thread.h>
#include "HttpServer.h"
#include "Configuration.h"
#include "HttpRequestParser.h"
#include "HttpResponse.h"
#include "HttpRequestHandler.h"


namespace http {
    namespace server {

        char *HttpServer::rootDir_;
        workerqueue_t HttpServer::workerqueue;

        HttpServer::HttpServer() {
            //по сути подключение libevent pthreads
            evthread_use_pthreads();
        }

        HttpServer::~HttpServer() {}

        void HttpServer::startServer(int port, int nWorkers, char *rootDir) {
            struct sockaddr_in listenAddr;
            struct event_base *base;
            struct evconnlistener *listener;

            if (rootDir == nullptr) {
                HttpServer::rootDir_ = Configuration::ROOT_DIR;
            } else {
                HttpServer::rootDir_ = rootDir;
            }
            if (port == 0) {
                port = Configuration::PORT;
            }
            if (nWorkers == 0) {
                nWorkers = Configuration::nWorkers;
            }

            base = event_base_new();
            if (evthread_make_base_notifiable(base) < 0) {
                event_base_free(base);
                throw std::runtime_error("Couldn't make base notifiable!");
            }

            memset(&listenAddr, 0, sizeof(listenAddr));
            listenAddr.sin_family = AF_INET;
            listenAddr.sin_addr.s_addr = htonl(0);
            listenAddr.sin_port = htons(port);

            WorkerQueue::workerqueueInit((workerqueue_t *) &workerqueue, nWorkers);
            listener = evconnlistener_new_bind(base, connectionCallback, (void *) &workerqueue,
                                               LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE | LEV_OPT_THREADSAFE,
                                               SOMAXCONN,
                                               (struct sockaddr *) &listenAddr, sizeof(listenAddr));

            if (listener == NULL) {
                event_base_free(base);
                WorkerQueue::workerqueueShutdown(&workerqueue);
                throw std::runtime_error("Port is busy");
            }

            evconnlistener_set_error_cb(listener, errorCallback);
            event_base_dispatch(base);
            event_base_free(base);
            WorkerQueue::workerqueueShutdown(&workerqueue);

        }

        void
        HttpServer::connectionCallback(evconnlistener *listener,
                int fd,
                sockaddr *address,
                int socklen,
                void *arg) {
            struct event_base *base = evconnlistener_get_base(listener);
            struct bufferevent *bev = bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE | BEV_OPT_THREADSAFE);

            workerqueue_t *workerqueue = (workerqueue_t *) arg;
            Client *client = new Client;
            client->setBuf_ev(bev);
            client->setEvbase(base);
            client->setOutput_buffer(evbuffer_new());

            job_t *job = new job_t;
            job->job_function = serverJobFunction;
            job->user_data = client;
            WorkerQueue::workerqueueAddJob(workerqueue, job);
        }

        void HttpServer::serverJobFunction(job *job) {
            Client *client = (Client *) job->user_data;
            bufferevent_setcb(client->getBuf_ev(), readCallback, writeCallback, eventCallback, NULL);
            bufferevent_enable(client->getBuf_ev(), EV_PERSIST | EV_READ);

            delete client;
            delete job;
        }

        void HttpServer::readCallback(struct bufferevent *bev, void *ctx) {
            size_t length = evbuffer_get_length(bufferevent_get_input(bev));
            char *data = (char *) malloc(sizeof(char) * length);

            evbuffer_remove(bufferevent_get_input(bev), data, length);
            HttpRequestParser requestParser;
            HttpRequest request;
            HttpResponse response;

            requestParser.reset();
            requestParser.parse(request, data, length);
            http::server::HttpRequestHandler requestHandler(rootDir_);
            requestHandler.handleRequest(&request, &response);
            evbuffer_add(bufferevent_get_output(bev), response.toString().c_str(), response.toString().length());

            free(data);
        }


        void HttpServer::eventCallback(struct bufferevent *bev, short events, void *ctx) {
            if (events & BEV_EVENT_ERROR)
                std::cout << "bufferevent error!" <<
                          std::endl;
            if (events & (BEV_EVENT_EOF | BEV_EVENT_ERROR)) {
                bufferevent_free(bev);
            }
        }

        void HttpServer::writeCallback(bufferevent *bev, void *ctx) {
            bufferevent_free(bev);
        }

        void HttpServer::errorCallback(evconnlistener *listener, void *ctx) {
            struct event_base *base = evconnlistener_get_base(listener);
            int err = EVUTIL_SOCKET_ERROR();
            std::cout << "Got an error on the listener. (" << err << ") " << evutil_socket_error_to_string(err);
            event_base_loopexit(base, NULL);
        }
    }
}