cmake_minimum_required(VERSION 3.6)
project(http_server)
set (CMAKE_CXX_STANDARD 14)

find_library(PTHREAD_LIBRARY pthread)
set(SOURCE_FILES main.cpp httpserver/HttpServer.cpp httpserver/HttpServer.h httpserver/WorkerQueue.cpp httpserver/WorkerQueue.h httpserver/Configuration.h httpserver/Client.cpp httpserver/Client.h httpserver/HttpRequest.cpp httpserver/HttpRequest.h httpserver/HttpRequestHandler.cpp httpserver/HttpRequestHandler.h httpserver/HttpRequestParser.cpp httpserver/HttpRequestParser.h httpserver/HttpResponse.cpp httpserver/HttpResponse.h httpserver/PairNameValue.h httpserver/DateTimeHelper.h httpserver/MimeType.cpp httpserver/MimeType.h )
add_executable(http_server ${SOURCE_FILES})

add_custom_target(http-server COMMAND make -C ${http_server_SOURCE_DIR}
        CLION_EXE_DIR=${http_server_SOURCE_DIR})
