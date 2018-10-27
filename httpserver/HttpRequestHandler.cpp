#include "HttpRequestHandler.h"
#include "DateTimeHelper.h"
#include "MimeType.h"


namespace http {
    namespace server {

        template <typename T>
        std::string to_string(T value)
        {
            //create an output string stream
            std::ostringstream os ;

            //throw the value into the string stream
            os << value ;

            //convert the string stream into a string and return
            return os.str() ;
        }

        HttpRequestHandler::HttpRequestHandler(std::string docRoot)
                : docRoot_(docRoot)
        { }

        void http::server::HttpRequestHandler::handleRequest(HttpRequest *request,
                                                             HttpResponse *response) {
            if (!isAllowMethod(request->getMethod())) {
                *response = HttpResponse::stockReply(HttpResponse::METHOD_NOT_ALLOWED);

                return;
            }
            std::string resoursePath = request->getUri();

            if (!urlDecode(request->getUri(), &resoursePath)) {
                *response = HttpResponse::stockReply(HttpResponse::BAD_REQUEST);

                return;
            }
//            || resoursePath.find("..") != std::string::npos
            if (resoursePath.empty() || resoursePath[0] != '/' ) {
                *response = HttpResponse::stockReply(HttpResponse::BAD_REQUEST);

                return;
            }
            bool indexFile = false;

            if (resoursePath[resoursePath.size() - 1] == '/') {
                resoursePath += "index.html";
                indexFile = true;
            }
            std::string fullPath = docRoot_ + resoursePath;
            std::cout<<fullPath<<"/n";

            if (access(fullPath.c_str(), 0) == -1) {
                if (indexFile) {
                    *response = HttpResponse::stockReply(HttpResponse::FORBIDDEN);
                } else {
                    *response = HttpResponse::stockReply(HttpResponse::NOT_FOUND);
                }

                return;
            }
            if (access(fullPath.c_str(), 3) == -1) {
                *response = HttpResponse::stockReply(HttpResponse::FORBIDDEN);

                return;
            }

            std::ifstream fin(fullPath.c_str(), std::ios_base::in | std::ios_base::binary);

            if (isMethodWithContent(request->getMethod())) {
                writeContentToRequest(fin, response);
            }
            response->setStatus(HttpResponse::OK);
            response->addHeader(PairNameValue("Date", DateTimeHelper::getDateHeader()));
            response->addHeader(PairNameValue("Server", "libevent|C++"));
            response->addHeader(PairNameValue("Content-Length", to_string(getFileLength(fin))));
            response->addHeader(PairNameValue("Content-Type", mime_types::extensionToType(getFileExtension(resoursePath))));
            response->addHeader(PairNameValue("Connection", "close"));
            response->addHeader(PairNameValue("Allow", "GET | HEAD"));
        }

        bool HttpRequestHandler::isAllowMethod(std::string method) {
            return method == "GET" || method == "HEAD";
        }

        bool HttpRequestHandler::isMethodWithContent(std::string method) {
            return method == "GET" || method == "POST";

        }

        void HttpRequestHandler::writeContentToRequest(std::ifstream &fin, HttpResponse *response) {
            char buff[512];

            while (fin.read(buff, sizeof(buff)).gcount() > 0) {
                response->contentAppend(buff, (unsigned long) fin.gcount());
            }
            fin.clear();
            fin.seekg(0);
        }

        int HttpRequestHandler::getFileLength(std::ifstream &fin) {
            fin.seekg(0, std::ios::end);

            return (int) fin.tellg();
        }

        std::string HttpRequestHandler::getFileExtension(std::string path) {
            size_t lastSlashPos = path.find_last_of("/");
            size_t lastDotPos = path.find_last_of(".");
            std::string extension;

            if (lastDotPos != std::string::npos && lastDotPos > lastSlashPos) {
                extension = path.substr(lastDotPos + 1);
            }

            return extension;
        }

        bool HttpRequestHandler::urlDecode(const std::string &in, std::string *out) {
            out->clear();
            out->reserve(in.size());
            for (size_t i = 0; i < in.size(); ++i) {
                if (in[i] == '%') {
                    if (i + 3 <= in.size()) {
                        int value = 0;
                        std::istringstream is(in.substr(i + 1, 2));

                        if (is >> std::hex >> value) {
                            *out += static_cast<char>(value);
                            i += 2;
                        } else {
                            return false;
                        }
                    }
                } else {
                    *out += in[i];
                }
            }

            return true;
        }
    }
}
