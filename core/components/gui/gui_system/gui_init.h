#pragma once
#include <ds_store.hpp>

namespace components {

	void gui_init(ds::AppDataStorage& data);
	void gui_term(ds::AppDataStorage& data);
}

namespace com = components;