#include "HttpRequest.h"


namespace http {
    namespace server {
        std::string HttpRequest::getMethod() {
            return method_;
        }

        std::string HttpRequest::getUri() {
            return uri_;
        }

        std::vector<PairNameValue> &HttpRequest::getHeaders() {
            return headers_;
        }

        void HttpRequest::addHeader(PairNameValue header) {
            headers_.push_back(header);
        }

        int HttpRequest::getHttpVersionMajor() const {
            return httpVersionMajor_;
        }

        int HttpRequest::getHttpVersionMinor() const {
            return httpVersionMinor_;
        }

        void HttpRequest::setHttpVersionMajor(int httpVersionMajor) {
            httpVersionMajor_ = httpVersionMajor;
        }

        void HttpRequest::setHttpVersionMinor(int httpVersionMinor) {
            httpVersionMinor_ = httpVersionMinor;
        }

        void HttpRequest::pushBackMethod(char ch) {
            method_.push_back(ch);
        }

        std::vector<PairNameValue> &HttpRequest::getQueryParameters() {
            return queryParameters_;
        }

        void HttpRequest::addQueryParameter(PairNameValue parameter) {
            queryParameters_.push_back(parameter);
        }

        void HttpRequest::pushBackUri(char ch) {
            uri_.push_back(ch);
        }
    }
}
