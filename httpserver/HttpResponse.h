#ifndef HTTP_SERVER_MLCOURSE_HTTPRESPONCE_H
#define HTTP_SERVER_MLCOURSE_HTTPRESPONCE_H

#include <vector>
#include "PairNameValue.h"

namespace http {
    namespace server {
        class HttpResponse {
        public:
            HttpResponse();

            ~HttpResponse() {}

            enum StatusType {
                OK = 200,
                CREATED = 201,
                ACCEPTED = 202,
                NO_CONTENT = 204,
                MULTIPLE_CHOICES = 300,
                MOVED_PERMANENTLY = 301,
                MOVED_TEMPORARILY = 302,
                NOT_MODIFIED = 304,
                BAD_REQUEST = 400,
                UNAUTHORIZED = 401,
                FORBIDDEN = 403,
                NOT_FOUND = 404,
                METHOD_NOT_ALLOWED = 405,
                INTERNAL_SERVER_ERROR = 500,
                NOT_IMPLEMENTED = 501,
                BAD_GATEWAY = 502,
                SERVICE_UNAVAILABLE = 503
            };

            static HttpResponse stockReply(StatusType status);

            std::vector<PairNameValue> getHeaders() const;

            std::string getContent() const;

            StatusType getStatus() const;

            void addHeader(PairNameValue header);

            void setContent(std::string content);

            void contentAppend(char buff[], unsigned long size);

            void setStatus(StatusType status);

            std::string toString();

        private:
            std::vector<PairNameValue> headers_;
            std::string content_;
            StatusType status_;
        };

    }
}

#endif //HTTP_SERVER_MLCOURSE_HTTPRESPONCE_H
