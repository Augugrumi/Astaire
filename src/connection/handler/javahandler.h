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

namespace connection{

    namespace handler{

        class JavaHandler : public AbsHandler {
        private:
            unsigned char* execute_java(const std::string& class_file_path,
                    const std::string& class_name,
                    const std::string& method_name,
                    unsigned char* pkt,
                    std::size_t pkt_size) const;
        public:
            JavaHandler(const std::string& config_path);
            void handler_request(unsigned char* message, std::size_t size) const;
        };
    } // namespace handler
} // namespace connection

#endif

#endif //ASTAIRE_JAVAHANDLER_H
