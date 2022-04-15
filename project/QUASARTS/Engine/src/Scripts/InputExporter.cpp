#include "InputExporter.h"
#include "KeyCode.h"
#include "Core/Input.h"

namespace Engine {

	void InputExporter::exportInput(sol::state& p_lua_state)
	{
		
		p_lua_state.new_usertype<Input>("Input",

			//use the same API with C++
			"get_key", &Input::get_key,
			"get_key_pressed", &Input::get_key_pressed,
			"get_key_released", &Input::get_key_released,
			"get_key_combination", &Input::get_key_combination,
			"get_mouse", &Input::get_mouse,
			"get_mouse_clicked", &Input::get_mouse_clicked,
			"get_mouse_released", &Input::get_mouse_released
			);

		//key names stay the same with C++ definition
		p_lua_state.new_enum<Key>("Key",
			{
				{"Q_KEY_UNKNOWN",		Key::KEY_UNKNOWN},
				{"Q_KEY_SPACE",			Key::KEY_SPACE},
				{"Q_KEY_APOSTROPHE",	Key::KEY_APOSTROPHE},
				{"Q_KEY_COMMA",			Key::KEY_COMMA},
				{"Q_KEY_MINUS",			Key::KEY_MINUS},
				{"Q_KEY_PERIOD",		Key::KEY_PERIOD},
				{"Q_KEY_SLASH",			Key::KEY_SLASH},
				{"Q_KEY_0",				Key::KEY_0},
				{"Q_KEY_1",				Key::KEY_1},
				{"Q_KEY_2",				Key::KEY_2},
				{"Q_KEY_3",				Key::KEY_3},
				{"Q_KEY_4",				Key::KEY_4},
				{"Q_KEY_5",				Key::KEY_5},
				{"Q_KEY_6",				Key::KEY_6},
				{"Q_KEY_7",				Key::KEY_7},
				{"Q_KEY_8",				Key::KEY_8},
				{"Q_KEY_9",				Key::KEY_9},
				{"Q_KEY_SEMICOLON",		Key::KEY_SEMICOLON},
				{"Q_KEY_EQUAL",			Key::KEY_EQUAL},
				{"Q_KEY_A",				Key::KEY_A},
				{"Q_KEY_B",				Key::KEY_B},
				{"Q_KEY_C",				Key::KEY_C},
				{"Q_KEY_D",				Key::KEY_D},
				{"Q_KEY_E",				Key::KEY_E},
				{"Q_KEY_F",				Key::KEY_F},
				{"Q_KEY_G",				Key::KEY_G},
				{"Q_KEY_H",				Key::KEY_H},
				{"Q_KEY_I",				Key::KEY_I},
				{"Q_KEY_J",				Key::KEY_J},
				{"Q_KEY_K",				Key::KEY_K},
				{"Q_KEY_L",				Key::KEY_L},
				{"Q_KEY_M",				Key::KEY_M},
				{"Q_KEY_N",				Key::KEY_N},
				{"Q_KEY_O",				Key::KEY_O},
				{"Q_KEY_P",				Key::KEY_P},
				{"Q_KEY_Q",				Key::KEY_Q},
				{"Q_KEY_R",				Key::KEY_R},
				{"Q_KEY_S",				Key::KEY_S},
				{"Q_KEY_T",				Key::KEY_T},
				{"Q_KEY_U",				Key::KEY_U},
				{"Q_KEY_V",				Key::KEY_V},
				{"Q_KEY_W",				Key::KEY_W},
				{"Q_KEY_X",				Key::KEY_X},
				{"Q_KEY_Y",				Key::KEY_Y},
				{"Q_KEY_Z",				Key::KEY_Z},
				{"Q_KEY_LEFT_BRACKET",	Key::KEY_LEFT_BRACKET},
				{"Q_KEY_BACKSLASH",		Key::KEY_BACKSLASH},
				{"Q_KEY_RIGHT_BRACKET",	Key::KEY_RIGHT_BRACKET},
				{"Q_KEY_GRAVE_ACCENT",	Key::KEY_GRAVE_ACCENT},
				{"Q_KEY_WORLD_1",		Key::KEY_WORLD_1},
				{"Q_KEY_WORLD_2",		Key::KEY_WORLD_2},
				{"Q_KEY_ESCAPE",		Key::KEY_ESCAPE},
				{"Q_KEY_ENTER",			Key::KEY_ENTER},
				{"Q_KEY_TAB",			Key::KEY_TAB},
				{"Q_KEY_BACKSPACE",		Key::KEY_BACKSPACE},
				{"Q_KEY_INSERT",		Key::KEY_INSERT},
				{"Q_KEY_DELETE",		Key::KEY_DELETE},
				{"Q_KEY_RIGHT",			Key::KEY_RIGHT},
				{"Q_KEY_LEFT",			Key::KEY_LEFT},
				{"Q_KEY_DOWN",			Key::KEY_DOWN},
				{"Q_KEY_UP",			Key::KEY_UP},
				{"Q_KEY_PAGE_UP",		Key::KEY_PAGE_UP},
				{"Q_KEY_PAGE_DOWN",		Key::KEY_PAGE_DOWN},
				{"Q_KEY_HOME",			Key::KEY_HOME},
				{"Q_KEY_END",			Key::KEY_END},
				{"Q_KEY_CAPS_LOCK",		Key::KEY_CAPS_LOCK},
				{"Q_KEY_SCROLL_LOCK",	Key::KEY_SCROLL_LOCK},
				{"Q_KEY_NUM_LOCK",		Key::KEY_NUM_LOCK},
				{"Q_KEY_PRINT_SCREEN",	Key::KEY_PRINT_SCREEN},
				{"Q_KEY_PAUSE",			Key::KEY_PAUSE},
				{"Q_KEY_F1",			Key::KEY_F1},
				{"Q_KEY_F2",			Key::KEY_F2},
				{"Q_KEY_F3",			Key::KEY_F3},
				{"Q_KEY_F4",			Key::KEY_F4},
				{"Q_KEY_F5",			Key::KEY_F5},
				{"Q_KEY_F6",			Key::KEY_F6},
				{"Q_KEY_F7",			Key::KEY_F7},
				{"Q_KEY_F8",			Key::KEY_F8},
				{"Q_KEY_F9",			Key::KEY_F9},
				{"Q_KEY_F10",			Key::KEY_F10},
				{"Q_KEY_F11",			Key::KEY_F11},
				{"Q_KEY_F12",			Key::KEY_F12},
				{"Q_KEY_F13",			Key::KEY_F13},
				{"Q_KEY_F14",			Key::KEY_F14},
				{"Q_KEY_F15",			Key::KEY_F15},
				{"Q_KEY_F16",			Key::KEY_F16},
				{"Q_KEY_F17",			Key::KEY_F17},
				{"Q_KEY_F18",			Key::KEY_F18},
				{"Q_KEY_F19",			Key::KEY_F19},
				{"Q_KEY_F20",			Key::KEY_F20},
				{"Q_KEY_F21",			Key::KEY_F21},
				{"Q_KEY_F22",			Key::KEY_F22},
				{"Q_KEY_F23",			Key::KEY_F23},
				{"Q_KEY_F24",			Key::KEY_F24},
				{"Q_KEY_F25",			Key::KEY_F25},
				{"Q_KEY_KP_0",			Key::KEY_KP_0},
				{"Q_KEY_KP_1",			Key::KEY_KP_1},
				{"Q_KEY_KP_2",			Key::KEY_KP_2},
				{"Q_KEY_KP_3",			Key::KEY_KP_3},
				{"Q_KEY_KP_4",			Key::KEY_KP_4},
				{"Q_KEY_KP_5",			Key::KEY_KP_5},
				{"Q_KEY_KP_6",			Key::KEY_KP_6},
				{"Q_KEY_KP_7",			Key::KEY_KP_7},
				{"Q_KEY_KP_8",			Key::KEY_KP_8},
				{"Q_KEY_KP_9",			Key::KEY_KP_9},
				{"Q_KEY_KP_DECIMAL",	Key::KEY_KP_DECIMAL},
				{"Q_KEY_KP_DIVIDE",		Key::KEY_KP_DIVIDE},
				{"Q_KEY_KP_MULTIPLY",	Key::KEY_KP_MULTIPLY},
				{"Q_KEY_KP_SUBTRACT",	Key::KEY_KP_SUBTRACT},
				{"Q_KEY_KP_ADD",		Key::KEY_KP_ADD},
				{"Q_KEY_KP_ENTER",		Key::KEY_KP_ENTER},
				{"Q_KEY_KP_EQUAL",		Key::KEY_KP_EQUAL},
				{"Q_KEY_LEFT_SHIFT",	Key::KEY_LEFT_SHIFT},
				{"Q_KEY_LEFT_CONTROL",	Key::KEY_LEFT_CONTROL},
				{"Q_KEY_LEFT_ALT",		Key::KEY_LEFT_ALT},
				{"Q_KEY_LEFT_SUPER",	Key::KEY_LEFT_SUPER},
				{"Q_KEY_RIGHT_SHIFT",	Key::KEY_RIGHT_SHIFT},
				{"Q_KEY_RIGHT_CONTROL",	Key::KEY_RIGHT_CONTROL},
				{"Q_KEY_RIGHT_ALT",		Key::KEY_RIGHT_ALT},
				{"Q_KEY_RIGHT_SUPER",	Key::KEY_RIGHT_SUPER},
				{"Q_KEY_MENU",			Key::KEY_MENU}
			});
	}
}