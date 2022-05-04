#pragma once
#include "Core/Core.h"
#include <vector>

namespace Engine
{
	class QS_API GuiWrapper
	{
	public:
		static void init();
		static void release();
		static void begin();
		static void end();
	};

}


