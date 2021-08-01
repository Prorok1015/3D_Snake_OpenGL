#pragma once

#ifndef IMAGE
#define IMAGE
class Image
{
	enum class ImageType
	{
		PNG, JPG, BMP, TGA
	};
	unsigned char*  data;// = nullptr;

	int width;
	int height;
	int channels;
	size_t size;
	bool isGetData = false;

	bool read(const char* filename);
public:
	Image(const char* filename);
	~Image();

	ImageType getTypeFilename(const char* filename);
	bool write(const char* filename);
	void rotate();
	void mirrorY();
	unsigned char* getData() { isGetData = true; return data; };
	int getChannels() { return channels; };
	int getWidth() { return width; };
	int getHeight() { return height; };
};

#endif // IMAGE



