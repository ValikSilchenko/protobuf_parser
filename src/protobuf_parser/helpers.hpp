#pragma once

#include <vector>
#include <memory>
#include <google/protobuf/io/coded_stream.h>

#if GOOGLE_PROTOBUF_VERSION >= 3012004
#define PROTOBUF_MESSAGE_BYTE_SIZE(message) ((message).ByteSizeLong())
#else
#define PROTOBUF_MESSAGE_BYTE_SIZE(message) ((message).ByteSize())
#endif


typedef std::vector<char> Data;
typedef std::shared_ptr<const Data> PointerToConstData;

template <typename Message>
PointerToConstData serializeDelimited(const Message& msg)
{
    uint32_t messageSize = PROTOBUF_MESSAGE_BYTE_SIZE(msg);
    char msgInBytes[sizeof(uint32_t) + messageSize];
    memcpy(msgInBytes, &messageSize, sizeof(uint32_t));
    memcpy(msgInBytes + sizeof(uint32_t), msg.SerializeAsString().c_str(), messageSize);

    Data data(sizeof(uint32_t) + messageSize);
    for (int i = 0; i < sizeof(uint32_t) + messageSize; i++)
    {
        data[i] = msgInBytes[i];
    }

    PointerToConstData result = std::make_shared<const Data>(data);

    return result;
}

/*!
 * \brief Расшифровывает сообщение, предваренное длиной из массива байтов.
 *
 * \tparam Message Тип сообщения, для работы с которым предназначена данная
 * функция.
 *
 * \param data Указатель на буфер данных.
 * \param size Размер буфера данных.
 * \param bytesConsumed Количество байт, которое потребовалось для расшифровки
 * сообщения в случае успеха.
 *
 * \return Умный указатель на сообщение. Если удалось расшифровать сообщение, то
 * он не пустой.
 */
template <typename Message>
std::shared_ptr<Message> parseDelimited(const void* data, size_t size,
                                        size_t* bytesConsumed = nullptr)
{
    if (data && size > sizeof(uint32_t))
    {
        auto dataVec = static_cast<const Data*>(data);
        char* message;
        char buffer[sizeof(uint32_t)];

        for (int i = 0; i < sizeof(uint32_t); i++)
        {
            buffer[i] = dataVec->at(i);
        }

        uint32_t messageSize;
        memcpy(&messageSize, buffer, sizeof(uint32_t));

        if (messageSize == 0 || size < sizeof(uint32_t) + messageSize)
        {
            if (bytesConsumed)
            {
                *bytesConsumed = 0;
            }
            return nullptr;
        }

        message = new char[messageSize];
        for (int byte = 0; byte < messageSize; byte++)
        {
            message[byte] = dataVec->at(sizeof(uint32_t) + byte);
        }

        std::shared_ptr<Message> result = std::make_shared<Message>(Message());
        result->ParseFromString(message);

        if (bytesConsumed)
        {
            *bytesConsumed = sizeof(uint32_t) + messageSize;
        }
        delete [] message;
        return result;
    }

    if (bytesConsumed)
    {
        *bytesConsumed = 0;
    }
    return nullptr;
}
