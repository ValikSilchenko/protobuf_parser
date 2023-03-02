#pragma once

#include <list>
#include <string>
#include <memory>
#include <vector>

template <typename MessageType>
class DelimitedMessagesStreamParser
{
 public:
  typedef std::shared_ptr<const MessageType> PointerToConstValue;

  std::list<PointerToConstValue> parse(const std::string& data);

 private:
  std::vector<char> m_buffer;
};

