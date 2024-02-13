#include "components_init.h"
#include "common/common.h"
#include "application.h"
#include "input/inp_input_init.h"

void components::component_init(ds::DataStorage& data)
{
	data.construct<app::Application>();
	input_init(data);
}
