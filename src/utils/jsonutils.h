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
#if JSONCPP_INCLUDE_PATH_FIX
#include <jsoncpp/json/json.h>
#else
#include <json/json.h>
#endif

namespace utils {
namespace addressFields {
static const std::string ADDRESS="url";
static const std::string PORT="port";
static const std::string CONTENT="content";
} // namespace addressFields
namespace externHookFields {
static const std::string DEFAULT_CONFIG_PATH="./conf.json";;
static const std::string LAUNGUAGE="language";
static const std::string FILE_PATH="file_path";
static const std::string CLASS_NAME="class_name";
static const std::string METHOD="method_name";
} // namespace externHookFields
class JsonUtils {
public:
    class JsonWrapper {
    private:
        Json::Value obj;
    public:
        JsonWrapper(const std::string&);
        JsonWrapper(const std::ifstream&);
        std::string getField(const std::string&) const;
        Json::Value getObj(const std::string&) const;
    };
};

} // namespace utils

#endif //ASTAIRE_JSONUTILS_H
