//
// Created by zanna on 17/09/18.
//

#include "javahandler.h"
#if HAS_JNI

namespace connection {
    namespace handler {

        JavaHandler::JavaHandler(const std::string &config_file) : AbsHandler(config_file) {
            std::string class_file_path = config->getField(utils::JsonUtils::FILE_PATH);
            std::string class_name = config->getField(utils::JsonUtils::CLASS_NAME);
            std::string method_name = config->getField(utils::JsonUtils::METHOD);
            // Pointer to the JVM (Java Virtual Machine)
            JNIEnv * env;
            // Pointer to native interface

            //create arguments and options for JVM
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


                // find method inside the class
                jmethodID mid = env->GetStaticMethodID(cls2,
                                                       const_cast<char *>(method_name.c_str()), "([B)[B");

                if (mid == nullptr) {
                    // exception looking for the method
                    std::cerr << "ERROR: method not found" << std::endl;
                    exit(EXIT_FAILURE);
                }
            }
        }

        unsigned char* JavaHandler::execute_java(const std::string& class_file_path,
                                                 const std::string& class_name,
                                                 const std::string& method_name,
                                                 unsigned char* pkt,
                                                 std::size_t pkt_size) {
            uint8_t *new_pkt;
            JNIEnv* env;
            std::cout << "1" << std::endl;
            if (jvm == nullptr)
                std::cout << "jvm null" << std::endl;
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

            jvm->AttachCurrentThreadAsDaemon((void**)&env, NULL);

            std::cout << "2" << std::endl;

            jbyteArray ret = env->NewByteArray(pkt_size);
            std::cout << "3" << std::endl;
            env->SetByteArrayRegion(ret, 0, pkt_size, (const jbyte *) pkt);
            std::cout << "4" << std::endl;
            jclass cls2 = env->FindClass(const_cast<char *>(class_name.c_str()));
            std::cout << "5" << std::endl;
            jmethodID mid = env->GetStaticMethodID(cls2,
                                                   const_cast<char *>(method_name.c_str()), "([B)[B");
            std::cout << "6" << std::endl;
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

            return new_pkt;

        }

        void JavaHandler::handler_request(unsigned char* message, std::size_t size) {
            message = execute_java(config->getField(utils::JsonUtils::FILE_PATH),
                               config->getField(utils::JsonUtils::CLASS_NAME),
                               config->getField(utils::JsonUtils::METHOD),
                               message, size);
        }

        JavaHandler::~JavaHandler() {
            jvm->DestroyJavaVM();
        }
    } // namespace handler
} // namespace connection

#endif
