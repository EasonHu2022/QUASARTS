#pragma once

/* Values from glfw3.h */

// The unknown key //
#define Q_KEY_UNKNOWN            -1

// Printable keys //			/* values from ASCII */
#define Q_KEY_SPACE				32
#define Q_KEY_APOSTROPHE		39  /* ' */
#define Q_KEY_COMMA				44  /* , */
#define Q_KEY_MINUS				45  /* - */
#define Q_KEY_PERIOD			46  /* . */
#define Q_KEY_SLASH				47  /* / */
#define Q_KEY_0					48
#define Q_KEY_1					49
#define Q_KEY_2					50
#define Q_KEY_3					51
#define Q_KEY_4					52
#define Q_KEY_5					53
#define Q_KEY_6					54
#define Q_KEY_7					55
#define Q_KEY_8					56
#define Q_KEY_9					57
#define Q_KEY_SEMICOLON			59  /* ; */
#define Q_KEY_EQUAL				61  /* = */
#define Q_KEY_A					65
#define Q_KEY_B					66
#define Q_KEY_C					67
#define Q_KEY_D					68
#define Q_KEY_E					69
#define Q_KEY_F					70
#define Q_KEY_G					71
#define Q_KEY_H					72
#define Q_KEY_I					73
#define Q_KEY_J					74
#define Q_KEY_K					75
#define Q_KEY_L					76
#define Q_KEY_M					77
#define Q_KEY_N					78
#define Q_KEY_O					79
#define Q_KEY_P					80
#define Q_KEY_Q					81
#define Q_KEY_R					82
#define Q_KEY_S					83
#define Q_KEY_T					84
#define Q_KEY_U					85
#define Q_KEY_V					86
#define Q_KEY_W					87
#define Q_KEY_X					88
#define Q_KEY_Y					89
#define Q_KEY_Z					90
#define Q_KEY_LEFT_BRACKET		91  /* [ */
#define Q_KEY_BACKSLASH			92  /* \ */
#define Q_KEY_RIGHT_BRACKET		93  /* ] */
#define Q_KEY_GRAVE_ACCENT		96  /* ` */
#define Q_KEY_WORLD_1			161 /* non-US #1 */
#define Q_KEY_WORLD_2			162 /* non-US #2 */

// Function keys //
#define Q_KEY_ESCAPE			256
#define Q_KEY_ENTER				257
#define Q_KEY_TAB				258
#define Q_KEY_BACKSPACE			259
#define Q_KEY_INSERT			260
#define Q_KEY_DELETE			261
#define Q_KEY_RIGHT				262
#define Q_KEY_LEFT				263
#define Q_KEY_DOWN				264
#define Q_KEY_UP				265
#define Q_KEY_PAGE_UP			266
#define Q_KEY_PAGE_DOWN			267
#define Q_KEY_HOME				268
#define Q_KEY_END				269
#define Q_KEY_CAPS_LOCK			280
#define Q_KEY_SCROLL_LOCK		281
#define Q_KEY_NUM_LOCK			282
#define Q_KEY_PRINT_SCREEN		283
#define Q_KEY_PAUSE				284
#define Q_KEY_F1				290
#define Q_KEY_F2				291
#define Q_KEY_F3				292
#define Q_KEY_F4				293
#define Q_KEY_F5				294
#define Q_KEY_F6				295
#define Q_KEY_F7				296
#define Q_KEY_F8				297
#define Q_KEY_F9				298
#define Q_KEY_F10				299
#define Q_KEY_F11				300
#define Q_KEY_F12				301
#define Q_KEY_F13				302
#define Q_KEY_F14				303
#define Q_KEY_F15				304
#define Q_KEY_F16				305
#define Q_KEY_F17				306
#define Q_KEY_F18				307
#define Q_KEY_F19				308
#define Q_KEY_F20				309
#define Q_KEY_F21				310
#define Q_KEY_F22				311
#define Q_KEY_F23				312
#define Q_KEY_F24				313
#define Q_KEY_F25				314
#define Q_KEY_KP_0				320
#define Q_KEY_KP_1				321
#define Q_KEY_KP_2				322
#define Q_KEY_KP_3				323
#define Q_KEY_KP_4				324
#define Q_KEY_KP_5				325
#define Q_KEY_KP_6				326
#define Q_KEY_KP_7				327
#define Q_KEY_KP_8				328
#define Q_KEY_KP_9				329
#define Q_KEY_KP_DECIMAL		330
#define Q_KEY_KP_DIVIDE			331
#define Q_KEY_KP_MULTIPLY		332
#define Q_KEY_KP_SUBTRACT		333
#define Q_KEY_KP_ADD			334
#define Q_KEY_KP_ENTER			335
#define Q_KEY_KP_EQUAL			336
#define Q_KEY_LEFT_SHIFT		340
#define Q_KEY_LEFT_CONTROL		341
#define Q_KEY_LEFT_ALT			342
#define Q_KEY_LEFT_SUPER		343
#define Q_KEY_RIGHT_SHIFT		344
#define Q_KEY_RIGHT_CONTROL		345
#define Q_KEY_RIGHT_ALT			346
#define Q_KEY_RIGHT_SUPER		347
#define Q_KEY_MENU				348


