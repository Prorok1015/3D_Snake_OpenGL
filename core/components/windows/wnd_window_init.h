#pragma once
#include <ds_store.hpp>

namespace components {

	void window_init(ds::AppDataStorage& data);
	void window_term(ds::AppDataStorage& data);
}

namespace com = components;