//
// Created by zanna on 17/09/18.
//

#include "jsonutils.h"

namespace utils {
// FIXME pass this as a program argument
JsonUtils::JsonWrapper::JsonWrapper(const std::string& path) {
    /*std::ifstream ifs(const_cast<char *>(path.c_str()));*/
    Json::Reader reader;
    reader.parse(path, this->obj);
}

JsonUtils::JsonWrapper::JsonWrapper(const std::ifstream& ifs) {
    Json::Reader reader;
    reader.parse(const_cast<std::ifstream&>(ifs), this->obj);
}

std::string JsonUtils::JsonWrapper::getField(const std::string& field_name) const {
    return obj[field_name].asString();
}

Json::Value JsonUtils::JsonWrapper::getObj(const std::string& field_name) const {
    return obj[field_name];
}
} // namespace utils
