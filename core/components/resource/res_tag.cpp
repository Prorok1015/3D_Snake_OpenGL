#include "res_tag.h"
#include <engine_log.h>

const res::Tag res::Tag::null{};

res::Tag res::operator+ (const res::Tag& l, const res::Tag& r)
{
	if (l == r) {
		egLOG("resoure/tag", "Equal Tags when resource::operator+ (const resource::Tag& l, const resource::Tag& r)");
		return l;
	}

	const std::string path = std::vformat("{}{}{}", std::make_format_args(l.path(), r.path(), r.name()));
	return Tag(l.protocol(), path);
}
