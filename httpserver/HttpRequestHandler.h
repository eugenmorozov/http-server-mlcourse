#ifndef HTTP_SERVER_MLCOURSE_HTTPREQUESTHANDLER_H
#define HTTP_SERVER_MLCOURSE_HTTPREQUESTHANDLER_H

#include "HttpRequest.h"
#include "HttpResponse.h"


#include <fstream>
#include <unistd.h>

namespace http {
    namespace server {
        class HttpRequestHandler {
        public:
            HttpRequestHandler(std::string docRoot);

            void handleRequest(HttpRequest *request, HttpResponse *response);

            bool isAllowMethod(std::string method);

            bool isMethodWithContent(std::string method);

            int getFileLength(std::ifstream &fin);

            std::string getFileExtension(std::string path);

        private:
            bool urlDecode(const std::string &in, std::string *out);

            void writeContentToRequest(std::ifstream &fin, HttpResponse *response);

            std::string docRoot_;

        };
    }
}

#endif //HTTP_SERVER_MLCOURSE_HTTPREQUESTHANDLER_H
