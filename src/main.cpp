#include "util/pr.pb.h"
#include "protobuf_parser/helpers.hpp"
#include <iostream>

int main() {
    TestTask::Messages::WrapperMessage message;
    auto request = new TestTask::Messages::RequestForSlowResponse;
    request->set_time_in_seconds_to_sleep(12);
    message.set_allocated_request_for_slow_response(request);

    std::cout << message.DebugString() << "\n\n";

    std::shared_ptr<const Data> data = serializeDelimited<TestTask::Messages::WrapperMessage>(message);
    std::shared_ptr<TestTask::Messages::WrapperMessage> res = parseDelimited<TestTask::Messages::WrapperMessage>(data.get(), data->size());

    if (res)
    {
        TestTask::Messages::WrapperMessage response = *(res);
        std::cout << response.DebugString() << "\n\n";
    }

    return 0;
}

