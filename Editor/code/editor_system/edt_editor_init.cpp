#include "edt_editor_init.h"
#include "edt_editor_system.h"
#include <application.h>

namespace components {

	void editor_init(ds::AppDataStorage& data)
	{
		auto& editor = data.construct<edt::EditorSystem>();
		auto& app = data.require<app::Application>();
		app.pre_render += [&editor] { editor.pre_render(); };
	}

	void editor_term(ds::AppDataStorage& data)
	{

		data.destruct<edt::EditorSystem>();
	}

}