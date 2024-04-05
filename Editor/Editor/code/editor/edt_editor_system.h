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
		//char buf[64] = "objects/getaur/scene.gltf";
		char buf[64] = "objects/backpack/backpack.obj";
	};

}

namespace edt = editor;