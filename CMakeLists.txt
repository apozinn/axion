cmake_minimum_required(VERSION 3.10)
project(axion VERSION 0.1.0)

find_package(PkgConfig REQUIRED)
pkg_check_modules(MONGOCXX REQUIRED libmongocxx)
pkg_check_modules(BSONCXX REQUIRED libbsoncxx)

include(CTest)
enable_testing()

add_executable(axion src/main.cpp)
target_include_directories(axion PRIVATE
    "include"
    ${MONGOCXX_INCLUDE_DIRS}
    ${BSONCXX_INCLUDE_DIRS}
)
target_link_libraries(axion PRIVATE
    ${MONGOCXX_LIBRARIES}
    ${BSONCXX_LIBRARIES}
    dpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)