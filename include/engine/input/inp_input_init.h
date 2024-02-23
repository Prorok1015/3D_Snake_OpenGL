#pragma once
#include "../common/ds_store.hpp"

namespace components {

	void input_init(ds::AppDataStorage& data);
	void input_term(ds::AppDataStorage& data);
}

namespace com = components;