// Key mods //

#define Q_MOD_SHIFT				0x0001
#define Q_MOD_CONTROL			0x0002
#define Q_MOD_ALT				0x0004
#define Q_MOD_SUPER				0x0008
#define Q_MOD_CAPS_LOCK			0x0010
#define Q_MOD_NUM_LOCK			0x0020



// Converters //

#include "GLFW/glfw3.h"

static int keycode_convert_glfw_to_q( const int glfwKeycode )
{
	int q_keycode = Q_KEY_UNKNOWN;

	switch (glfwKeycode)
	{
	case (GLFW_KEY_UNKNOWN):			q_keycode =	Q_KEY_UNKNOWN;					break;
	case (GLFW_KEY_SPACE):				q_keycode =	Q_KEY_SPACE;					break;
	case (GLFW_KEY_APOSTROPHE):			q_keycode =	Q_KEY_APOSTROPHE;				break;
	case (GLFW_KEY_COMMA):				q_keycode = Q_KEY_COMMA;					break;
	case (GLFW_KEY_MINUS):				q_keycode = Q_KEY_MINUS;					break;
	case (GLFW_KEY_PERIOD):				q_keycode = Q_KEY_PERIOD;					break;
	case (GLFW_KEY_SLASH):				q_keycode = Q_KEY_SLASH;					break;
	case (GLFW_KEY_0):					q_keycode = Q_KEY_0;						break;
	case (GLFW_KEY_1):					q_keycode = Q_KEY_1;						break;
	case (GLFW_KEY_2):					q_keycode = Q_KEY_2;						break;
	case (GLFW_KEY_3):					q_keycode = Q_KEY_3;						break;
	case (GLFW_KEY_4):					q_keycode = Q_KEY_4;						break;
	case (GLFW_KEY_5):					q_keycode = Q_KEY_5;						break;
	case (GLFW_KEY_6):					q_keycode = Q_KEY_6;						break;
	case (GLFW_KEY_7):					q_keycode = Q_KEY_7;						break;
	case (GLFW_KEY_8):					q_keycode = Q_KEY_8;						break;
	case (GLFW_KEY_9):					q_keycode = Q_KEY_9;						break;
	case (GLFW_KEY_SEMICOLON):			q_keycode = Q_KEY_SEMICOLON;				break;
	case (GLFW_KEY_EQUAL):				q_keycode = Q_KEY_EQUAL;					break;
	case (GLFW_KEY_A):					q_keycode = Q_KEY_A;						break;
	case (GLFW_KEY_B):					q_keycode = Q_KEY_B;						break;
	case (GLFW_KEY_C):					q_keycode = Q_KEY_C;						break;
	case (GLFW_KEY_D):					q_keycode = Q_KEY_D;						break;
	case (GLFW_KEY_E):					q_keycode = Q_KEY_E;						break;
	case (GLFW_KEY_F):					q_keycode = Q_KEY_F;						break;
	case (GLFW_KEY_G):					q_keycode = Q_KEY_G;						break;
	case (GLFW_KEY_H):					q_keycode = Q_KEY_H;						break;
	case (GLFW_KEY_I):					q_keycode = Q_KEY_I;						break;
	case (GLFW_KEY_J):					q_keycode = Q_KEY_J;						break;
	case (GLFW_KEY_K):					q_keycode = Q_KEY_K;						break;
	case (GLFW_KEY_L):					q_keycode = Q_KEY_L;						break;
	case (GLFW_KEY_M):					q_keycode = Q_KEY_M;						break;
	case (GLFW_KEY_N):					q_keycode = Q_KEY_N;						break;
	case (GLFW_KEY_O):					q_keycode = Q_KEY_O;						break;
	case (GLFW_KEY_P):					q_keycode = Q_KEY_P;						break;
	case (GLFW_KEY_Q):					q_keycode = Q_KEY_Q;						break;
	case (GLFW_KEY_R):					q_keycode = Q_KEY_R;						break;
	case (GLFW_KEY_S):					q_keycode = Q_KEY_S;						break;
	case (GLFW_KEY_T):					q_keycode = Q_KEY_T;						break;
	case (GLFW_KEY_U):					q_keycode = Q_KEY_U;						break;
	case (GLFW_KEY_V):					q_keycode = Q_KEY_V;						break;
	case (GLFW_KEY_W):					q_keycode = Q_KEY_W;						break;
	case (GLFW_KEY_X):					q_keycode = Q_KEY_X;						break;
	case (GLFW_KEY_Y):					q_keycode = Q_KEY_Y;						break;
	case (GLFW_KEY_Z):					q_keycode = Q_KEY_Z;						break;
	case (GLFW_KEY_LEFT_BRACKET):		q_keycode = Q_KEY_LEFT_BRACKET;				break;
	case (GLFW_KEY_BACKSLASH):			q_keycode = Q_KEY_BACKSLASH;				break;
	case (GLFW_KEY_RIGHT_BRACKET):		q_keycode = Q_KEY_RIGHT_BRACKET;			break;
	case (GLFW_KEY_GRAVE_ACCENT):		q_keycode = Q_KEY_GRAVE_ACCENT;				break;
	case (GLFW_KEY_WORLD_1):			q_keycode = Q_KEY_WORLD_1;					break;
	case (GLFW_KEY_WORLD_2):			q_keycode = Q_KEY_WORLD_2;					break;
	case (GLFW_KEY_ESCAPE):				q_keycode = Q_KEY_ESCAPE;					break;
	case (GLFW_KEY_ENTER):				q_keycode = Q_KEY_ENTER;					break;
	case (GLFW_KEY_TAB):				q_keycode = Q_KEY_TAB;						break;
	case (GLFW_KEY_BACKSPACE):			q_keycode = Q_KEY_BACKSPACE;				break;
	case (GLFW_KEY_INSERT):				q_keycode = Q_KEY_INSERT;					break;
	case (GLFW_KEY_DELETE):				q_keycode = Q_KEY_DELETE;					break;
	case (GLFW_KEY_RIGHT):				q_keycode = Q_KEY_RIGHT;					break;
	case (GLFW_KEY_LEFT):				q_keycode = Q_KEY_LEFT;						break;
	case (GLFW_KEY_DOWN):				q_keycode = Q_KEY_DOWN;						break;
	case (GLFW_KEY_UP):					q_keycode = Q_KEY_UP;						break;
	case (GLFW_KEY_PAGE_UP):			q_keycode = Q_KEY_PAGE_UP;					break;
	case (GLFW_KEY_PAGE_DOWN):			q_keycode = Q_KEY_PAGE_DOWN;				break;
	case (GLFW_KEY_HOME):				q_keycode = Q_KEY_HOME;						break;
	case (GLFW_KEY_END):				q_keycode = Q_KEY_END;						break;
	case (GLFW_KEY_CAPS_LOCK):			q_keycode = Q_KEY_CAPS_LOCK;				break;
	case (GLFW_KEY_SCROLL_LOCK):		q_keycode = Q_KEY_SCROLL_LOCK;				break;
	case (GLFW_KEY_NUM_LOCK):			q_keycode = Q_KEY_NUM_LOCK;					break;
	case (GLFW_KEY_PRINT_SCREEN):		q_keycode = Q_KEY_PRINT_SCREEN;				break;
	case (GLFW_KEY_PAUSE):				q_keycode = Q_KEY_PAUSE;					break;
	case (GLFW_KEY_F1):					q_keycode = Q_KEY_F1;						break;
	case (GLFW_KEY_F2):					q_keycode = Q_KEY_F2;						break;
	case (GLFW_KEY_F3):					q_keycode = Q_KEY_F3;						break;
	case (GLFW_KEY_F4):					q_keycode = Q_KEY_F4;						break;
	case (GLFW_KEY_F5):					q_keycode = Q_KEY_F5;						break;
	case (GLFW_KEY_F6):					q_keycode = Q_KEY_F6;						break;
	case (GLFW_KEY_F7):					q_keycode = Q_KEY_F7;						break;
	case (GLFW_KEY_F8):					q_keycode = Q_KEY_F8;						break;
	case (GLFW_KEY_F9):					q_keycode = Q_KEY_F9;						break;
	case (GLFW_KEY_F10):				q_keycode = Q_KEY_F10;						break;
	case (GLFW_KEY_F11):				q_keycode = Q_KEY_F11;						break;
	case (GLFW_KEY_F12):				q_keycode = Q_KEY_F12;						break;
	case (GLFW_KEY_F13):				q_keycode = Q_KEY_F13;						break;
	case (GLFW_KEY_F14):				q_keycode = Q_KEY_F14;						break;
	case (GLFW_KEY_F15):				q_keycode = Q_KEY_F15;						break;
	case (GLFW_KEY_F16):				q_keycode = Q_KEY_F16;						break;
	case (GLFW_KEY_F17):				q_keycode = Q_KEY_F17;						break;
	case (GLFW_KEY_F18):				q_keycode = Q_KEY_F18;						break;
	case (GLFW_KEY_F19):				q_keycode = Q_KEY_F19;						break;
	case (GLFW_KEY_F20):				q_keycode = Q_KEY_F20;						break;
	case (GLFW_KEY_F21):				q_keycode = Q_KEY_F21;						break;
	case (GLFW_KEY_F22):				q_keycode = Q_KEY_F22;						break;
	case (GLFW_KEY_F23):				q_keycode = Q_KEY_F23;						break;
	case (GLFW_KEY_F24):				q_keycode = Q_KEY_F24;						break;
	case (GLFW_KEY_F25):				q_keycode = Q_KEY_F25;						break;
	case (GLFW_KEY_KP_0):				q_keycode = Q_KEY_KP_0;						break;
	case (GLFW_KEY_KP_1):				q_keycode = Q_KEY_KP_1;						break;
	case (GLFW_KEY_KP_2):				q_keycode = Q_KEY_KP_2;						break;
	case (GLFW_KEY_KP_3):				q_keycode = Q_KEY_KP_3;						break;
	case (GLFW_KEY_KP_4):				q_keycode = Q_KEY_KP_4;						break;
	case (GLFW_KEY_KP_5):				q_keycode = Q_KEY_KP_5;						break;
	case (GLFW_KEY_KP_6):				q_keycode = Q_KEY_KP_6;						break;
	case (GLFW_KEY_KP_7):				q_keycode = Q_KEY_KP_7;						break;
	case (GLFW_KEY_KP_8):				q_keycode = Q_KEY_KP_8;						break;
	case (GLFW_KEY_KP_9):				q_keycode = Q_KEY_KP_9;						break;
	case (GLFW_KEY_KP_DECIMAL):			q_keycode = Q_KEY_KP_DECIMAL;				break;
	case (GLFW_KEY_KP_DIVIDE):			q_keycode = Q_KEY_KP_DIVIDE;				break;
	case (GLFW_KEY_KP_MULTIPLY):		q_keycode = Q_KEY_KP_MULTIPLY;				break;
	case (GLFW_KEY_KP_SUBTRACT):		q_keycode = Q_KEY_KP_SUBTRACT;				break;
	case (GLFW_KEY_KP_ADD):				q_keycode = Q_KEY_KP_ADD;					break;
	case (GLFW_KEY_KP_ENTER):			q_keycode = Q_KEY_KP_ENTER;					break;
	case (GLFW_KEY_KP_EQUAL):			q_keycode = Q_KEY_KP_EQUAL;					break;
	case (GLFW_KEY_LEFT_SHIFT):			q_keycode = Q_KEY_LEFT_SHIFT;				break;
	case (GLFW_KEY_LEFT_CONTROL):		q_keycode = Q_KEY_LEFT_CONTROL;				break;
	case (GLFW_KEY_LEFT_ALT):			q_keycode = Q_KEY_LEFT_ALT;					break;
	case (GLFW_KEY_LEFT_SUPER):			q_keycode = Q_KEY_LEFT_SUPER;				break;
	case (GLFW_KEY_RIGHT_SHIFT):		q_keycode = Q_KEY_RIGHT_SHIFT;				break;
	case (GLFW_KEY_RIGHT_CONTROL):		q_keycode = Q_KEY_RIGHT_CONTROL;			break;
	case (GLFW_KEY_RIGHT_ALT):			q_keycode = Q_KEY_RIGHT_ALT;				break;
	case (GLFW_KEY_RIGHT_SUPER):		q_keycode = Q_KEY_RIGHT_SUPER;				break;
	case (GLFW_KEY_MENU):				q_keycode = Q_KEY_MENU;						break;
	}

	return q_keycode;
}


static int keymods_convert_glfw_to_q( const int glfwMods )
{
	int q_mods = 0;

	if (glfwMods & GLFW_MOD_SHIFT)			q_mods = q_mods | Q_MOD_SHIFT;
	if (glfwMods & GLFW_MOD_CONTROL)		q_mods = q_mods | Q_MOD_CONTROL;
	if (glfwMods & GLFW_MOD_ALT)			q_mods = q_mods | Q_MOD_ALT;
	if (glfwMods & GLFW_MOD_SUPER)			q_mods = q_mods | Q_MOD_SUPER;
	if (glfwMods & GLFW_MOD_CAPS_LOCK)		q_mods = q_mods | Q_MOD_CAPS_LOCK;
	if (glfwMods & GLFW_MOD_NUM_LOCK)		q_mods = q_mods | Q_MOD_NUM_LOCK;

	return q_mods;
}