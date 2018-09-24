#ifndef HTTP_SERVER_MLCOURSE_MIMETYPE_H
#define HTTP_SERVER_MLCOURSE_MIMETYPE_H

#include <iostream>
#include <cstring>

namespace http {
    namespace server {
        namespace mime_types {
            std::string extensionToType(const std::string &extension);
        }

    }
}

#endif //HTTP_SERVER_MLCOURSE_MIMETYPE_H
