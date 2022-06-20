#pragma once
#include "Input.h"

namespace Engine
{
	Input* Input::Instance = NULL;
	bool Input::keyPressed[MAX_KEYS] = {};
	bool Input::keyReleased[MAX_KEYS] = {};
	bool Input::keyHeld[MAX_KEYS] = {};
	int Input::numKeysHeld = 0;
	bool Input::mouseReleased[MAX_BUTTONS] = {};
	bool Input::mouseHeld[MAX_BUTTONS] = {};
	bool Input::mouseClicked[MAX_BUTTONS] = {};
	int Input::numButtonsHeld = 0;
	glm::vec2 Input::mousePosition = { -1.0f,-1.0f };
	glm::vec2 Input::mouseMotion = { 0.0f,0.0f };
	bool Input::mouseMoving = false;

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
		EventModule::Instance()->register_handler(EV_CALLBACK_REGISTRATION(MouseMoved));
		EventModule::Instance()->register_handler(EV_CALLBACK_REGISTRATION(Scrolled));
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
	bool Input::get_key_combination(const std::vector<int>& keys)
	{
		int numKeys = keys.size();
		if (numKeysHeld == 0 || numKeysHeld != numKeys) return false;
		for (int i = 0; i < numKeys; ++i)
		{
			if (keyHeld[ keys[i] ] == false) return false;
		}
		return true;
	}
	bool Input::get_mouse(int mouse)
	{
		return mouseHeld[mouse];
	}
	bool Input::get_mouse_clicked(int mouse)
	{
		return mouseClicked[mouse];
	}
	bool Input::get_mouse_released(int mouse)
	{
		return mouseReleased[mouse];
	}
	bool Input::get_mouse_combination(const std::vector<int>& buttons)
	{
		int numButtons = buttons.size();
		if (numButtonsHeld == 0 || numButtonsHeld != numButtons) return false;
		for (int i = 0; i < numButtons; ++i)
		{
			if (mouseHeld[buttons[i]] == false) return false;
		}
		return true;
	}
	void Input::reset_state()
	{
		//memset(keyHeld, 0, MAX_KEYS);
		memset(keyPressed, 0, MAX_KEYS);
		memset(keyReleased, 0, MAX_KEYS);
		//memset(mouseHeld, 0, MAX_BUTTONS);
		memset(mouseClicked, 0, MAX_BUTTONS);
		memset(mouseReleased,0, MAX_BUTTONS);
		mouseMoving = false;
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
			

		if (!keyHeld[keycode])
		{
			keyPressed[keycode] = true;
			keyHeld[keycode] = true;
			++numKeysHeld;
			//QDEBUG("key pressed");
		}
		
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
		--numKeysHeld;
		//QDEBUG("key released");
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
		if (!mouseHeld[button])
		{
			mouseHeld[button] = true;
			++numButtonsHeld;
		}
		//QDEBUG("mouse pressed");
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
		--numButtonsHeld;
		//QDEBUG("mouse released");
	}

	void Input::EV_CALLBACK_SIGNATURE(MouseMoved)
	{
		float x = -1;
		std::string name = "xPos";
		auto ret = evt.find_argument(&x, name);
		if (!ret)
		{
			QDEBUG("No args named {0} in event 'MouseMoved', please check !", name);
			return;
		}

		float y = -1;
		name = "yPos";
		ret = evt.find_argument(&y, name);
		if (!ret)
		{
			QDEBUG("No args named {0} in event 'MouseMoved', please check !", name);
			return;
		}

		glm::vec2 newPos = {x,y};
		if (mousePosition.x >=0 && mousePosition.y >= 0)
			mouseMotion = newPos - mousePosition;
		mousePosition = newPos;
		mouseMoving = true;
		//QDEBUG("mouse moving");
	}

	void Input::EV_CALLBACK_SIGNATURE(Scrolled)
	{
		/*int button = -1;
		std::string name = "button";
		auto ret = evt.find_argument(&button, name);
		if (!ret)
		{
			QDEBUG("No args named {0} in event 'Scrolled', please check !", name);
			return;
		}

		mouseReleased[button] = true;
		mouseHeld[button] = false;
		--numButtonsHeld;*/
	}


};