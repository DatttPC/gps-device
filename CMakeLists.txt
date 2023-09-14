cmake_minimum_required(VERSION 3.0.0)
project(gps-device VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

add_executable(gps-device main.cpp source.cpp packet.cpp)

# Link OpenSSL libraries
find_package(OpenSSL REQUIRED)
target_link_libraries(gps-device OpenSSL::SSL)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
