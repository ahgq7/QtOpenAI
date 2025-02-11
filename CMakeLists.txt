cmake_minimum_required(VERSION 3.16)
project(QtOpenAI LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Network)

add_library(QtOpenAI SHARED
    src/qtopenai.cpp
    src/qtopenai.h
    src/qtopenai_global.h
    src/network/networkmanager.cpp
    src/network/networkmanager.h
    src/models/chatcompletion.cpp
    src/models/chatcompletion.h
    src/models/embedding.cpp
    src/models/embedding.h
    src/models/imagegeneration.cpp
    src/models/imagegeneration.h
)

target_compile_definitions(QtOpenAI PRIVATE QTOPENAI_LIBRARY)

target_include_directories(QtOpenAI PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(QtOpenAI PUBLIC Qt6::Core Qt6::Network)

set_target_properties(QtOpenAI PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

# Örnek proje derlemesi için:
add_subdirectory(examples/chat)
