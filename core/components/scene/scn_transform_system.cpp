#include "scn_transform_system.h"
#include "common.h"
#include "scn_model.h"
#include "ecs_common_system.h"
#include "wnd_window_system.h"
#include "logger/engine_log.h"
#include <execution>

struct is_local_transform_invalidated {};
struct is_transform_will_update_by_parent {};

void calc_interpolated_scaling(glm::vec3& Out, float AnimationTimeTicks, const res::animation_node& anim);
void calc_interpolated_position(glm::vec3& Out, float AnimationTimeTicks, const res::animation_node& anim);
void calc_interpolated_rotation(glm::quat& Out, float AnimationTimeTicks, const res::animation_node& anim);
void calc_world_transforms(ecs::entity ent, const float ticks, const res::animation& anim, std::vector<glm::mat4>& out);
void calc_world_transforms(ecs::entity ent);
void load_bone_offsets(ecs::entity ent, std::vector<glm::mat4>& out);

void on_validate_local_transform(ecs::entity ent)
{
    /*if (auto* name = ecs::registry.try_get<scn::name_component>(ent)) {
        egLOG("", "name {} child is validated!", name->name);
    }
    else {
        egLOG("", "name UNLNOWN child is validated!");
    }*/
    if (ecs::registry.all_of<is_transform_will_update_by_parent>(ent)) {
        return;
    }

    ecs::registry.remove<is_local_transform_invalidated>(ent);
    ecs::registry.emplace<is_transform_will_update_by_parent>(ent);
    if (ecs::registry.all_of<scn::children_component>(ent))
    {
        auto& children = ecs::registry.get<scn::children_component>(ent).children;
        for (auto& child : children)
        {
            on_validate_local_transform(child);
        }
    }
}

void on_invalidate_local_transform(ecs::entity ent)
{
    if (ecs::registry.any_of<is_transform_will_update_by_parent, is_local_transform_invalidated>(ent)) {
        return;
    }
    /*
    if (auto* name = ecs::registry.try_get<scn::name_component>(ent)) {
        egLOG("", "name {} is invalidated!", name->name);
    }
    else {
        egLOG("", "name UNLNOWN is invalidated!");
    }*/

    ecs::registry.emplace<is_local_transform_invalidated>(ent);
    if (ecs::registry.all_of<scn::children_component>(ent))
    {
        auto& children = ecs::registry.get<scn::children_component>(ent).children;
        for (auto& child : children)
        {
            on_validate_local_transform(child);
        }
    }
}

void scn::init_transform_system()
{
    entt::sigh_helper {ecs::registry}
    .with<scn::local_transform>()
    .on_construct<&on_invalidate_local_transform>()
    .on_update<&on_invalidate_local_transform>();
}

void scn::deinit_transform_system()
{
    ecs::registry.on_construct<scn::local_transform>().disconnect<&on_invalidate_local_transform>();
    ecs::registry.on_update<scn::local_transform>().disconnect<&on_invalidate_local_transform>();
}

void scn::update_transform_system(float time_second)
{
    auto entts = ecs::registry.view<is_local_transform_invalidated>();
    std::for_each(std::execution::unseq, entts.begin(), entts.end(), [](auto ent) { calc_world_transforms(ent); });
}

void scn::update_bone_offsets_system(float dt)
{
    for (ecs::entity ent : ecs::registry.view<scn::model_root_component, scn::animations_component, scn::playable_animation>())
    {
        auto& root = ecs::registry.get<scn::model_root_component>(ent);
        root.data.bones_matrices.clear();
        load_bone_offsets(ent, root.data.bones_matrices);
    }
}

void load_bone_offsets(ecs::entity ent, std::vector<glm::mat4>& out)
{
    if (ecs::registry.all_of<scn::bone_component, scn::world_transform>(ent))
    {
        auto& bone = ecs::registry.get<scn::bone_component>(ent);
        auto& trans = ecs::registry.get<scn::world_transform>(ent);
        out.push_back(trans.world * bone.offset);


    }
    if (ecs::registry.all_of<scn::children_component>(ent))
    {
        auto& children = ecs::registry.get<scn::children_component>(ent).children;
        for (auto& child : children)
        {
            load_bone_offsets(child, out);
        }
    }
}

