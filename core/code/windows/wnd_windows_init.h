#pragma once
#include "../common/ds_store.hpp"

namespace components {

	void windows_init(ds::AppDataStorage& data);
	void windows_term(ds::AppDataStorage& data);
}

namespace com = components;