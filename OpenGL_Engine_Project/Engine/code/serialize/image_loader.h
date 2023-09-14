#pragma once

struct png_result
{
	unsigned id = 0;
	int width = 0;
	int height = 0;

	operator bool() { return !has_error(); }
	bool has_error() { return id == 0; }
};

png_result load_image(const char* file);
