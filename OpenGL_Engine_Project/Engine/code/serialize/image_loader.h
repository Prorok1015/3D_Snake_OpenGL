#pragma once
#include <Image/Image.h>
#include <memory>

std::unique_ptr<Image> load_image(const char* file);

