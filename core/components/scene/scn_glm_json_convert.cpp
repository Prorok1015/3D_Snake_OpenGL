#include "scn_glm_json_convert.h"

void glm::tag_invoke(json::value_from_tag, json::value& jv, const glm::vec2& v) {
    jv = json::array{ v.x, v.y };
}

void glm::tag_invoke(json::value_from_tag, json::value& jv, const glm::vec3& v) {
    jv = json::array{ v.x, v.y, v.z };
}

void glm::tag_invoke(json::value_from_tag, json::value& jv, const glm::vec4& v) {
    jv = json::array{ v.x, v.y, v.z, v.w };
}

void glm::tag_invoke(json::value_from_tag, json::value& jv, const glm::mat4& m) {
    jv = json::array{
        json::array{ m[0][0], m[0][1], m[0][2], m[0][3] },
        json::array{ m[1][0], m[1][1], m[1][2], m[1][3] },
        json::array{ m[2][0], m[2][1], m[2][2], m[2][3] },
        json::array{ m[3][0], m[3][1], m[3][2], m[3][3] }
    };
}

