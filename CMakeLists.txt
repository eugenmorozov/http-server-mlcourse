cmake_minimum_required(VERSION 3.6)
project(http_server)
set (CMAKE_CXX_STANDARD 14)

add_custom_target(http-server COMMAND make -C ${http_server_SOURCE_DIR}
        CLION_EXE_DIR=${http_server_SOURCE_DIR})
