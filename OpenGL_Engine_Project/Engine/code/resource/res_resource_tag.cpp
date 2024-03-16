#include "res_resource_tag.h"

//
//resource::RelativeTag resource::operator+ (const resource::Tag& a, const resource::Tag& b)
//{
//	if (a == b) {
//		return RelativeTag{ a };
//	}
//
//	return RelativeTag{ a, b };
//}

resource::Tag resource::operator+ (const resource::Tag& a, const resource::Tag& b)
{
	if (a == b) {
		return a;
	}

	return RelativeTag{ a, b };
}

resource::RelativeTag resource::operator+ (resource::RelativeTag a, resource::Tag b)
{
	return a.append(b);
}

resource::RelativeTag resource::operator+ (resource::RelativeTag a, resource::RelativeTag b)
{
	return a.append(b);
}