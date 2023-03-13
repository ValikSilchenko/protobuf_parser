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
    size_t variantByteSize = google::protobuf::io::CodedOutputStream::VarintSize32(messageSize);

    Data data(variantByteSize + messageSize);

    uint8_t *buf = reinterpret_cast<uint8_t*>(&*(data.begin()));
    google::protobuf::io::CodedOutputStream::WriteVarint32ToArray(messageSize, buf);
    msg.SerializeWithCachedSizesToArray(buf + variantByteSize);

    return std::make_shared<const Data>(data);
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
        auto constDataVec = static_cast<const Data*>(data);
        Data dataVec(constDataVec->begin(),constDataVec->end());
        std::shared_ptr<Message> result = std::make_shared<Message>(Message());
        std::string bytes;
        uint32_t messageSize;

        uint8_t *buf = reinterpret_cast<uint8_t*>(&*(dataVec.begin()));
        google::protobuf::io::CodedInputStream stream(buf, size);

        stream.ReadVarint32(&messageSize);
        size_t variantByteSize = google::protobuf::io::CodedOutputStream::VarintSize32(messageSize);

        if (messageSize == 0 || size < variantByteSize + messageSize)
        {
            if (bytesConsumed)
            {
                *bytesConsumed = 0;
            }
            return nullptr;
        }

        stream.ReadString(&bytes, messageSize);
        result->ParseFromString(bytes);
        if (bytesConsumed)
        {
            *bytesConsumed = variantByteSize + messageSize;
        }
        return result;
    }

    if (bytesConsumed)
    {
        *bytesConsumed = 0;
    }
    return nullptr;
}
