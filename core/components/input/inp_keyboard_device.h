#pragma once
#include <common.h>
#include "inp_key_enums.hpp"

namespace inp
{
	class KeyboardDevice
	{
		struct Hasher {
			std::size_t operator () (KEYBOARD_BUTTONS key) const { return std::size_t(key); }
		};

		using KeyContainer = std::unordered_map<KEYBOARD_BUTTONS, Key, Hasher>;

	public:
		void on_key_action(int keycode, int scancode, int action, int mode);
		void on_key_keyboard_action(KEYBOARD_BUTTONS keycode, KEY_ACTION action, float time);

		Key get_key(KEYBOARD_BUTTONS keycode) const { return get_some_key(keycode, keys_); };
		Key get_prev_key(KEYBOARD_BUTTONS keycode) const { return get_some_key(keycode, prev_keys_); };

		template <typename CALLBACK>
		void visit_keys(CALLBACK&& callback) const { std::for_each(keys_.begin(), keys_.end(), [&callback](auto par) { callback(par.first, par.second); }); }

		void clear_key(KEYBOARD_BUTTONS keycode) { prev_keys_[keycode] = keys_[keycode]; keys_[keycode] = {}; }

	private:
		static Key get_some_key(KEYBOARD_BUTTONS keycode, const KeyContainer& keys);

	public:
		Event<void(KEYBOARD_BUTTONS, KEY_ACTION state)> onKeyStateChanged;

	private:
		KeyContainer keys_;
		KeyContainer prev_keys_;
	};
} 