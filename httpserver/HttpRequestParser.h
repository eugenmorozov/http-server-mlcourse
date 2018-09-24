#ifndef HTTP_SERVER_MLCOURSE_HTTPREQUESTPARSER_H
#define HTTP_SERVER_MLCOURSE_HTTPREQUESTPARSER_H

#include <cstddef>
#include "HttpRequest.h"
namespace http {
    namespace server {
        class HttpRequestParser {
        public:
            HttpRequestParser();

            ~HttpRequestParser();

            void reset();

            enum ParseResult {
                good, bad, indeterminate
            };

            ParseResult parse(HttpRequest &request, const char *buffer, size_t length);

        private:
            ParseResult consume(HttpRequest &request, char input);

            static bool isChar(int c);

            static bool isCtl(int c);

            static bool isTspecial(int c);

            static bool isDigit(int c);

            enum State {
                method_start,
                method,
                uri,
                query_param_start,
                query_param_name,
                query_param_value,
                http_version_h,
                http_version_t_1,
                http_version_t_2,
                http_version_p,
                http_version_slash,
                http_version_major_start,
                http_version_major,
                http_version_minor_start,
                http_version_minor,
                expecting_newline_1,
                header_line_start,
                header_lws,
                header_name,
                space_before_header_value,
                header_value,
                expecting_newline_2,
                expecting_newline_3
            } state_;
        };
    }
}


#endif //HTTP_SERVER_MLCOURSE_HTTPREQUESTPARSER_H
