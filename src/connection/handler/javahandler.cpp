//
// Created by zanna on 17/09/18.
//

#include "javahandler.h"
#if HAS_JNI

namespace connection {
    namespace handler {

        JavaHandler::JavaHandler(const std::string &config_file) : AbsHandler(config_file) {}

        unsigned char* JavaHandler::execute_java(const std::string& class_file_path,
                                                 const std::string& class_name,
                                                 const std::string& method_name,
                                                 unsigned char* pkt,
                                                 std::size_t pkt_size) const {
            uint8_t *new_pkt = nullptr;
            // Pointer to the JVM (Java Virtual Machine)
            JavaVM *jvm;
            // Pointer to native interface
            JNIEnv *env;

            //create arguments and options for JVM
            JavaVMInitArgs vm_args;
            JavaVMOption *options = new JavaVMOption[1];
            //setting where to find the java class file

            std::string s = "-Djava.class.path=" + class_file_path;
            options[0].optionString = const_cast<char *>(s.c_str());//path_with_argument;
            // minimum Java version
            vm_args.version = JNI_VERSION_1_6;
            // number of options -> the path of .class file
            vm_args.nOptions = 1;
            vm_args.options = options;
            // invalid options make the JVM init fail
            vm_args.ignoreUnrecognized = false;

            // load JVM and JNI
            jint rc = JNI_CreateJavaVM(&jvm, (void **) &env, &vm_args);  // YES !!
            delete options;

            if (rc != JNI_OK) {
                // TODO: error creating JVM
                exit(EXIT_FAILURE);
            }

            // looking for .class file
            jclass cls2 = env->FindClass(const_cast<char *>(class_name.c_str()));
            if (cls2 == nullptr) {
                // TODO: error looking for the class
                std::cerr << "ERROR: class not found" << std::endl;
            } else {                                  // if class found, continue
                jbyteArray ret = env->NewByteArray(pkt_size);
                env->SetByteArrayRegion(ret, 0, pkt_size, (const jbyte *) pkt);

                // find method inside the class
                jmethodID mid = env->GetStaticMethodID(cls2,
                                                       const_cast<char *>(method_name.c_str()), "([B)[B");

                if (mid == nullptr) {
                    // exception looking for the method
                    std::cerr << "ERROR: method not found" << std::endl;
                    exit(EXIT_FAILURE);
                } else {
                    jbyteArray jpkt = (jbyteArray)(env->CallStaticObjectMethod(cls2, mid, ret));
                    if (env->ExceptionOccurred()) {
                        // exception on method execution occurred
                        env->ExceptionDescribe();
                        exit(EXIT_FAILURE);
                    } else {
                        jsize jlen = env->GetArrayLength(jpkt);
                        jbyte *jbody = env->GetByteArrayElements(jpkt, 0);
                        int *array = new int[jlen];
                        uint8_t new_pkt[jlen];
                        for (int i = 0; i < jlen; i++) {
                            new_pkt[i] = (uint8_t) jbody[i];
                        }
                        for (int i = 0; i < jlen; i++) {
                            std::cout << new_pkt[i] << std::endl;
                        }
                        delete jbody;
                    }
                }
                jvm->DestroyJavaVM();
                return new_pkt;
            }
        }

        void JavaHandler::handler_request(unsigned char* message, std::size_t size) const {
            message = execute_java(config->getField(utils::JsonUtils::FILE_PATH),
                               config->getField(utils::JsonUtils::CLASS_NAME),
                               config->getField(utils::JsonUtils::METHOD),
                               message, size);
        }
    } // namespace handler
} // namespace connection

#endif
