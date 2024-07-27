#pragma once
#include "../common/ds_store.hpp"

namespace components {

	void debug_ui_init(ds::AppDataStorage& data);

	void debug_ui_term(ds::AppDataStorage& data);
}

namespace com = components;