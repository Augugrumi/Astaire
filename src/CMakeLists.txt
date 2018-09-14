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
    "connection/rawsocketudpconnectionmanager.cpp")

# Set library version
set_target_properties(
    ${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_NAME}_VERSION_STRING
)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR}/src)
target_include_directories(${PROJECT_NAME} PRIVATE "${astaire_SOURCE_DIR}/src/utils")
target_include_directories(${PROJECT_NAME} PRIVATE "${astaire_SOURCE_DIR}/src/connection")
target_include_directories(${PROJECT_NAME} PRIVATE "${astaire_SOURCE_DIR}/src/connection/handler")
option(ENABLE_BOOST_THREAD "Enable boost thread support" ON)

pkg_check_modules(LIBPISTACHE REQUIRED libpistache)
target_include_directories(${PROJECT_NAME} PRIVATE ${LIBPISTACHE_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBPISTACHE_LIBRARIES})


if (ENABLE_BOOST_THREAD)
    message("Trying to use Boost Thread (if Boost version >= 1.66...)")
    # We're going to log only with boost log
    find_package(Boost 1.66 COMPONENTS thread)
    if (Boost_FOUND)
        message("Boost Thread enabled")
        set(HAS_BOOST_THREAD TRUE)
    else()
        message("Boost Thread disabled")
        set(HAS_BOOST_THREAD FALSE)
    endif()
else()
    message("Boost Thread disabled, using standard threads")
    set(HAS_BOOST_THREAD FALSE)

    find_package(Threads REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE "-pthread")

endif()

target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${Boost_LOG_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PRIVATE "-lboost_thread")

find_package(Boost 1.58 COMPONENTS system REQUIRED)
add_definitions(-DSO)
target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${Boost_LOG_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PRIVATE "-lboost_system")

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

