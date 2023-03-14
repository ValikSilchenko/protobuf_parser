cmake_minimum_required(VERSION 3.16)
cmake_policy(SET CMP0016 NEW)

project(protobuf_parser)

set(CMAKE_CXX_STANDARD 14)

include_directories(${CMAKE_SOURCE_DIR}/src)

find_package(Protobuf REQUIRED)

include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS pr.proto)
add_executable(protobuf_parser
        src/main.cpp
        src/protobuf_parser/DelimitedMessagesStreamParser.h
        src/protobuf_parser/helpers.hpp
        ${PROTO_HDRS}
        ${PROTO_SRCS})
target_link_libraries(protobuf_parser protobuf)

enable_testing()
add_subdirectory(test)