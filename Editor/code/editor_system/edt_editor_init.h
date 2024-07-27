#pragma once
#include <common/ds_store.hpp>

namespace components {

	void editor_init(ds::AppDataStorage& data);
	void editor_term(ds::AppDataStorage& data);
}

namespace com = components;