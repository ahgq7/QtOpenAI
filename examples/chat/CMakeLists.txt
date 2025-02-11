cmake_minimum_required(VERSION 3.16)
project(ChatExample)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core)

add_executable(ChatExample main.cpp)

target_link_libraries(ChatExample PRIVATE Qt6::Core QtOpenAI)