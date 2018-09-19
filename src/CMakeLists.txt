add_executable(${PROJECT_NAME}
    "main.cpp"
    "utils/log.cpp"
    "utils/threadpool.h"
    "connection/connectionmanager.cpp"
    "connection/handler/binding_handler.cpp"
    "connection/handler/helloworldhandler.cpp"
    "connection/handler/asynctaskexecutor.cpp"
    "connection/tcpconnectionmanager.cpp"
    "connection/udpconnectionmanager.cpp"
    "connection/boostudpconnectionmanager.cpp"
    "connection/rawsocketudpconnectionmanager.cpp"
    "connection/handler/javahandler.cpp"
    "connection/handler/abshandler.cpp"
    "utils/jsonutils.cpp"
    "connection/handler/handlercreator.cpp"
    "connection/handler/printerhandler.cpp")



# Set library version
set_target_properties(
    ${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_NAME}_VERSION_STRING
)

# In debug mode add more compilation flags
if ((CMAKE_BUILD_TYPE MATCHES Debug)  OR (CMAKE_BUILD_TYPE MATCHES DEBUG))
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
endif()

# Include src/ directories
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR}/src)
target_include_directories(${PROJECT_NAME} PRIVATE "${astaire_SOURCE_DIR}/src/utils")
target_include_directories(${PROJECT_NAME} PRIVATE "${astaire_SOURCE_DIR}/src/connection")
target_include_directories(${PROJECT_NAME} PRIVATE "${astaire_SOURCE_DIR}/src/connection/handler")

# Optional compilation flags
option(BOOST_THREAD "For boost thread pool support" ON)
option(JNI "For Java VNF hook implementations" ON)
option(UDP "For UDP support" ON)
option(TCP "For TCP support" ON)

set(HAS_UDP ${UDP})
set(HAS_TCP ${TCP})
# Set buffer limit for UDP Connections
set(BUFFER_SIZE 65536) # Buffer when receiving packets
set(TIMEOUT_WAIT -1) # Polling timeout. Default to no timeout

if (NOT TCP AND NOT UDP)
    message(FATAL_ERROR "You must have at least one between TCP or UDP enabled")
endif()

if (TCP)
# Search for libpistache
pkg_check_modules(LIBPISTACHE REQUIRED libpistache)
target_include_directories(${PROJECT_NAME} PRIVATE ${LIBPISTACHE_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBPISTACHE_LIBRARIES})
endif()

# If enabled, search and use Boost threads
if (BOOST_THREAD)
    message(STATUS "Trying to use Boost Thread (if Boost version >= 1.66...)")
    # We're going to log only with boost log
    find_package(Boost 1.66 COMPONENTS thread)
    if (Boost_FOUND)
        message(STATUS "Boost Thread enabled")
        set(HAS_BOOST_THREAD TRUE)
    else()
        message(STATUS "Boost Thread disabled")
        set(HAS_BOOST_THREAD FALSE)
    endif()
else()
    message(STATUS "Boost Thread disabled, using standard threads")
    set(HAS_BOOST_THREAD FALSE)
endif()

# Search for basic Thread support
find_package(Threads REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE "-pthread")

target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${Boost_LOG_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PRIVATE "-lboost_thread")

# Boost core is needed
find_package(Boost 1.58 COMPONENTS system REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${Boost_LOG_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PRIVATE "-lboost_system")

# Search for JNI integration
if (JNI)
    message(STATUS "JNI enabled, searching for a valid Java installation...")
    find_package(JNI)
    if (JNI_FOUND)
        target_include_directories(${PROJECT_NAME} PRIVATE ${JNI_INCLUDE_DIRS})
        target_link_libraries(${PROJECT_NAME} PRIVATE ${JNI_LIBRARIES})
        set(HAS_JNI TRUE)
    else()
        message(STATUS "No valid Java found, feature disabled")
        set(HAS_JNI FALSE)
    endif()
else()
    message(STATUS "JNI disabled")
    set(HAS_JNI FALSE)
endif()


pkg_check_modules(LIBJSONCPP REQUIRED jsoncpp>=1.7.2)
execute_process (
        COMMAND bash -c "cat /etc/os-release | grep ID= | head -n1 | cut -d\"=\" -f2 | tr -d '[:space:]'"
        OUTPUT_VARIABLE distro_name
)
message(STATUS "Checking for distro in use... ${distro_name}")
if (${distro_name} STREQUAL "ubuntu")
    set(JSONCPP_INCLUDE_PATH_FIX TRUE)
    message(STATUS "Fixing path for jsoncpp on Ubuntu")
else()
    set(JSONCPP_INCLUDE_PATH_FIX FALSE)
endif()
target_link_libraries(${PROJECT_NAME} PRIVATE "-ljsoncpp")

# Create config.h for extra compiler macros
configure_file(
    "${astaire_SOURCE_DIR}/src/config.h.in"
    "${CMAKE_BINARY_DIR}/src/config.h"
    @ONLY
)


# Specify shared library install directory
install(
    TARGETS
    ${PROJECT_NAME}
    DESTINATION
    ${CMAKE_BINARY_DIR}
)

