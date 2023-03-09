#include <gtest/gtest.h>
#include "protobuf_parser/DelimitedMessagesStreamParser.h"
#include "proto/pr.pb.h"


TEST(ParserTests, test_parsing_with_full_msg)
{
    int testNum = 3;

    TestTask::Messages::WrapperMessage message;
    TestTask::Messages::RequestForSlowResponse* request;

    std::string data;
    std::shared_ptr<const Data> pointerToMsg;
    DelimitedMessagesStreamParser<TestTask::Messages::WrapperMessage>::ParsedMsgsList list;
    DelimitedMessagesStreamParser<TestTask::Messages::WrapperMessage> parser;

    for (int i = 0; i < 5; i++)
    {
        request = new TestTask::Messages::RequestForSlowResponse;
        request->set_time_in_seconds_to_sleep(testNum * i);
        message.set_allocated_request_for_slow_response(request);

        pointerToMsg = serializeDelimited<TestTask::Messages::WrapperMessage>(message);
        for (int j = 0; j < pointerToMsg->size(); j++)
        {
            data.push_back((*pointerToMsg)[j]);
        }
    }

    list = parser.parse(data);
    int numToCompare = 0;
    for (const auto& msg : list)
    {
        ASSERT_TRUE(msg->has_request_for_slow_response());
        ASSERT_EQ(msg->request_for_slow_response().time_in_seconds_to_sleep(), numToCompare);
        numToCompare += testNum;
    }
}

TEST(ParserTests, test_parsing_with_part_of_msg)
{
    int testNum = 3;

    TestTask::Messages::WrapperMessage message;
    TestTask::Messages::RequestForSlowResponse* request;

    std::string data;
    std::shared_ptr<const Data> pointerToMsg;
    DelimitedMessagesStreamParser<TestTask::Messages::WrapperMessage>::ParsedMsgsList list;
    DelimitedMessagesStreamParser<TestTask::Messages::WrapperMessage> parser;

    request = new TestTask::Messages::RequestForSlowResponse;
    request->set_time_in_seconds_to_sleep(testNum);
    message.set_allocated_request_for_slow_response(request);

    pointerToMsg = serializeDelimited<TestTask::Messages::WrapperMessage>(message);
    for (int j = 0; j < pointerToMsg->size() / 2; j++)
    {
        data.push_back((*pointerToMsg)[j]);
    }

    list = parser.parse(data);
    ASSERT_TRUE(list.empty());

    data.clear();
    for (int j = pointerToMsg->size() / 2; j < pointerToMsg->size(); j++)
    {
        data.push_back((*pointerToMsg)[j]);
    }

    list = parser.parse(data);

    ASSERT_TRUE(list.front()->has_request_for_slow_response());
    ASSERT_EQ(list.front()->request_for_slow_response().time_in_seconds_to_sleep(), testNum);

}

