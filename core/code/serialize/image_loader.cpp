#include "image_loader.h"
#include "../common/common.h"

std::unique_ptr<stb_image::Image> load_image(const std::string_view file)
{
    stb_image::Image img;
    img.flip(stb_image::Image::ImageFlip::VERTICAL);
    if (!img.open(file)) {
        ASSERT_FAIL("Cant open image file");
        return nullptr;
    }
    return std::make_unique<stb_image::Image>(std::move(img));
}
