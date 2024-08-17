#include "res_resource_texture.h"
#include "res_resource_system.h"
#include <image_loader.h>


resource::Picture::Picture(const resource::Tag& tag)
	: resource::Resource(tag)
{
	std::string path = ResourceSystem::get_absolut_path(tag);
	if (path.empty()) {
		return;
	}

	if (std::unique_ptr<stb_image::Image> img = load_image(path.data())) {
		size_.x = img->width();
		size_.y = img->height();
		channels_ = img->channels_count();
		data_ = img->pop_data();
	}
}

resource::Picture::~Picture()
{
	stb_image::Image::free_image_data(data_);
}