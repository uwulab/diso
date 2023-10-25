#include <aria2/aria2.h>
#include <getopt.h>
#include <spdlog/spdlog.h>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <variant>
#include "diso_error.hpp"
#include "diso_checksum.hpp"
#include "diso_utils.hpp"

int main(int argc, const char *argv[])
{
  // diso -u URL1 -u URL2 -o /dev/rdisk2 [--bs=64M] [--decompress=yes]
  // [--file=name.iso] [--checksum=sha256:abcd..|file:file.sha256]
  static int verbose_flag;
  static int decompress_flag;
  std::vector<std::string> sources;
  std::string out, extract_file;
  diso_result_checksum result_checksum;
  checksum parsed_checksum;
  diso_result_buffer_size result_buffer_size;
  unsigned long long buffer_size = 64 * 1024 * 1024; // 64M
  static struct option long_options[] = {
      /* These options set a flag. */
      {"verbose", no_argument, &verbose_flag, 1},
      {"decompress", required_argument, 0, 'd'},
      {"url", required_argument, 0, 'u'},
      {"output", required_argument, 0, 'o'},
      {"buffer-size", required_argument, 0, 'b'},
      {"checksum", required_argument, 0, 'c'},
      {"file", required_argument, 0, 'f'},
      {0, 0, 0, 0}};
  int c;
  spdlog::set_level(spdlog::level::debug);

  while (1)
  {
    int option_index = 0;
    c = getopt_long(argc, (char *const *)argv, "d:u:o:b:c:f:v", long_options,
                    &option_index);
    if (c == -1)
      break;
    switch (c)
    {
    case 1:
      verbose_flag = 1;
      break;
    case 'd':
      decompress_flag = strcmp("yes", optarg) == 0;
      break;
    case 'u':
      sources.emplace_back(optarg);
      break;
    case 'o':
      out = std::string(optarg);
      break;
    case 'b':
    {
      result_buffer_size = parse_buffer_size(optarg);
      std::visit(overloaded{[](const diso_error &error)
                            {
                              error.log_error_and_exit();
                            },
                            [&buffer_size](const unsigned long long &bs)
                            {
                              buffer_size = bs;
                            }},
                 result_buffer_size);
      break;
    }
    case 'c':
    {
      result_checksum = parse_checksum_option(optarg);
      std::visit(overloaded{[](const diso_error &error)
                            {
                              error.log_error_and_exit();
                            },
                            [&parsed_checksum](const checksum &c)
                            {
                              parsed_checksum = c;
                            }},
                 result_checksum);
      break;
    }
    case 'f':
      extract_file = std::string(optarg);
      break;
    default:
      break;
    }
  }

  if (verbose_flag)
  {
    spdlog::info("verbose: {}", verbose_flag);
    spdlog::info("source url(s): {}", sources.size());
    for (auto &u : sources)
    {
      spdlog::info(" - {}", u);
    }
    if (extract_file.size())
    {
      spdlog::info("extract image file name: {}", extract_file);
    }
    spdlog::info("output write to: {}", out);
    spdlog::info("checksum type: {}, value: {}", parsed_checksum.type, parsed_checksum.value);
    spdlog::info("buffer_size: {} bytes", buffer_size);
    spdlog::info("decompress: {}", decompress_flag);
  }

  aria2::libraryInit();
  return 0;
}
