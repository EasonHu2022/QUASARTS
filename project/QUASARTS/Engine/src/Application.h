#pragma once
#include "core.h"
#include <vector>
#include "RunTime.h"
#include "Window.h"
namespace Engine
{

	class QS_API Application
	{
	private:
		std::string name = "Quarsart Editor V1.0";
		Window* m_window = NULL;

	public:
		Window* get_window() { return m_window; }
		Application();
		virtual ~Application();
		void init();
		void start();
		void update();
		void stop();
		std::vector<RunTime*> runtimes;
		//generally it's ordered;
		void push_runtime(RunTime* runtime);
		void pop_runtime();
		//to be defined in the inst of application



	};
	
}


