#pragma once

#include <glm/gtx/quaternion.hpp>
namespace scene {
	class Transform final
	{
	public:
		Transform() = default;
		Transform(glm::vec3 position) : pos(position) {}
		~Transform() = default;
		Transform(const Transform&) = default;
		Transform(Transform&&) = default;
		Transform& operator= (const Transform&) = default;
		Transform& operator= (Transform&&) = default;

		glm::vec3 forward() const { return glm::rotate(get_orientation(), glm::vec3(0.0f, 0.0f, -1.0f)); }
		glm::vec3 back() const { return -forward(); }
		glm::vec3 right() const { return glm::rotate(get_orientation(), glm::vec3(1.0f, 0.0f, 0.0f)); }
		glm::vec3 left() const { return -right(); }
		glm::vec3 up() const { return glm::rotate(get_orientation(), glm::vec3(0.0f, 1.0f, 0.0f)); }
		glm::vec3 down() const { return -up(); }

		glm::mat4 get_view() const {

			glm::quat orientation = get_orientation();
			auto m_ViewMatrix = glm::translate(glm::mat4(1.0f), pos) * glm::toMat4(orientation);
			return glm::inverse(m_ViewMatrix);
		}

		glm::mat4 get_transform() const {
			glm::mat4 result{ 1.f };
			result = glm::translate(result, pos);
			result = glm::rotate(result, roll, glm::vec3(0, 0, 1));
			result = glm::rotate(result, yaw, glm::vec3(0, 1, 0));
			result = glm::rotate(result, pitch, glm::vec3(1, 0, 0));
			return result;
		}

		void add_rotate(float pitch_, float yaw_ = 0.f, float roll_ = 0.f) {
			pitch += pitch_;
			yaw += yaw_;
			roll += roll_;
		}

		void add_position(glm::vec3 addition) {
			pos += addition;
		}

		void move_to(glm::vec3 n_pos) {
			pos = n_pos;
		}

		void look_at(glm::vec3 at) {
			auto tf = glm::lookAt(pos, at, up());

			orintation = glm::eulerAngles(glm::quat_cast(glm::inverse(tf)));
		}

		float get_pitch() const { return pitch; }
		void set_pitch(float radian) { pitch = radian; }

		float get_roll() const { return roll; }
		void set_roll(float radian) { roll = radian; }

		float get_yaw() const { return yaw; }
		void set_yaw(float radian) { yaw = radian; }

		glm::vec3 get_pos() const { return pos; }
		void set_pos(glm::vec3 val) { pos = val; }

		glm::quat get_orientation() const {
			return glm::quat(orintation);
		}

	private:
		union
		{
			struct {
				float pitch;
				float yaw;
				float roll;
			};
			glm::vec3 orintation{ 0 };
		};

		glm::vec3 scale{ 0 };
		glm::vec3 pos{ 0 };
	};

}

namespace scn = scene;