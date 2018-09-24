
#ifndef HTTP_SERVER_MLCOURSE_CONFIGURATION_H
#define HTTP_SERVER_MLCOURSE_CONFIGURATION_H

//файл дефолтной конфигурации сервера
class Configuration {
public:
    static const int PORT = 8000;
    static const int nWorkers = 1;
    static char *ROOT_DIR;
};

char *Configuration::ROOT_DIR = (char *) "/var/www/html";

#endif //HTTP_SERVER_MLCOURSE_CONFIGURATION_H
