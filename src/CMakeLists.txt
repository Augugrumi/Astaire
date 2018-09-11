add_executable(${PROJECT_NAME}
    "main.cpp"
    "utils/log.cpp"
    "connection/connectionmanager.cpp"
    "connection/handler/handler.cpp")

# Set library version
set_target_properties(
    ${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_NAME}_VERSION_STRING
)

set(CMAKE_REQUIRED_LIBRARIES m)
# TODO Just a test, to remove
# CHECK_LIBRARY_EXISTS(pistache tuntap_version "/usr/lib/" HAVE_TUNTAP)

pkg_check_modules(LIBPISTACHE REQUIRED libpistache)


target_include_directories(${PROJECT_NAME} PRIVATE ${LIBPISTACHE_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBPISTACHE_LIBRARIES})

# We're going to log only with boost log
#find_package(Boost 1.58 COMPONENTS log system REQUIRED)
#add_definitions(-DBOOST_LOG_DYN_LINK)
#add_definitions(-DSO)
#target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIRS})
#target_link_libraries(${PROJECT_NAME} PRIVATE ${Boost_LOG_LIBRARIES})
#target_link_libraries(${PROJECT_NAME} PRIVATE "-lboost_system")

# Specify shared library install directory
install(
    TARGETS
    ${PROJECT_NAME}
    DESTINATION
    ${CMAKE_BINARY_DIR}
)
