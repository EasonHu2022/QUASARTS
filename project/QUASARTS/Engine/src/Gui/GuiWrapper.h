#pragma once
#include "Core/core.h"
#include <vector>
#include "Gui/Guiview.h"
namespace Engine
{
	class QS_API GuiWrapper
	{
	private:
		GuiWrapper();
		~GuiWrapper();
	public:
		static void init();
		static void release();
		static void begin();
		static void end();



	public:
		void on_gui();
		int add_gui_view(GuiView* view);
		void remove_gui_view(int handle);
	public:
		static GuiWrapper* Instance;

		std::vector<Engine::GuiView*> view_stack;
	};

}


