#pragma once

/* Values from glfw3.h */

#define Q_MOUSE_BUTTON_UNKNOWN		-1

#define Q_MOUSE_BUTTON_1			0
#define Q_MOUSE_BUTTON_2			1					
#define Q_MOUSE_BUTTON_3			2
#define Q_MOUSE_BUTTON_4			3
#define Q_MOUSE_BUTTON_5			4
#define Q_MOUSE_BUTTON_6			5
#define Q_MOUSE_BUTTON_7			6
#define Q_MOUSE_BUTTON_8			7
#define Q_MOUSE_BUTTON_LAST			Q_MOUSE_BUTTON_8
#define Q_MOUSE_BUTTON_LEFT			Q_MOUSE_BUTTON_1
#define Q_MOUSE_BUTTON_RIGHT		Q_MOUSE_BUTTON_2
#define Q_MOUSE_BUTTON_MIDDLE		Q_MOUSE_BUTTON_3



// Converters //

#include "GLFW/glfw3.h"

static int mousecode_convert_glfw_to_q( const int glfwMousecode )
{
	int mouseCode = Q_MOUSE_BUTTON_UNKNOWN;

	switch( glfwMousecode )
	{
	case (GLFW_MOUSE_BUTTON_1):			mouseCode = Q_MOUSE_BUTTON_1;			break;
	case (GLFW_MOUSE_BUTTON_2):			mouseCode = Q_MOUSE_BUTTON_2;			break;
	case (GLFW_MOUSE_BUTTON_3):			mouseCode = Q_MOUSE_BUTTON_3;			break;
	case (GLFW_MOUSE_BUTTON_4):			mouseCode = Q_MOUSE_BUTTON_4;			break;
	case (GLFW_MOUSE_BUTTON_5):			mouseCode = Q_MOUSE_BUTTON_5;			break;
	case (GLFW_MOUSE_BUTTON_6):			mouseCode = Q_MOUSE_BUTTON_6;			break;
	case (GLFW_MOUSE_BUTTON_7):			mouseCode = Q_MOUSE_BUTTON_7;			break;
	case (GLFW_MOUSE_BUTTON_8):			mouseCode = Q_MOUSE_BUTTON_8;			break;
	}
	
	return mouseCode;
}