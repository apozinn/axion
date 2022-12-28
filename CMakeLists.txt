cmake_minimum_required(VERSION 3.0.0)
project(axion VERSION 0.1.0)

include(CTest)
enable_testing()

add_executable(axion src/main.cpp)

target_link_libraries(axion PRIVATE dpp)
target_include_directories(axion PRIVATE "include")

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)