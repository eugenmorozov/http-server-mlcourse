cmake_minimum_required(VERSION 3.6)
project(http_server)
find_library(PTHREAD_LIBRARY pthread)

set(SOURCE_FILES main.cpp httpserver/HttpServer.cpp httpserver/HttpServer.h httpserver/WorkerQueue.cpp httpserver/WorkerQueue.h httpserver/Configuration.h httpserver/Client.cpp httpserver/Client.h httpserver/HttpRequest.cpp httpserver/HttpRequest.h httpserver/HttpRequestHandler.cpp httpserver/HttpRequestHandler.h httpserver/HttpRequestParser.cpp httpserver/HttpRequestParser.h httpserver/HttpResponse.cpp httpserver/HttpResponse.h httpserver/PairNameValue.h httpserver/DateTimeHelper.h httpserver/MimeType.cpp httpserver/MimeType.h )
add_executable(http_server ${SOURCE_FILES})