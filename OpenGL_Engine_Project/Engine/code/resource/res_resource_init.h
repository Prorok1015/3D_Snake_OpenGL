#pragma once
#include "../common/ds_store.hpp"

namespace components {

	void resource_init(ds::AppDataStorage& data);
	void resource_term(ds::AppDataStorage& data);
}

namespace com = components;