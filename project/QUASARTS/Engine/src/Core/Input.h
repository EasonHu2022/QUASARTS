#pragma once
#include "Core/Core.h"
#include "Event/KeyCodes.h"
#include "Event/EventModule.h"
#include "glm/glm.hpp"
#define MAX_KEYS	1024
#define MAX_BUTTONS	32

namespace Engine
{

	class QS_API Input
	{
	private :
		static Input* Instance;
		Input();
		~Input();
	public:
		
		static void init();
		/// <summary>
		/// return true if the key is held in current frame
		/// </summary>
		/// <param name="key">keycode</param>
		/// <returns></returns>
		static bool get_key(int key);

		/// <summary>
		/// return true if the key is pressed in current frame
		/// </summary>
		/// <param name="key">keycode</param>
		/// <returns></returns>
		static bool get_key_pressed(int key);

		/// <summary>
		/// return true if the key is released in current frame
		/// </summary>
		/// <param name="key">keycode</param>
		/// <returns></returns>
		static bool get_key_released(int key);

		/// <summary>
		/// Return true if the combination of keys is held in current frame, and all other keys are not held.
		/// </summary>
		/// <param name="key">keycode</param>
		/// <returns></returns>
		static bool get_key_combination(const std::vector<int>& keys);

		/// <summary>
		/// return true if the mouse is held
		/// </summary>
		/// <param name="mouse"></param>
		/// <returns></returns>
		static bool get_mouse(int mouse);

		/// <summary>
		/// return true if the mouse is pressed
		/// </summary>
		/// <param name="mouse"></param>
		/// <returns></returns>
		static bool get_mouse_clicked(int mouse);

		/// <summary>
		/// return true if the mouse is released
		/// </summary>
		/// <param name="mouse"></param>
		/// <returns></returns>
		static bool get_mouse_released(int mouse);

		static bool get_mouse_combination(const std::vector<int>& buttons);

		/// <summary>
		/// reset pressed and released state per frame
		/// </summary>
		static void reset_state();

		/*
			other mouse event (like move or scroll), register it in your own class via event module
		*/
		


	public:
		void EV_CALLBACK_SIGNATURE(KeyPressed);
		void EV_CALLBACK_SIGNATURE(KeyReleased);
		void EV_CALLBACK_SIGNATURE(MouseButtonPressed);
		void EV_CALLBACK_SIGNATURE(MouseButtonReleased);
		void EV_CALLBACK_SIGNATURE(MouseMoved);
		void EV_CALLBACK_SIGNATURE(Scrolled);

		static glm::vec2 mousePosition;

		static glm::vec2 mouseMotion;

		static bool mouseMoving;

	private:
		static bool keyPressed[MAX_KEYS];

		static bool keyReleased[MAX_KEYS];

		static bool keyHeld[MAX_KEYS];

		static int numKeysHeld;


		static bool mouseReleased[MAX_BUTTONS];

		static bool mouseHeld[MAX_BUTTONS];

		static bool mouseClicked[MAX_BUTTONS];

		static int numButtonsHeld;

		

	};
};


