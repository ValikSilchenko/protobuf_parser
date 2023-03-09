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
    typedef std::list<PointerToConstValue> ParsedMsgsList;

    ParsedMsgsList parse(const std::string& data);

 private:
    std::vector<char> m_buffer;

    void addToBuffer(const std::string& data);
    void removeMessageFromBuffer(size_t msgSize);
};

template<typename MessageType>
typename DelimitedMessagesStreamParser<MessageType>::ParsedMsgsList DelimitedMessagesStreamParser<MessageType>::parse(
        const std::string &data
      ) {
    addToBuffer(data);

    std::list<PointerToConstValue> msgsList;
    std::shared_ptr<MessageType> parsedMsg = std::make_shared<MessageType>(MessageType());
    size_t consumedBytes;

    while (parsedMsg.get() != nullptr)
    {
        parsedMsg = parseDelimited<MessageType>(static_cast<const void*>(&m_buffer), m_buffer.size(), &consumedBytes);
        if (parsedMsg.get())
        {
            msgsList.push_back(parsedMsg);
            removeMessageFromBuffer(consumedBytes);
        }
    }

    return msgsList;
}

template<typename MessageType>
void DelimitedMessagesStreamParser<MessageType>::addToBuffer(const std::string& data) {
    for (char sym : data)
    {
        m_buffer.push_back(sym);
    }
}

template<typename MessageType>
void DelimitedMessagesStreamParser<MessageType>::removeMessageFromBuffer(size_t msgSize) {
    for (int i = 0; i < msgSize; i++)
    {
        m_buffer.erase(m_buffer.cbegin());
    }
}

