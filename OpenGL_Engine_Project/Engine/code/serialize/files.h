#pragma once

#include <string>

extern bool write_binary_file(std::string filename, const char* data, size_t size);
extern bool read_binary_file(std::string filename, char* data, size_t size);
