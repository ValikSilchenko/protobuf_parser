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

template <typename Message> PointerToConstData serializeDelimited(const Message& msg) { return nullptr; }

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
        const char* charData = static_cast<const char*>(data);
        char* message;
        uint32_t messageSize;
        memcpy(&messageSize, data, sizeof(uint32_t));

        if (size < sizeof(uint32_t) + messageSize)
        {
            return nullptr;
        }

        message = new char[messageSize];
        for (int byte = 0; byte < messageSize; byte++)
        {
            message[byte] = charData[sizeof(uint32_t) + byte];
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
    return nullptr;
}
