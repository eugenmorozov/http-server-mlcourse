#include "httpserver/HttpServer.h"

using namespace http::server;

int main(int argc, char **argv) {

    int port = 0;
    int workers = 0;
    char *rootDir = nullptr;
    int opt = 0;

    while ((opt = getopt(argc, argv, "p:w:r")) != -1) {
        switch (opt) {
            case 'p':
                port = std::atoi(optarg);
                if (port < 1 || port > 65535) {
                    std::cout << "Wrong port number: " << port << std::endl;
                    return 0;
                }
                break;
            case 'w':
                workers = std::atoi(optarg);
                //std::cout<<"its ok with workers";
                if (workers < 1) {
                    std::cout << "Wrong workers: " << workers << std::endl;
                    return 0;
                }
                break;
            case 'r':
                std::cout<<"still doesn't work correctly";
                rootDir = optarg;
                std::cout<<optarg;
                if (rootDir[strlen(rootDir) - 1] == '/') {
                    rootDir[strlen(rootDir) - 1] = '\0';
                }
                break;
            default:
                break;
        }
    }

    std::cout << "server started on used port." << std::endl;
    HttpServer *httpServer = new HttpServer();
    httpServer->startServer(port, workers, rootDir);
    delete httpServer;

    return 0;
}