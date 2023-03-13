#include <gtest/gtest.h>
#include "protobuf_parser/helpers.hpp"
#include "proto/pr.pb.h"


TEST(HelpersTests, test_serialize_and_parsing_full_msg)
{
    int testNum = 12;
    std::string serializedMessage;

    TestTask::Messages::WrapperMessage message;
    TestTask::Messages::WrapperMessage response;
    std::shared_ptr<TestTask::Messages::WrapperMessage> res;
    std::shared_ptr<const Data> data;
    auto request = new TestTask::Messages::RequestForSlowResponse;

    request->set_time_in_seconds_to_sleep(testNum);
    message.set_allocated_request_for_slow_response(request);

    uint32_t msgSize = PROTOBUF_MESSAGE_BYTE_SIZE(message);
    size_t variantByteSize = google::protobuf::io::CodedOutputStream::VarintSize32(msgSize);
    data = serializeDelimited(message);

    size_t bytesConsumed;
    res = parseDelimited<TestTask::Messages::WrapperMessage>(static_cast<const void*>(&*data), data->size(), &bytesConsumed);
    ASSERT_TRUE(res.get() != nullptr);
    ASSERT_EQ(bytesConsumed, msgSize + variantByteSize);
    response = *res;

    ASSERT_TRUE(response.has_request_for_slow_response());
    ASSERT_EQ(response.request_for_slow_response().time_in_seconds_to_sleep(), testNum);
}

TEST(HelpersTests, test_serialize_and_parsing_msg_by_parts)
{
    int testNum = 12;
    std::string serializedMessage;

    TestTask::Messages::WrapperMessage message;
    TestTask::Messages::WrapperMessage response;
    std::shared_ptr<TestTask::Messages::WrapperMessage> res;
    std::shared_ptr<const Data> data;
    auto request = new TestTask::Messages::RequestForSlowResponse;

    request->set_time_in_seconds_to_sleep(testNum);
    message.set_allocated_request_for_slow_response(request);

    uint32_t msgSize = PROTOBUF_MESSAGE_BYTE_SIZE(message);
    size_t variantByteSize = google::protobuf::io::CodedOutputStream::VarintSize32(msgSize);
    data = serializeDelimited(message);

    size_t bytesConsumed;
    res = parseDelimited<TestTask::Messages::WrapperMessage>(static_cast<const void*>(&*data), data->size() / 2, &bytesConsumed);
    ASSERT_TRUE(res.get() == nullptr);
    ASSERT_EQ(bytesConsumed, 0);

    res = parseDelimited<TestTask::Messages::WrapperMessage>(static_cast<const void*>(&*data), data->size(), &bytesConsumed);
    ASSERT_TRUE(res.get() != nullptr);
    ASSERT_EQ(bytesConsumed, msgSize + variantByteSize);
    response = *res;

    ASSERT_TRUE(response.has_request_for_slow_response());
    ASSERT_EQ(response.request_for_slow_response().time_in_seconds_to_sleep(), testNum);
}


