#ifndef __DISO_ERROR_HPP__
#define __DISO_ERROR_HPP__

#pragma once

#include <spdlog/spdlog.h>
#include <cstdlib>
#include <string>

class diso_error
{
public:
  int level = SPDLOG_LEVEL_INFO;
  int code = 0;
  std::string reason;
  void log_error_and_exit() const
  {
    switch (this->level)
    {
    case SPDLOG_LEVEL_TRACE:
      spdlog::trace(this->reason);
      break;
    case SPDLOG_LEVEL_DEBUG:
      spdlog::debug(this->reason);
      break;
    case SPDLOG_LEVEL_INFO:
      spdlog::info(this->reason);
      break;
    case SPDLOG_LEVEL_WARN:
      spdlog::warn(this->reason);
      break;
    case SPDLOG_LEVEL_ERROR:
      spdlog::error(this->reason);
      break;
    case SPDLOG_LEVEL_CRITICAL:
      spdlog::critical(this->reason);
      break;
    default:
      spdlog::critical(this->reason);
      break;
    }
    exit(this->code);
  }
};

#endif
