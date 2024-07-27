#pragma once
#include <Image/Image.h>
#include <memory>

std::unique_ptr<stb_image::Image> load_image(const std::string_view file);

