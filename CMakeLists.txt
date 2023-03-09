cmake_minimum_required(VERSION 3.16)
cmake_policy(SET CMP0016 NEW)

project(protobuf_parser)

set(CMAKE_CXX_STANDARD 14)

include_directories(${CMAKE_SOURCE_DIR}/src)

execute_process(COMMAND protoc --cpp_out=../src/proto -I=../ pr.proto)
add_executable(protobuf_parser
        src/main.cpp
        src/protobuf_parser/DelimitedMessagesStreamParser.h
        src/protobuf_parser/helpers.hpp
        ../src/proto/pr.pb.h
        ../src/proto/pr.pb.cc)


target_link_libraries(protobuf_parser protobuf)

enable_testing()
add_subdirectory(test)