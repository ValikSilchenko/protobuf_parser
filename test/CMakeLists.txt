cmake_minimum_required(VERSION 3.16)

project(test_parser)

set(CMAKE_CXX_STANDARD 14)

include(CTest)
add_test(test ${EXECUTABLE_OUTPUT_PATH}/test_parser)

include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

add_executable(test_parser
        run_tests.cpp
        Test_DelimitedMessagesStreamParser.cpp
        Test_Helpers.cpp
        ${PROTO_SRCS}
        ${PROTO_HDRS}
        ${CMAKE_SOURCE_DIR}/src/protobuf_parser/DelimitedMessagesStreamParser.h)
target_link_libraries(test_parser protobuf gtest gmock pthread)
