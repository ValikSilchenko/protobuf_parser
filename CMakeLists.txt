cmake_minimum_required(VERSION 3.21)
cmake_policy(SET CMP0016 NEW)

project(protobuf_parser)

set(CMAKE_CXX_STANDARD 14)

include_directories(${CMAKE_SOURCE_DIR}/src)

add_executable(protobuf_parser src/main.cpp src/protobuf_parser/DelimitedMessagesStreamParser.hpp src/protobuf_parser/DelimitedMessagesStreamParser.cpp src/protobuf_parser/helpers.hpp src/util/pr.pb.h src/util/pr.pb.cc)
target_link_libraries(protobuf_parser protobuf)

enable_testing()
add_subdirectory(test)
