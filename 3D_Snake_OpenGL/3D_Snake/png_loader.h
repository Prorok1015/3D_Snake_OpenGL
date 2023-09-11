#pragma once

struct png_result
{
	unsigned id;
	int width;
	int height;

	operator bool() { return !has_error(); }
	bool has_error() { return id == 0; }
};

png_result _png_load(const char* file);
