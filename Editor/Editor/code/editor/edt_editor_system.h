#pragma once
#include <engine/common/common.h>

namespace editor
{
	class EditorSystem
	{
	public:
		EditorSystem();
		bool show_toolbar();

	private:
		char buf[64] = "getaur/scene.gltf";
	};

}

namespace edt = editor;