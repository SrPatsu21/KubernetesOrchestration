cmake_minimum_required(VERSION 3.5)

project(backend_api)

set(CMAKE_CXX_STANDARD 17)

find_package(Drogon CONFIG REQUIRED)

add_executable(${PROJECT_NAME} src/main.cc)

target_link_libraries(${PROJECT_NAME} PRIVATE Drogon::Drogon)