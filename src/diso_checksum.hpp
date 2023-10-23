#ifndef __DISO_CHECKSUM_HPP__
#define __DISO_CHECKSUM_HPP__

#pragma once

#include <string>

typedef enum
{
  SHA256 = 0,
  MD5
} checksum_type;

class checksum
{
public:
  checksum() {}
  checksum(checksum_type t, const std::string &v) : type(t), value(v) {}
  checksum_type type;
  std::string value;
};

#endif
