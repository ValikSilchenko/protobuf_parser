cmake_minimum_required(VERSION 3.16)

project(test_parser)

set(CMAKE_CXX_STANDARD 14)

include(CTest)

add_executable(test_parser
        run_tests.cpp
        Test_DelimitedMessagesStreamParser.cpp
        Test_Helpers.cpp
        ../src/proto/pr.pb.h
        ../src/proto/pr.pb.cc
        ../src/protobuf_parser/DelimitedMessagesStreamParser.h)
target_link_libraries(test_parser protobuf gtest gmock pthread)
