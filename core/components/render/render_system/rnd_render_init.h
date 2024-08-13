#pragma once
#include <ds_store.hpp>

namespace components {

	void render_init(ds::AppDataStorage& data);
	void render_term(ds::AppDataStorage& data);
}

namespace com = components;