#include "Image.h"

#include "lib/stb_image.h"
#include "lib/stb_image_write.h"

#include <iostream>
#include <string>

struct Exeption
{
	std::string exep;

	Exeption(std::string&& s) : exep(s) {};
};

void Image::rotate()
{
	auto* mat = getData();
	// Consider all squares one by one
	for (int x = 0; x < width / 2; x++) {
		// Consider elements in group
		// of 4 in current square
		for (int y = x; y < height - x - 1; y++) {
			// Store current cell in
			// temp variable
			unsigned char temp = mat[x + y];

			// Move values from right to top
			mat[x + y] = mat[y + width - 1 - x];

			// Move values from bottom to right
			mat[y + width - 1 - x]
				= mat[height - 1 - x + width - 1 - y];

			// Move values from left to bottom
			mat[height - 1 - x + width - 1 - y]
				= mat[height - 1 - y + x];

			// Assign temp to left
			mat[height - 1 - y + x] = temp;
		}
	}
}

void Image::mirrorY()
{
	if (height >= 2)
	{
		int col = 0;
		for (; col < width; ++col)
		{
			int i = 0;          // top
			int j = height - 1;  // bottom

			while (i < j) // swap top and bottom row data progressively
			{
				for (int chan = 0; chan < channels; ++chan) {
					auto tmp = data[col + i + chan];
					data[col + i + chan] = data[col + j + chan];
					data[col + j + chan] = tmp;
					//std::swap(data[col + i + chan], data[col + j + chan]);
				}
				++i; 
				--j;
			}
			//std::cout << "i = " << i << "; j = " << j << std::endl;
			//std::cout << "/*=========================*/" << std::endl;
		}

		//std::cout << "       " << col << std::endl;
	}
}

Image::Image(const char* filename)
{
	try
	{
		if (!read(filename))
		{
			Exeption e("bad");
			throw e;
		}
		
		
	}
	catch (Exeption e)
	{
		std::cout << e.exep;
		system("pause");
		exit(1);
	}

};
Image::~Image()
{
	if(!isGetData)
	stbi_image_free(data);
};

bool Image::read(const char* filename)
{
	data = stbi_load(filename, &width, &height, &channels, 0);
	size = width * height * channels;

	return data != NULL;
};

Image::ImageType Image::getTypeFilename(const char* filename)
{
	std::string s(filename);
	if (s._Equal(".png"))
	{
		return ImageType::PNG;
	}
	else if (s._Equal(".jpg"))
	{
		return ImageType::JPG;
	}
	else if (s._Equal(".bmp"))
	{
		return ImageType::BMP;
	}
	else if (s._Equal(".tga"))
	{
		return ImageType::TGA;
	}
	return ImageType::PNG;
}
;
bool Image::write(const char* filename)
{
	ImageType type = getTypeFilename(filename);
	int success = 0;
	switch (type)
	{
	case Image::ImageType::PNG:
		success = stbi_write_png(filename, width, height, channels, data, width * channels);
		break;
	case Image::ImageType::JPG:
		success = stbi_write_jpg(filename, width, height, channels, data, 100);
		break;
	case Image::ImageType::BMP:
		success = stbi_write_bmp(filename, width, height, channels, data);
		break;
	case Image::ImageType::TGA:
		success = stbi_write_tga(filename, width, height, channels, data);
		break;
	}

	return success != 0;
};