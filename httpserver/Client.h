#include <event2/event.h>
#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <stdlib.h>

#ifndef HTTP_SERVER_MLCOURSE_CLIENT_H
#define HTTP_SERVER_MLCOURSE_CLIENT_H

class Client {
public:
    Client(){}
    ~Client(){
        if(output_buffer != NULL){
            evbuffer_free(output_buffer);
        }
    }

    int getFd() const {
        return fd;
    }
    void setFd(int fd) {
        Client::fd = fd;
    }
    void setEvbase(event_base *evbase) {
        Client::evbase = evbase;
    }
    void setBuf_ev(bufferevent *buf_ev) {
        Client::buf_ev = buf_ev;
    }
    void setOutput_buffer(evbuffer *output_buffer) {
        Client::output_buffer = output_buffer;
    }
    event_base *getEvbase() const {
        return evbase;
    }
    bufferevent *getBuf_ev() const {
        return buf_ev;
    }
    evbuffer *getOutput_buffer() const {
        return output_buffer;
    }
    int getFd() { return fd; }

private:
    int fd;
    struct event_base *evbase;
    struct bufferevent *buf_ev;
    struct evbuffer *output_buffer;
};


#endif //HTTP_SERVER_MLCOURSE_CLIENT_H
