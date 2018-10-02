//
// Created by zanna on 17/09/18.
//

#include "javahandler.h"
#include "config.h"
#if HAS_JNI

namespace connection {
namespace handler {
JavaHandler::JavaHandler(const std::string &config_file)
    : AbsHandler(config_file) {
    std::string class_file_path = config->getField(utils::jniFields::FILE_PATH);
    std::string class_name = config->getField(utils::jniFields::CLASS_NAME);
    std::string method_name = config->getField(utils::jniFields::METHOD);
    // Pointer to the JVM (Java Virtual Machine)
    JNIEnv * env;
    // Pointer to native interface

    //create arguments and options for JVM
    JavaVMOption *options = new JavaVMOption[1];
    //setting where to find the java class file

    std::string s = "-Djava.class.path=" + class_file_path;
    //path_with_argument;
    options[0].optionString = const_cast<char *>(s.c_str());
    // minimum Java version
    vm_args.version = JNI_VERSION_1_6;
    // number of options -> the path of .class file
    vm_args.nOptions = 1;
    vm_args.options = options;
    // invalid options make the JVM init fail
    vm_args.ignoreUnrecognized = false;

    // load JVM and JNI
    jint rc = JNI_CreateJavaVM(&jvm, reinterpret_cast<void **>(&env), &vm_args);

    if (rc != JNI_OK) {
        LOG(lfatal, "Failure creating the Java VM");
        exit(EXIT_FAILURE);
    }

    // looking for .class file
    jclass cls2 = env->FindClass(const_cast<char *>(class_name.c_str()));
    if (cls2 == nullptr) {
        LOG(lwarn, "Java class not found, this could cause segfault");
    } else { // if class found, continue

        // find method inside the class
        jmethodID mid = env->GetStaticMethodID(
                    cls2,
                    const_cast<char *>(method_name.c_str()), "([B)[B");

        if (mid == nullptr) {
            // exception looking for the method
            LOG(lfatal,
                "Failure finding Java method when handling the request");
            exit(EXIT_FAILURE);
        }
    }
}

std::shared_ptr<uint8_t> JavaHandler::execute_java(const std::string& class_file_path,
                                         const std::string& class_name,
                                         const std::string& method_name,
                                         std::shared_ptr<uint8_t> pkt,
                                         std::size_t pkt_size) {
    // FIXME remove all std::cout

    uint8_t* new_pkt = nullptr;
    JNIEnv* env;
    JavaVMOption *options = new JavaVMOption[1];
    //setting where to find the java class file

    std::string s = "-Djava.class.path=" + class_file_path;
    //path_with_argument;
    options[0].optionString = const_cast<char *>(s.c_str());
    // minimum Java version
    vm_args.version = JNI_VERSION_1_8;
    // number of options -> the path of .class file
    vm_args.nOptions = 1;
    vm_args.options = options;
    // invalid options make the JVM init fail
    vm_args.ignoreUnrecognized = false;

    jvm->AttachCurrentThreadAsDaemon(
                reinterpret_cast<void **>(&env),
                nullptr);

    jbyteArray ret = env->NewByteArray(pkt_size);
    env->SetByteArrayRegion(
                ret,
                0,
                pkt_size,
                reinterpret_cast<const jbyte*>(pkt.get()));
    jclass cls2 = env->FindClass(const_cast<char *>(class_name.c_str()));
    jmethodID mid = env->GetStaticMethodID(
                cls2,
                const_cast<char *>(method_name.c_str()), "([B)[B");
    jbyteArray jpkt = reinterpret_cast<jbyteArray>(
                env->CallStaticObjectMethod(cls2, mid, ret));
    if (env->ExceptionOccurred()) {
        // exception on method execution occurred
        std::string err = "A fatal exception occurred while the JVM was running"
                          "the method. This could be caused by a "
                          "misconfiguration or by using a wrong JVM version."
        #if !DEBUG_BUILD
              " For further information, compile the program in debug mode."
        #endif
                ;

        LOG(lfatal, err);
#if DEBUG_BUILD
        env->ExceptionDescribe();
#endif
        exit(EXIT_FAILURE);
    } else {
        jsize jlen = env->GetArrayLength(jpkt);
        jbyte *jbody = env->GetByteArrayElements(jpkt, nullptr);
        new_pkt = new uint8_t[jlen];
        for (int i = 0; i < jlen; i++) {
            new_pkt[i] = jbody[i];
        }
        delete jbody;
    }

    return std::shared_ptr<uint8_t>(new_pkt);
}

std::shared_ptr<uint8_t> JavaHandler::handler_request(std::shared_ptr<uint8_t> message, std::size_t size) {
    return execute_java(config->getField(utils::jniFields::FILE_PATH),
                       config->getField(utils::jniFields::CLASS_NAME),
                       config->getField(utils::jniFields::METHOD),
                       message, size);
}

JavaHandler::~JavaHandler() {
    jvm->DestroyJavaVM();
}
} // namespace handler
} // namespace connection

#endif
