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

		glm::vec3 forward() const { return glm::rotate(get_orientation(), glm::vec3(0.0f, 0.0f, 1.0f)); }
		glm::vec3 back() const { return -forward(); }
		glm::vec3 right() const { return glm::rotate(get_orientation(), glm::vec3(1.0f, 0.0f, 0.0f)); }
		glm::vec3 left() const { return -right(); }
		glm::vec3 up() const { return glm::rotate(get_orientation(), glm::vec3(0.0f, 1.0f, 0.0f)); }
		glm::vec3 down() const { return -up(); }

		glm::mat4 to_matrix() const {
			glm::mat4 result{ 1.0 };
			result *= glm::translate(glm::mat4(1.0), get_pos());
			result *= glm::toMat4(get_orientation());
			result *= glm::scale(glm::mat4(1.0), get_scale());
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
			auto tf = glm::lookAt(pos, at, down());

			orintation = glm::eulerAngles(glm::quat_cast(inverse(tf)));
		}

		float get_pitch() const { return pitch; }
		void set_pitch(float radian) { pitch = radian; }

		float get_roll() const { return roll; }
		void set_roll(float radian) { roll = radian; }

		float get_yaw() const { return yaw; }
		void set_yaw(float radian) { yaw = radian; }

		glm::vec3 get_pos() const { return pos; }
		void set_pos(glm::vec3 val) { pos = val; }

		glm::vec3 get_scale() const { return scale; }
		void set_scale(glm::vec3 val) { scale = val; }

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

		glm::vec3 scale{ 1 };
		glm::vec3 pos{ 0 };
	};

}

namespace scn = scene;