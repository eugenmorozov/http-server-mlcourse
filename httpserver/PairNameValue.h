#ifndef HTTP_SERVER_MLCOURSE_PAIRNAMEVALUE_H
#define HTTP_SERVER_MLCOURSE_PAIRNAMEVALUE_H

#include <string>

namespace http {
    namespace server {

        struct PairNameValue {
            PairNameValue() {}

            PairNameValue(std::string name, std::string value)
                    : name(name),
                      value(value) {}

            std::string name;
            std::string value;
        };

    }
}


#endif //HTTP_SERVER_MLCOURSE_PAIRNAMEVALUE_H
