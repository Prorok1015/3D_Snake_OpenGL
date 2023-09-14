#pragma once
#include <string_view>

#ifndef IMAGE
#define IMAGE
class Image
{
public:
	enum class ImageType
	{
		NONE, PNG, JPG, BMP, TGA
	};

	enum class ImageFlip
	{
		NONE, VERTICAL
	};
public:
	Image() = default;
    Image(const std::string_view filename);
	Image(const Image&) = delete;
	Image(Image&&) = delete;
	Image& operator= (const Image&) = delete;
	Image& operator= (Image&&) = delete;
	~Image();

	bool open(const std::string_view filename) { return read(filename); }
	ImageType type(const std::string_view filename);
	bool write(const std::string_view filename);
	unsigned char* pop_data() { auto res = data_; data_ = nullptr; return res; };
	int channels_count() { return channels_; };
	int width() { return width_; };
	int height() { return height_; };
	void flip(ImageFlip fp) { flipFlag_ = fp; }

private:
    bool read(const std::string_view filename);
	void set_image_flip(ImageFlip flag);

private:
	unsigned char*  data_ = nullptr;
	int width_ = 0;
	int height_ = 0;
	int channels_ = 0;
	size_t size_ = 0;
	ImageFlip flipFlag_ = ImageFlip::NONE;
};

#endif // IMAGE



