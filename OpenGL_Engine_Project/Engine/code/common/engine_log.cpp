#include "engine_log.h"
#include <iostream>

#ifdef _DEBUG

void engine::logger(const std::string_view file, int line, const std::string_view msg)
{
	std::cout << msg << " | file: " << file << '(' << line << ')' << std::endl;
}

#endif