void scn::update_animation_system(float dt)
{
	for (ecs::entity ent : ecs::registry.view<scn::model_root_component, scn::animations_component, scn::playable_animation>())
	{
		auto& root = ecs::registry.get<scn::model_root_component>(ent);
		auto& anims = ecs::registry.get<scn::animations_component>(ent).animations;
		auto& play = ecs::registry.get<scn::playable_animation>(ent);

        auto it = std::find_if(anims.begin(), anims.end(), [&name = play.name](auto& anim) {
            return anim.name == name;
            });

        if (it != anims.end())
        {
            /*if (auto* name = ecs::registry.try_get<scn::name_component>(ent)) {
                egLOG("", "anim update started with: {}", name->name);
            } else {
                egLOG("", "anim update started with: UNNEMED");
            }*/

            auto& playable_animation = *it;
            play.current_tick += dt;
            const float ticks_per_second = playable_animation.ticks_per_second > 0 ? playable_animation.ticks_per_second : 25.0f;
            const float time_in_ticks = play.current_tick * ticks_per_second;
            float animation_time_ticks = fmod(time_in_ticks, (float)playable_animation.duration);
            if (time_in_ticks > playable_animation.duration) {
                if (play.is_repeat_animation) {
                    play.current_tick = 0.f;
                } else {
                    animation_time_ticks = playable_animation.duration;
                }
            }

            std::vector<glm::mat4> result;
            calc_world_transforms(ent, animation_time_ticks, playable_animation, result);

        }
	}
}

void calc_world_transforms(ecs::entity ent)
{
    glm::mat4 local = glm::mat4{ 1.0 };
    glm::mat4 parent = glm::mat4{ 1.0 };

    if (ecs::registry.all_of<scn::parent_component>(ent))
    {
        auto& parent_ = ecs::registry.get<scn::parent_component>(ent);
        if (ecs::registry.all_of<scn::world_transform>(parent_.parent))
        {
            auto& parent_trans = ecs::registry.get<scn::world_transform>(parent_.parent);
            parent = parent_trans.world;
        }
    }

    if (ecs::registry.all_of<scn::local_transform>(ent))
    {
        auto& trans = ecs::registry.get<scn::local_transform>(ent);
        local = trans.local;
        ecs::registry.emplace_or_replace<scn::world_transform>(ent, parent * local);
    }

    /*if (auto* name = ecs::registry.try_get<scn::name_component>(ent)) {
        egLOG("", "name {} is world calculated!", name->name);
    }
    else {
        egLOG("", "name UNLNOWN is world calculated!");
    }*/

    ecs::registry.remove<is_local_transform_invalidated>(ent);
    ecs::registry.remove<is_transform_will_update_by_parent>(ent);
    
    if (ecs::registry.all_of<scn::children_component>(ent))
    {
        auto& children = ecs::registry.get<scn::children_component>(ent);
        for (auto& child : children.children)
        {
            calc_world_transforms(child);
        }
    }
}


void calc_world_transforms(ecs::entity ent, const float ticks, const res::animation& anim, std::vector<glm::mat4>& out)
{
    if (ecs::registry.all_of<scn::keyframes_component>(ent))
    {
        auto& keys_component = ecs::registry.get<scn::keyframes_component>(ent);
        auto& keys = keys_component.keyframes;
        if (auto it = keys.find(anim.name); it != keys.end()) {
            // Interpolate scaling and generate scaling transformation matrix
            glm::vec3 scaling;
            calc_interpolated_scaling(scaling, ticks, it->second);
            glm::mat4 scaling_m = glm::scale(scaling);

            // Interpolate rotation and generate rotation transformation matrix
            glm::quat rotation_q;
            calc_interpolated_rotation(rotation_q, ticks, it->second);
            glm::mat4 rotation_m = glm::toMat4(rotation_q);

            // Interpolate translation and generate translation transformation matrix
            glm::vec3 translation;
            calc_interpolated_position(translation, ticks, it->second);
            glm::mat4 translation_m = glm::translate(translation);

            // Combine the above transformations
            auto local = translation_m * rotation_m * scaling_m;
            ecs::registry.emplace_or_replace<scn::local_transform>(ent, local);
        }
    }

    if (ecs::registry.all_of<scn::children_component>(ent))
    {
        auto& children = ecs::registry.get<scn::children_component>(ent).children;
        for (auto& child : children)
        {
            calc_world_transforms(child, ticks, anim, out);
        }
    }
}

