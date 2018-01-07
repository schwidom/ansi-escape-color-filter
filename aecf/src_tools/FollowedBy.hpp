#pragma once

#include <string>

class FollowedBy
{
public:
  FollowedBy( std::string first, std::string rest);

  const std::string next();

private:
  bool m_gotFirst;

  std::string m_first;
  std::string m_rest;
};
