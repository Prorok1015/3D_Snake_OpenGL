#pragma once
#include "../common/ds_store.hpp"

namespace components{

	void component_init(ds::AppDataStorage& data);
	void component_term(ds::AppDataStorage& data);
}

namespace com = components;