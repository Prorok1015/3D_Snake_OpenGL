#pragma once
#include <string>

bool write_binary_file(std::string filename, const char* data, size_t size);
bool read_binary_file(std::string filename, char* data, size_t size);
