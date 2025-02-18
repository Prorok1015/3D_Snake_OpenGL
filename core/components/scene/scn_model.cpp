#include "scn_model.h"
#include "ecs_common_system.h"
#include "scn_glm_json_convert.h"

namespace json = boost::json;

void scn::tag_invoke(json::value_from_tag, json::value& jv, const keyframes_component& c) {
    /*json::object obj;
    for (const auto& [key, value] : c.keyframes) {
        obj[key] = json::value_from(value);
    }
    jv = std::move(obj);*/
}

void scn::tag_invoke(json::value_from_tag, json::value& jv, const name_component& c) {
    jv = c.name;
}

void scn::tag_invoke(json::value_from_tag, json::value& jv, const mesh_component& c) {
    //jv = json::value_from(c.mesh);
}

void scn::tag_invoke(json::value_from_tag, json::value& jv, const model_root_component& c) {
    //jv = json::value_from(c.data);
}

void scn::tag_invoke(json::value_from_tag, json::value& jv, const animations_component& c) {
    json::array arr;
    for (const auto& animation : c.animations) {
        //arr.push_back(json::value_from(animation));
    }
    jv = std::move(arr);
}

void scn::tag_invoke(json::value_from_tag, json::value& jv, const bone_component& c) {
    jv = json::value_from(c.offset);
}

void scn::tag_invoke(json::value_from_tag, json::value& jv, const parent_component& c) {
    //jv = json::value_from(c.parent);
}

void scn::tag_invoke(json::value_from_tag, json::value& jv, const children_component& c) {
    json::array arr;
    for (const auto& child : c.children) {
        arr.push_back(to_json(child));
    }
    jv = { "children", std::move(arr) };
}

void scn::tag_invoke(json::value_from_tag, json::value& jv, const local_transform& c) {
    jv = {
        { "local", json::value_from(c.local) }
    };
}

void scn::tag_invoke(json::value_from_tag, json::value& jv, const scene_anchor_component& c) {
    jv = {"__type", "world_anchor"};
}

void scn::tag_invoke(json::value_from_tag, json::value& jv, const renderable& c) {
    jv = true;
}

void scn::tag_invoke(json::value_from_tag, json::value& jv, const sky_component& c) {
    jv = {
        //{ "data", json::value_from(c.data) },
        //{ "mesh", json::value_from(c.mesh) },
        //{ "cube_map", /*json::value_from(c.cube_map)*/ "TMP" }
    };
}

void scn::tag_invoke(json::value_from_tag, json::value& jv, const directional_light& c) {
    jv = {
        { "direction", json::value_from(c.direction) },
        { "diffuse", json::value_from(c.diffuse) },
        { "ambient", json::value_from(c.ambient) },
        { "specular", json::value_from(c.specular) }
    };
}

void scn::tag_invoke(json::value_from_tag, json::value& jv, const playable_animation& c) {
    jv = {
        { "name", c.name },
        { "current_tick", c.current_tick },
        { "is_repeat_animation", c.is_repeat_animation }
    };
}

json::value scn::to_json(const ecs::entity& e) {
    json::object jv;
    if (ecs::registry.all_of<scn::name_component>(e)) {
        auto& name = ecs::registry.get<scn::name_component>(e);
        jv["name"] = json::value_from(name);
    }
    if (ecs::registry.all_of<scn::mesh_component>(e)) {
        auto& mesh = ecs::registry.get<scn::mesh_component>(e);
        //jv["mesh"] = json::value_from(*mesh);
    }
    if (ecs::registry.all_of<scn::model_root_component>(e)) {
        auto& model_root = ecs::registry.get<scn::model_root_component>(e);
        //jv["model_root"] = json::value_from(*model_root);
    }   
    if (ecs::registry.all_of<scn::animations_component>(e)) {   
        auto& animations = ecs::registry.get<scn::animations_component>(e);
        //jv["animations"] = json::value_from(*animations);
    }
    if (ecs::registry.all_of<scn::bone_component>(e)) {
        auto& bone = ecs::registry.get<scn::bone_component>(e);
        //jv["bone"] = json::value_from(*bone);
    }
    if (ecs::registry.all_of<scn::parent_component>(e)) {
        auto& parent = ecs::registry.get<scn::parent_component>(e);
        //jv["parent"] = json::value_from(*parent);
    }
    if (ecs::registry.all_of<scn::children_component>(e)) {
        auto& children = ecs::registry.get<scn::children_component>(e);
        //jv["children"] = json::value_from(*children);
    }
    if (ecs::registry.all_of<scn::local_transform>(e)) {
        auto& transform = ecs::registry.get<scn::local_transform>(e);
        //jv["transform"] = json::value_from(*transform);
    }
    if (ecs::registry.all_of<scn::sky_component>(e)) {
        auto& sky = ecs::registry.get<scn::sky_component>(e);
        //jv["sky"] = json::value_from(*sky);
    }
    if (ecs::registry.all_of<scn::directional_light>(e)) {
        auto& directional_light = ecs::registry.get<scn::directional_light>(e);
        //jv["directional_light"] = json::value_from(*directional_light);  
    }
    if (ecs::registry.all_of<scn::playable_animation>(e)) {
        auto& playable_animation = ecs::registry.get<scn::playable_animation>(e);
        //jv["playable_animation"] = json::value_from(*playable_animation);
    }   
    return jv;
}
