#pragma once
#include "common.h"
#include <boost/json.hpp>

namespace json = boost::json;

namespace glm {
    void tag_invoke(json::value_from_tag, json::value& jv, const glm::vec2& v);
    void tag_invoke(json::value_from_tag, json::value& jv, const glm::vec3& v);
    void tag_invoke(json::value_from_tag, json::value& jv, const glm::vec4& v);
    void tag_invoke(json::value_from_tag, json::value& jv, const glm::mat4& m);
}
