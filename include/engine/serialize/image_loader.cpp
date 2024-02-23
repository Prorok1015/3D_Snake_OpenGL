#include "image_loader.h"
#include "../common/common.h"

std::unique_ptr<Image> load_image(const char* file)
{
    Image img;
    img.flip(Image::ImageFlip::VERTICAL);
    if (!img.open(file)) {
        ASSERT_FAIL("Cant open image file");
        return nullptr;
    }
    return std::make_unique<Image>(std::move(img));
}
