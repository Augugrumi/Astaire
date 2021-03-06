//
// Created by zanna on 17/09/18.
//

#ifndef ASTAIRE_JAVAHANDLER_H
#define ASTAIRE_JAVAHANDLER_H

#include "config.h"
#if HAS_JNI

#include <jni.h>
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <string>

#include "abshandler.h"
#include "log.h"

namespace connection {
namespace handler {
class JavaHandler : public AbsHandler {
private:
    JavaVMInitArgs vm_args;
    jclass cls2;
    jmethodID mid;
    JavaVM *jvm;
    std::shared_ptr<uint8_t> execute_java(const std::string&,
                                          const std::string&,
                                          const std::string&,
                                          std::shared_ptr<uint8_t>,
                                          std::size_t);

public:
    JavaHandler(const std::string&);
    virtual msgptr handler_request(msgptr, std::size_t) override;
    ~JavaHandler();
};
} // namespace handler
} // namespace connection

#endif

#endif //ASTAIRE_JAVAHANDLER_H
