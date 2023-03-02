#pragma once

#include <list>
#include <string>
#include <memory>
#include <vector>

#include "helpers.hpp"

template <typename MessageType>
class DelimitedMessagesStreamParser
{
 public:
    DelimitedMessagesStreamParser() = default;

    typedef std::shared_ptr<const MessageType> PointerToConstValue;

    std::list<PointerToConstValue> parse(const std::string& data);

 private:
    std::vector<char> m_buffer;
};

template<typename MessageType>
std::list<typename DelimitedMessagesStreamParser<MessageType>::PointerToConstValue> DelimitedMessagesStreamParser<MessageType>::parse(const std::string &data)
{
    std::list<PointerToConstValue> result;

    int startIndex = 0;
    size_t consumedBytes = -1;
    while (consumedBytes != 0)
    {
        result.push_back(parseDelimited<MessageType>(data.substr(startIndex).c_str(), data.substr(startIndex).size(), &consumedBytes));
        startIndex += static_cast<int>(consumedBytes);
    }

    return result;
}

