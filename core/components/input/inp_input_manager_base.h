#pragma once

namespace inp
{
	class input_manager_base
	{
	public:
		virtual ~input_manager_base() {}

		virtual void on_notify_listeners(float dt) = 0;

	};
}