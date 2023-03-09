#include "proto/pr.pb.h"
#include "protobuf_parser/DelimitedMessagesStreamParser.h"
#include <iostream>

int main() {
    TestTask::Messages::WrapperMessage message;
    TestTask::Messages::RequestForSlowResponse* request;

    std::string data;
    std::shared_ptr<const Data> pointerToMsg;
    for (int i = 0; i < 5; i++)
    {
        request = new TestTask::Messages::RequestForSlowResponse;
        request->set_time_in_seconds_to_sleep(3 * i);
        message.set_allocated_request_for_slow_response(request);
        pointerToMsg = serializeDelimited<TestTask::Messages::WrapperMessage>(message);
        for (int j = 0; j < pointerToMsg->size(); j++)
        {
            data.push_back((*pointerToMsg)[j]);
        }
    }

    std::list<DelimitedMessagesStreamParser<TestTask::Messages::WrapperMessage>::PointerToConstValue> list;

    DelimitedMessagesStreamParser<TestTask::Messages::WrapperMessage> parser;
    list = parser.parse(data);

    for (const auto& pointer : list)
    {
        std::cout << (*pointer).DebugString() << "\n";
    }

    return 0;
}

