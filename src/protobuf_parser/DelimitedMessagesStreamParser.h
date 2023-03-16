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
    size_t consumedBytes = -1;

    while (consumedBytes != 0)
    {
        msgsList.push_back(parseDelimited<MessageType>(
                static_cast<const void*>(&m_buffer),
                m_buffer.size(),
                &consumedBytes)
                );
        if (msgsList.back() != nullptr)
        {
            removeMessageFromBuffer(consumedBytes);
        } else {
            msgsList.pop_back();
        }
    }

    return msgsList;
}

template<typename MessageType>
void DelimitedMessagesStreamParser<MessageType>::addToBuffer(const std::string& data) {
    m_buffer.insert(m_buffer.cend(), data.cbegin(), data.cend());
}

template<typename MessageType>
void DelimitedMessagesStreamParser<MessageType>::removeMessageFromBuffer(size_t msgSize) {
    m_buffer.erase(m_buffer.cbegin(), m_buffer.cbegin() + msgSize);
}

