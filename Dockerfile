FROM ubuntu:16.04
MAINTAINER ppiper
RUN apt-get update && apt-get install -y --no-install-recommends apt-utils
RUN apt-get -y install g++
RUN apt-get -y install libevent-dev
#RUN apt-get -y install git

#RUN git clone https://github.com/init/http-test-suite.git

RUN mkdir -p /var/www/html
ADD ./ ./http-test-suite
WORKDIR ./http-test-suite

ADD ./ ./

RUN g++ -std=c++11  -o server  main.cpp ./httpserver/Client.cpp ./httpserver/HttpServer.cpp ./httpserver/WorkerQueue.cpp ./httpserver/MimeType.cpp ./httpserver/HttpResponse.cpp ./httpserver/HttpRequest.cpp ./httpserver/HttpRequestParser.cpp ./httpserver/HttpRequestHandler.cpp  -levent -levent_pthreads -lpthread

EXPOSE 80

CMD ./server -p80 -w20
