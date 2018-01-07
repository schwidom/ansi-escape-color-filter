#include "FollowedBy.hpp"




FollowedBy::FollowedBy(std::string first, std::string rest)
  : m_gotFirst {false} , m_first {first} , m_rest {rest}
{
}

const std::string FollowedBy::next() {
  if(!m_gotFirst) {
    m_gotFirst= true;
    return m_first;
  }
  return m_rest;
}




