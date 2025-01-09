#include "scn_transform_system.h"
#include "common.h"
#include "scn_model.h"
#include "ecs/ecs_common_system.h"
#include "wnd_window_system.h"

void calc_interpolated_scaling(glm::vec3& Out, float AnimationTimeTicks, const res::animation_node& anim);
void calc_interpolated_position(glm::vec3& Out, float AnimationTimeTicks, const res::animation_node& anim);
void calc_interpolated_rotation(glm::quat& Out, float AnimationTimeTicks, const res::animation_node& anim);
void calc_world_transforms(ecs::entity ent, const float ticks, const res::animation& anim, std::vector<glm::mat4>& out);
void calc_world_transforms(ecs::entity ent);

void scn::update_transform_system(float time_second)
{
    for (ecs::entity anchor : ecs::filter<scn::scene_anchor_component>())
    {
        calc_world_transforms(anchor);
    }
}

void scn::update_animation_system(float time_second)
{
	for (ecs::entity ent : ecs::filter<scn::model_root_component, scn::animations_component>())
	{
		auto* root = ecs::get_component<scn::model_root_component>(ent);
		auto* animation = ecs::get_component<scn::animations_component>(ent);

        if (!animation->animations.empty())
        {
            auto& playable_animation = animation->animations.front();
            const float ticks_per_second = playable_animation.ticks_per_second > 0 ? playable_animation.ticks_per_second : 25.0f;
            const float time_in_ticks = time_second * ticks_per_second;
            const float animation_time_ticks = fmod(time_in_ticks, (float)playable_animation.duration);

            std::vector<glm::mat4> result;
            result.reserve(root->data.bones.size());

            calc_world_transforms(ent, animation_time_ticks, playable_animation, result);

            root->data.bones_matrices = result;
        }
	}
}

void calc_world_transforms(ecs::entity ent)
{
    if (auto* animation = ecs::get_component<scn::animations_component>(ent)) {
        if (!animation->animations.empty())
            return;
    }

    glm::mat4 local = glm::mat4{ 1.0 };
    glm::mat4 parent = glm::mat4{ 1.0 };

    if (auto* parent_ = ecs::get_component<scn::parent_component>(ent))
    {
        if (auto* parent_trans = ecs::get_component<scn::transform_component>(parent_->parent))
        {
            parent = parent_trans->world;
        }
    }

    if (auto* trans = ecs::get_component<scn::transform_component>(ent))
    {
        local = trans->local;
        trans->world = parent * local;
    }

    if (auto* children = ecs::get_component<scn::children_component>(ent))
    {
        for (auto& child : children->children)
        {
            calc_world_transforms(child);
        }
    }
}


void calc_world_transforms(ecs::entity ent, const float ticks, const res::animation& anim, std::vector<glm::mat4>& out)
{
    glm::mat4 local = glm::mat4{ 1.0 };
    glm::mat4 parent = glm::mat4{ 1.0 };

    if (auto* parent_ = ecs::get_component<scn::parent_component>(ent))
    { 
        if (auto* parent_trans = ecs::get_component<scn::transform_component>(parent_->parent))
        {
            parent = parent_trans->world;
        }
    }

    if (auto* trans = ecs::get_component<scn::transform_component>(ent))
    {
        local = trans->local;
    }

    if (auto* keyframes = ecs::get_component<scn::keyframes_component>(ent))
    {
        auto& keys = keyframes->keyframes;
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
            local = translation_m * rotation_m * scaling_m;
        }
    }

    if (auto* bone = ecs::get_component<scn::bone_component>(ent))
    {
        out.push_back(parent * local * bone->offset);
    }

    if (auto* trans = ecs::get_component<scn::transform_component>(ent))
    {
        trans->world = parent * local;
    }

    if (auto* children = ecs::get_component<scn::children_component>(ent))
    {
        for (auto& child : children->children)
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

    //glm::mat4 matrix = glm::interpolate(glm::toMat4(StartRotationQ), glm::toMat4(EndRotationQ), Factor);

    //glm::vec3 s;
    //glm::vec3 po;
    //glm::vec3 sk;
    //glm::vec4 p;
    //glm::quat orientation;
    //glm::decompose(matrix, s, orientation, po, sk, p);
    //Out = glm::toMat4(glm::normalize(orientation)); // glm::eulerAngles(o);
}