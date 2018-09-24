#include "MimeType.h"

namespace http {
    namespace server {
        namespace mime_types {
            struct MimeTypeMapping {
                std::string extension;
                std::string mimeType;
            } mappings[] = {
                    {"gif",  "image/gif"},
                    {"htm",  "text/html"},
                    {"html", "text/html"},
                    {"jpg",  "image/jpeg"},
                    {"jpeg", "image/jpeg"},
                    {"css",  "text/css"},
                    {"js",   "text/javascript"},
                    {"swf",  "application/x-shockwave-flash"},
                    {"png",  "image/png"}
            };

            std::string extensionToType(const std::string &extension) {
                for (MimeTypeMapping m: mappings) {
                    if (m.extension == extension) {
                        return m.mimeType;
                    }
                }

                return "text/plain";
            }
        }
    }
}