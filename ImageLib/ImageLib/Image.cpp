#include "Image.h"

#include "lib/stb_image.h"
#include "lib/stb_image_write.h"

#include <iostream>
#include <string>
#include <exception>

#define EXCEPT(msg) std::exception(msg)

class Loger {
	std::wstring _msg;
	std::string _func;
	int _line;
	std::wstring _file;
public:
	Loger(std::wstring m, std::string func, int line, std::wstring file)
		: _msg(m),
		_func(func),
		_line(line),
		_file(file) {};

	void Trace() const
	{
		std::wcout	<< L"Дамп ошибки: "	<< std::endl
					<< L"Функция: "		<< std::wstring(_func.begin(), _func.end())	<< std::endl
					<< L"Строка: "		<< _file	<< L" (" << _line <<L")" << std::endl
					<< _msg << std::endl;
	}
};

Image::Image(const char* filename)
{
	try
	{
		stbi_set_flip_vertically_on_load(true);
		if (!read(filename))
		{
			throw Loger(L"Ошибка чтения файла", __func__, __LINE__, __FILEW__);
		}		
		
	}
	catch (Loger& e)
	{
		e.Trace();
	}
	catch (...) {};

};

Image::~Image()
{
	if(!isGetData)
		stbi_image_free(data);
};

bool Image::read(const char* filename)
{
	data = stbi_load(filename, &width, &height, &channels, 0);
	size = width;
	size *= height;
	size *= channels;

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