template<class T>
std::size_t find_keyframe_index(const std::vector<T>& arr, float time)
{
    for (std::size_t i = 0; i < arr.size() - 1; i++) {
        float t = (float)arr[i + 1].time;
        if (time < t) {
            return i;
        }
    }

    return 0;
}


void calc_interpolated_scaling(glm::vec3& Out, float AnimationTimeTicks, const res::animation_node& anim)
{
    // we need at least two values to interpolate...
    if (anim.scale_keys.size() == 1) {
        Out = anim.scale_keys[0].value;
        return;
    }

    std::size_t ScalingIndex = find_keyframe_index(anim.scale_keys, AnimationTimeTicks);
    std::size_t NextScalingIndex = ScalingIndex + 1;

    float t1 = anim.scale_keys[ScalingIndex].time;
    float t2 = anim.scale_keys[NextScalingIndex].time;
    float DeltaTime = t2 - t1;
    float Factor = std::clamp((AnimationTimeTicks - t1) / DeltaTime, 0.0f, 1.f);
    assert(Factor >= 0.0f && Factor <= 1.0f);
    const glm::vec3& Start = anim.scale_keys[ScalingIndex].value;
    const glm::vec3& End = anim.scale_keys[NextScalingIndex].value;
    glm::vec3 Delta = End - Start;
    Out = Start + Factor * Delta;
}

void calc_interpolated_position(glm::vec3& Out, float AnimationTimeTicks, const res::animation_node& anim)
{
    // we need at least two values to interpolate...
    if (anim.pos_keys.size() == 1) {
        Out = anim.pos_keys[0].value;
        return;
    }

    std::size_t PositionIndex = find_keyframe_index(anim.pos_keys, AnimationTimeTicks);
    std::size_t NextPositionIndex = PositionIndex + 1;

    float t1 = (float)anim.pos_keys[PositionIndex].time;
    float t2 = (float)anim.pos_keys[NextPositionIndex].time;
    float DeltaTime = t2 - t1;
    float Factor = std::clamp((AnimationTimeTicks - t1) / DeltaTime, 0.0f, 1.f);

    const glm::vec3& Start = anim.pos_keys[PositionIndex].value;
    const glm::vec3& End = anim.pos_keys[NextPositionIndex].value;
    glm::vec3 Delta = End - Start;
    Out = Start + Factor * Delta;
}

void calc_interpolated_rotation(glm::quat& Out, float AnimationTimeTicks, const res::animation_node& anim)
{
    // we need at least two values to interpolate...
    if (anim.rotate_keys.size() == 1) {
        Out = anim.rotate_keys[0].value;
        return;
    }

    std::size_t RotationIndex = find_keyframe_index(anim.rotate_keys, AnimationTimeTicks);
    std::size_t NextRotationIndex = RotationIndex + 1;

    float t1 = (float)anim.rotate_keys[RotationIndex].time;
    float t2 = (float)anim.rotate_keys[NextRotationIndex].time;
    float DeltaTime = t2 - t1;
    float Factor = std::clamp((AnimationTimeTicks - t1) / DeltaTime, 0.0f, 1.f);

    const glm::quat& StartRotationQ = anim.rotate_keys[RotationIndex].value;
    const glm::quat& EndRotationQ = anim.rotate_keys[NextRotationIndex].value;
    Out = glm::slerp(StartRotationQ, EndRotationQ, Factor);
}