#include "res_resource_tag.h"

resource::Tag resource::operator+ (const resource::Tag& l, const resource::Tag& r)
{
	if (l == r) {
		egLOG("resoure/tag", "Even Tags when resource::operator+ (const resource::Tag& l, const resource::Tag& r)");
		return l;
	}

	const std::string path = std::vformat("{}{}{}", std::make_format_args(l.path(), r.path(), r.name()));
	return Tag(l.protocol(), path);
}
