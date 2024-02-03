#pragma once

// need define GLFW_INCLUDE_NONE for build
#include <GLFW/glfw3.h>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <algorithm>
#include <functional>
#include <vector>
#include "engine_assert.h"
#include "engine_log.h"
#include "open_gl_specific.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

template<class T = void()>
struct Event
{
	using EVENT_SIGATURE_T = T;
	using EVENT_LISTENER_T = std::function<EVENT_SIGATURE_T>;

	auto begin() const { return std::begin(listeners); }
	auto end() const { return std::end(listeners); }
	template<class DELEGATE>
	void operator += (DELEGATE&& f) { listeners.emplace_back(f); }

	template<class ...ARGS>
	void operator() (ARGS&&... args) {
		for (const auto& cb : listeners) {
			cb(args...);
		}
	}
private:
	std::vector<EVENT_LISTENER_T> listeners;

};
