#pragma once
#include "../common/ds_store.hpp"

namespace components {

	void game_init(ds::AppDataStorage& data);
	void game_term(ds::AppDataStorage& data);
}

namespace com = components;