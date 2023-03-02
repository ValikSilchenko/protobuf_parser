#include "protobuf_parser/helpers.hpp"
#include "util/pr.pb.h"

void testParse()
{
    TestTask::Messages::WrapperMessage message;
    auto request = new TestTask::Messages::RequestForSlowResponse;
    request->set_time_in_seconds_to_sleep(12);
    message.set_allocated_request_for_slow_response(request);

    std::cout << message.DebugString() << "\n\n";

    char data[100];
    uint32_t size = sizeof(message);
    memcpy(data, &size, sizeof(uint32_t));
    std::string serializedMessage;
    message.SerializeToString(&serializedMessage);
    for (int i = 0; i < serializedMessage.size(); i++)
    {
        data[sizeof(uint32_t) + i] = serializedMessage[i];
    }
    std::shared_ptr<TestTask::Messages::WrapperMessage> res = parseDelimited<TestTask::Messages::WrapperMessage>(data, 100);

    TestTask::Messages::WrapperMessage response = *(res);
    std::cout << response.DebugString() << "\n\n";
}




