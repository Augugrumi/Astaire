//
// Created by zanna on 17/09/18.
//

#ifndef ASTAIRE_JSONUTILS_H
#define ASTAIRE_JSONUTILS_H

#include <string>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include "config.h"
//#if JSONCPP_INCLUDE_PATH_FIX
#include <jsoncpp/json/json.h>
/*#else
#include <json/json.h>
#endif*/

namespace utils{

    class JsonUtils {
    public:
        static const std::string DEFAULT_CONFIG_PATH;
        static const std::string LAUNGUAGE;
        static const std::string FILE_PATH;
        static const std::string CLASS_NAME;
        static const std::string METHOD;

        class JsonWrapper {
        private:
            Json::Value obj;
        public:
            JsonWrapper(const std::string& path);
            std::string getField(const std::string& field_name) const;
        };
    };

} // namespace utils

#endif //ASTAIRE_JSONUTILS_H
