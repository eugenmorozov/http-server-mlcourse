#include "HttpRequestParser.h"

namespace http {
    namespace server {

        HttpRequestParser::HttpRequestParser()
                : state_(method_start) {}

        HttpRequestParser::~HttpRequestParser() {}

        void HttpRequestParser::reset() {
            state_ = method_start;
        }

        HttpRequestParser::ParseResult
        HttpRequestParser::parse(HttpRequest &request, const char *buffer, size_t length) {
            for (int i = 0; i < length; ++i) {
                ParseResult result = consume(request, buffer[i]);
                if (result == good || result == bad) {
                    return result;
                }
            }
            return indeterminate;
        }


        HttpRequestParser::ParseResult HttpRequestParser::consume(HttpRequest &request, char input) {
            switch (state_) {
                case method_start:
                    if (!isChar(input) || isCtl(input) || isTspecial(input)) {
                        return bad;
                    } else {
                        state_ = method;
                        request.pushBackMethod(input);

                        return indeterminate;
                    }
                case method:
                    if (input == ' ') {
                        state_ = uri;

                        return indeterminate;
                    } else if (!isChar(input) || isCtl(input) || isTspecial(input)) {
                        return bad;
                    } else {
                        request.pushBackMethod(input);
                        return indeterminate;
                    }
                case uri:
                    if (input == '?') {
                        state_ = query_param_start;

                        return indeterminate;
                    } else if (input == ' ') {
                        state_ = http_version_h;

                        return indeterminate;
                    } else if (isCtl(input)) {
                        return bad;
                    } else {
                        request.pushBackUri(input);

                        return indeterminate;
                    }
                case query_param_start:
                    state_ = query_param_name;
                    request.getQueryParameters().push_back(PairNameValue());
                    request.getQueryParameters().back().name.push_back(input);

                    return indeterminate;
                case query_param_name:
                    if (input == '=') {
                        state_ = query_param_value;

                        return indeterminate;
                    } else {
                        request.getQueryParameters().back().name.push_back(input);
                        return indeterminate;
                    }
                case query_param_value:
                    if (input == '&') {
                        state_ = query_param_start;

                        return indeterminate;
                    } else if (input == ' ') {
                        state_ = http_version_h;

                        return indeterminate;
                    } else {
                        request.getQueryParameters().back().value.push_back(input);

                        return indeterminate;
                    }
                case http_version_h:
                    if (input == 'H') {
                        state_ = http_version_t_1;

                        return indeterminate;
                    } else {
                        return bad;
                    }
                case http_version_t_1:
                    if (input == 'T') {
                        state_ = http_version_t_2;

                        return indeterminate;
                    } else {
                        return bad;
                    }
                case http_version_t_2:
                    if (input == 'T') {
                        state_ = http_version_p;

                        return indeterminate;
                    } else {
                        return bad;
                    }
                case http_version_p:
                    if (input == 'P') {
                        state_ = http_version_slash;

                        return indeterminate;
                    } else {
                        return bad;
                    }
                case http_version_slash:
                    if (input == '/') {
                        state_ = http_version_major_start;
                        request.setHttpVersionMajor(0);
                        request.setHttpVersionMinor(0);

                        return indeterminate;
                    } else {
                        return bad;
                    }
                case http_version_major_start:
                    if (isDigit(input)) {
                        state_ = http_version_major;
                        request.setHttpVersionMajor(request.getHttpVersionMajor() * 10 + input - '0');

                        return indeterminate;
                    } else {
                        return bad;
                    }
                case http_version_major:
                    if (input == '.') {
                        state_ = http_version_minor_start;

                        return indeterminate;
                    } else {
                        return bad;
                    }
                case http_version_minor_start:
                    if (isDigit(input)) {
                        state_ = http_version_minor;
                        request.setHttpVersionMinor(request.getHttpVersionMinor() * 10 + input - '0');

                        return indeterminate;
                    } else {
                        return bad;
                    }
                case http_version_minor:
                    if (input == '\r') {
                        state_ = expecting_newline_1;

                        return indeterminate;
                    } else {
                        return bad;
                    }
                case expecting_newline_1:
                    if (input == '\n') {
                        state_ = header_line_start;

                        return indeterminate;
                    } else {
                        return bad;
                    }
                case header_line_start:
                    if (input == '\r') {
                        state_ = expecting_newline_3;

                        return indeterminate;
                    } else if (!request.getHeaders().empty() && (input == ' ' || input == '\t')) {
                        state_ = header_lws;

                        return indeterminate;
                    } else if (!isChar(input) || isCtl(input) || isTspecial(input)) {
                        return bad;
                    } else {
                        request.getHeaders().push_back(PairNameValue());
                        request.getHeaders().back().name.push_back(input);
                        state_ = header_name;

                        return indeterminate;
                    }
                case header_lws:
                    if (input == '\r') {
                        state_ = expecting_newline_2;

                        return indeterminate;
                    } else if (input == ' ' || input == '\t') {
                        return indeterminate;
                    } else if (isCtl(input)) {
                        return bad;
                    } else {
                        state_ = header_value;

                        return indeterminate;
                    }
                case header_name:
                    if (input == ':') {
                        state_ = space_before_header_value;

                        return indeterminate;
                    } else if (!isChar(input) || isCtl(input) || isTspecial(input)) {
                        return bad;
                    } else {
                        request.getHeaders().back().name.push_back(input);
                        return indeterminate;
                    }
                case space_before_header_value:
                    if (input == ' ') {
                        state_ = header_value;

                        return indeterminate;
                    } else {
                        return bad;
                    }
                case header_value:
                    if (input == '\r') {
                        state_ = expecting_newline_2;

                        return indeterminate;
                    } else if (isCtl(input)) {
                        return bad;
                    } else {
                        request.getHeaders().back().value.push_back(input);

                        return indeterminate;
                    }
                case expecting_newline_2:
                    if (input == '\n') {
                        state_ = header_line_start;

                        return indeterminate;
                    } else {
                        return bad;
                    }
                case expecting_newline_3:
                    return (input == '\n') ? good : bad;
                default:
                    return bad;
            }
        }

        bool HttpRequestParser::isTspecial(int c) {
            switch (c) {
                case '(':
                case ')':
                case '<':
                case '>':
                case '@':
                case ',':
                case ';':
                case ':':
                case '\\':
                case '"':
                case '/':
                case '[':
                case ']':
                case '?':
                case '=':
                case '{':
                case '}':
                case ' ':
                case '\t':
                    return true;
                default:
                    return false;
            }
        }

        bool HttpRequestParser::isCtl(int c) {
            return (c >= 0 && c <= 31) || (c == 127);
        }

        bool HttpRequestParser::isChar(int c) {
            return c >= 0 && c <= 127;
        }

        bool HttpRequestParser::isDigit(int c) {
            return c >= '0' && c <= '9';
        }
    }
}
