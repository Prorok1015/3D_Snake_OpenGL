#include "edt_editor_init.h"
#include "edt_editor_system.h"
#include <application.h>

namespace components {

	void editor_init(ds::AppDataStorage& data)
	{
		auto& editor = data.construct<edt::EditorSystem>();
	}

	void editor_term(ds::AppDataStorage& data)
	{

		data.destruct<edt::EditorSystem>();
	}

}