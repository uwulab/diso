#ifndef __DISO_UTILS_HPP__
#define __DISO_UTILS_HPP__

#pragma once

#include <spdlog/spdlog.h>
#include <cstring>
#include <cstdlib>
#include <string>
#include <variant>
#include "diso_error.hpp"
#include "diso_checksum.hpp"

template <class... Ts>
struct overloaded : Ts...
{
  using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

using diso_result_checksum = std::variant<checksum, diso_error>;
diso_result_checksum parse_checksum_option(const char *option_val)
{
  diso_result_checksum c;
  size_t val_len = strlen(option_val);
  checksum_type t;
  bool ok = false;
  for (size_t i = 0; i < val_len; i++)
  {
    if (option_val[i] == ':')
    {
      if (strncmp(option_val, "sha256:", 7) == 0)
      {
        t = SHA256;
      }
      else if (strncmp(option_val, "md5:", 4) == 0)
      {
        t = MD5;
      }
      else
      {
        diso_error error;
        error.level = SPDLOG_LEVEL_ERROR;
        error.code = -1;
        error.reason = spdlog::fmt_lib::format("Unsupported checksum type and value: {}", option_val);
        c = std::move(error);
        break;
      }
      if (i + 1 < val_len)
      {
        c = checksum(t, std::string(option_val + i + 1));
      }
      else
      {
        diso_error error;
        error.level = SPDLOG_LEVEL_ERROR;
        error.code = -1;
        error.reason = "Empty checksum value";
        c = std::move(error);
      }

      break;
    }
  }
  return c;
}

using diso_result_buffer_size = std::variant<unsigned long long, diso_error>;

diso_result_buffer_size parse_buffer_size(const char *option_val)
{
  diso_result_buffer_size buffer_size;
  char *unitptr = nullptr;
  unsigned long long quantity = strtoull(optarg, &unitptr, 10);
  size_t multiplier = 1;
  if (unitptr)
  {
    if (strlen(unitptr) > 1)
    {
      spdlog::critical("Invalid buffer size: {}", optarg);
      exit(1);
    }
    switch (unitptr[0])
    {
    case 'k':
    case 'K':
      multiplier = 1024;
      break;
    case 'm':
    case 'M':
      multiplier = 1024 * 1024;
      break;
    case 'g':
    case 'G':
      multiplier = 1024 * 1024 * 1024;
      break;
    default: {
      diso_error error;
      error.level = SPDLOG_LEVEL_ERROR;
      error.code = -2;
      error.reason = spdlog::fmt_lib::format("Invalid buffer size: {}", optarg);
      buffer_size = std::move(error);
      return buffer_size;
    }
    }
  }
  if (quantity * multiplier <= 0)
  {
    diso_error error;
    error.level = SPDLOG_LEVEL_ERROR;
    error.code = -2;
    error.reason = spdlog::fmt_lib::format("Invalid buffer size: {}", optarg);
    buffer_size = std::move(error);
    return buffer_size;
  }
  buffer_size = quantity * multiplier;
  return buffer_size;
}

#endif
