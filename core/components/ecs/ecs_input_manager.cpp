#include "ecs_input_manager.h"
#include "ecs_common_system.h"
#include "inp_input_system.h"
#include "ecs_component.h"

struct event_visitor
{
	event_visitor(ecs::flow_input_manager& mng_)
		: mng(mng_)
	{}

	void operator() (const auto& evt) const noexcept
	{
		using T = std::decay_t<decltype(evt)>;
		auto ent = mng.get_empty_entity<T>();
		ecs::add_component(ent, evt);
		if (!ecs::get_component<ecs::input_changed_event_component>(ent)) {
			ecs::add_component(ent, ecs::input_changed_event_component{});
		}
	}

private:
	ecs::flow_input_manager& mng;
};

void ecs::flow_input_manager::on_notify_listeners(float dt)
{

}

void ecs::flow_input_manager::on_handle_event(const inp::input_event& evt)
{
	std::visit(event_visitor(*this), evt);
}
