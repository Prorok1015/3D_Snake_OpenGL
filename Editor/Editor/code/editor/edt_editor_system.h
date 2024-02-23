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
		char buf[64] = "Old_Hunter/source/Sketchfab.fbx";
	};

}

namespace edt = editor;