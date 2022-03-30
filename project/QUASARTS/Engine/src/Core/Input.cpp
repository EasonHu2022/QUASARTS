#pragma once
#include "Input.h"

namespace Engine
{
	Input* Input::Instance = NULL;
	bool Input::keyPressed[MAX_KEYS] = {};
	bool Input::keyReleased[MAX_KEYS] = {};
	bool Input::keyHeld[MAX_KEYS] = {};
	bool Input::mouseReleased[MAX_BUTTONS] = {};
	bool Input::mouseHeld[MAX_BUTTONS] = {};
	bool Input::mouseClicked[MAX_BUTTONS] = {};

	void Input::init()
	{
		if (Input::Instance == NULL)
		{
			Input::Instance = new Input();
		}
	}


	Input::Input()
	{
		EventModule::Instance()->register_handler(EV_CALLBACK_REGISTRATION(KeyPressed));
		EventModule::Instance()->register_handler(EV_CALLBACK_REGISTRATION(KeyReleased));
		EventModule::Instance()->register_handler(EV_CALLBACK_REGISTRATION(MouseButtonPressed));
		EventModule::Instance()->register_handler(EV_CALLBACK_REGISTRATION(MouseButtonReleased));
	}

	Input::~Input()
	{
		
	}


	bool Input::get_key(int key)
	{
		return keyHeld[key];
	}
	bool Input::get_key_pressed(int key)
	{
		return keyPressed[key];
	}
	bool Input::get_key_released(int key)
	{
		return keyReleased[key];
	}
	bool Input::get_mouse(int mouse)
	{
		return false;
	}
	bool Input::get_mouse_clicked(int mouse)
	{
		return false;
	}
	bool Input::get_mouse_released(int mouse)
	{
		return false;
	}
	void Input::reset_state()
	{
		//memset(keyHeld, 0, MAX_KEYS);
		memset(keyPressed, 0, MAX_KEYS);
		memset(keyReleased, 0, MAX_KEYS);
		//memset(mouseHeld, 0, MAX_BUTTONS);
		memset(mouseClicked, 0, MAX_BUTTONS);
		memset(mouseReleased,0, MAX_BUTTONS);
	}
	void Input::EV_CALLBACK_SIGNATURE(KeyPressed)
	{
		int keycode = -1;
		std::string name = "key";
		auto ret = evt.find_argument(&keycode, name);
		if (!ret)
		{
			QDEBUG("No args named {0} in event 'Keypressed',please check !", name);
			return;
		}			
		
		keyPressed[keycode] = true;
		keyHeld[keycode] = true;
	}
	void Input::EV_CALLBACK_SIGNATURE(KeyReleased)
	{
		int keycode = -1;
		std::string name = "key";
		auto ret = evt.find_argument(&keycode, name);
		if (!ret)
		{
			QDEBUG("No args named {0} in event 'KeyReleased',please check !", name);
			return;
		}

		keyReleased[keycode] = true;
		keyHeld[keycode] = false;
	}
	void Input::EV_CALLBACK_SIGNATURE(MouseButtonPressed)
	{
		int button = -1;
		std::string name = "button";
		auto ret = evt.find_argument(&button, name);
		if (!ret)
		{
			QDEBUG("No args named {0} in event 'MouseButtonPressed', please check !", name);
			return;
		}

		mouseClicked[button] = true;
		mouseHeld[button] = true;
	}
	void Input::EV_CALLBACK_SIGNATURE(MouseButtonReleased)
	{
		int button = -1;
		std::string name = "button";
		auto ret = evt.find_argument(&button, name);
		if (!ret)
		{
			QDEBUG("No args named {0} in event 'MouseButtonReleased', please check !", name);
			return;
		}

		mouseReleased[button] = true;
		mouseHeld[button] = false;
	}
};


