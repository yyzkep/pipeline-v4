//
// Created by drev on 15/08/2025.
//

#pragma once
//#include "Prediction.h"
#include "surface.hpp"
#include "pred.hpp"

struct kbutton_t;

enum GameActionSet_t
{
	GAME_ACTION_SET_NONE = -1,
	GAME_ACTION_SET_MENUCONTROLS = 0,
	GAME_ACTION_SET_FPSCONTROLS,
	GAME_ACTION_SET_IN_GAME_HUD,
	GAME_ACTION_SET_SPECTATOR,
};

enum GameActionSetFlags_t
{
	GAME_ACTION_SET_FLAGS_NONE = 0,
	GAME_ACTION_SET_FLAGS_TAUNTING = (1 << 0),
};

struct CameraThirdData_t
{
	float	Pitch;
	float	Yaw;
	float	Dist;
	float	Lag;
	vector_t	HullMin;
	vector_t	HullMax;
};

enum class ESteamControllerPadIInput; //shut up clang
class base_combat_weapon;
class i_input
{
public:
	int cam_is_third_person()
	{
		return utilities::find_vfunc<31, int>(this);
	}

	void cam_to_third_person()
	{
		utilities::find_vfunc<32, void>(this);
	}

	void cam_to_first_person()
	{
		utilities::find_vfunc<33, void>(this);
	}
public:
	typedef struct
	{
		unsigned int AxisFlags;
		unsigned int AxisMap;
		unsigned int ControlMap;
	} joy_axis_t;

	void		DescribeJoystickAxis(char const* axis, joy_axis_t* mapping);
	char const* DescribeAxis(int index);

	enum
	{
		GAME_AXIS_NONE = 0,
		GAME_AXIS_FORWARD,
		GAME_AXIS_PITCH,
		GAME_AXIS_SIDE,
		GAME_AXIS_YAW,
		MAX_GAME_AXES
	};

	enum
	{
		CAM_COMMAND_NONE = 0,
		CAM_COMMAND_TOTHIRDPERSON = 1,
		CAM_COMMAND_TOFIRSTPERSON = 2
	};

	enum
	{
		MOUSE_ACCEL_THRESHHOLD1 = 0,	// if mouse moves > this many mickey's double it
		MOUSE_ACCEL_THRESHHOLD2,		// if mouse moves > this many mickey's double it a second time
		MOUSE_SPEED_FACTOR,				// 0 = disabled, 1 = threshold 1 enabled, 2 = threshold 2 enabled

		NUM_MOUSE_PARAMS,
	};

	// Has the mouse been initialized?
	bool		MouseInitialized;
	// Is the mosue active?
	bool		MouseActive;
	// Has the joystick advanced initialization been run?
	bool		JoystickAdvancedInit;
	// Used to support hotplugging by reinitializing the advanced joystick system when we toggle between some/none joysticks.
	bool		HadJoysticks;

	// Accumulated mouse deltas
	float		AccumulatedMouseXMovement;
	float		AccumulatedMouseYMovement;
	float		PreviousMouseXPosition;
	float		PreviousMouseYPosition;
	float		RemainingJoystickSampleTime;
	float		KeyboardSampleTime;

	// Flag to restore systemparameters when exiting
	bool		RestoreSPI;
	// Original mouse parameters
	int			OrigMouseParms[NUM_MOUSE_PARAMS];
	// Current mouse parameters.
	int			NewMouseParms[NUM_MOUSE_PARAMS];
	bool		CheckMouseParam[NUM_MOUSE_PARAMS];
	// Are the parameters valid
	bool		MouseParmsValid;
	// Joystick Axis data
	joy_axis_t Axes[6];
	// List of queryable keys
	void* Keys;

	// Is the 3rd person camera using the mouse?
	bool		CameraInterceptingMouse;
	// Are we in 3rd person view?
	bool		CameraInThirdPerson;
	// Should we move view along with mouse?
	bool		CameraMovingWithMouse;


	// Is the camera in distance moving mode?
	bool		CameraDistanceMove;
	// Old and current mouse position readings.
	int			CameraOldX;
	int			CameraOldY;
	int			CameraX;
	int			CameraY;

	// orthographic camera settings
	bool		CameraIsOrthographic;

	q_angle_t		PreviousViewAngles;

	float		LastForwardMove;

	float PreviousJoystickForward;
	float PreviousJoystickSide;
	float PreviousJoystickPitch;
	float PreviousJoystickYaw;

	// Steam controller stuff
	int ControllerType[1 + 8];

	GameActionSet_t PreferredGameActionSet;
	GameActionSetFlags_t GameActionSetFlags;

	bool SteamControllerGameActionsInitialized;

	user_cmd* Commands;
	void* VerifiedCommands;

	CameraThirdData_t* CameraThirdData;

	inline user_cmd* get_cmds()
	{
		return *reinterpret_cast<user_cmd**>(reinterpret_cast<uintptr_t>(this) + 0xFC);
	}

};

enum AnalogCode_t
{
	ANALOG_CODE_INVALID = -1,
	MOUSE_X = 0,
	MOUSE_Y,
	MOUSE_XY, // invoked when either x or y changes
	MOUSE_WHEEL,
	ANALOG_CODE_LAST = 10,
};

enum ButtonCode_t
{
	BUTTON_CODE_INVALID = -1,
	BUTTON_CODE_NONE = 0,

	KEY_FIRST = 0,

	KEY_NONE = KEY_FIRST,
	KEY_0,
	KEY_1,
	KEY_2,
	KEY_3,
	KEY_4,
	KEY_5,
	KEY_6,
	KEY_7,
	KEY_8,
	KEY_9,
	KEY_A,
	KEY_B,
	KEY_C,
	KEY_D,
	KEY_E,
	KEY_F,
	KEY_G,
	KEY_H,
	KEY_I,
	KEY_J,
	KEY_K,
	KEY_L,
	KEY_M,
	KEY_N,
	KEY_O,
	KEY_P,
	KEY_Q,
	KEY_R,
	KEY_S,
	KEY_T,
	KEY_U,
	KEY_V,
	KEY_W,
	KEY_X,
	KEY_Y,
	KEY_Z,
	KEY_PAD_0,
	KEY_PAD_1,
	KEY_PAD_2,
	KEY_PAD_3,
	KEY_PAD_4,
	KEY_PAD_5,
	KEY_PAD_6,
	KEY_PAD_7,
	KEY_PAD_8,
	KEY_PAD_9,
	KEY_PAD_DIVIDE,
	KEY_PAD_MULTIPLY,
	KEY_PAD_MINUS,
	KEY_PAD_PLUS,
	KEY_PAD_ENTER,
	KEY_PAD_DECIMAL,
	KEY_LBRACKET,
	KEY_RBRACKET,
	KEY_SEMICOLON,
	KEY_APOSTROPHE,
	KEY_BACKQUOTE,
	KEY_COMMA,
	KEY_PERIOD,
	KEY_SLASH,
	KEY_BACKSLASH,
	KEY_MINUS,
	KEY_EQUAL,
	KEY_ENTER,
	KEY_SPACE,
	KEY_BACKSPACE,
	KEY_TAB,
	KEY_CAPSLOCK,
	KEY_NUMLOCK,
	KEY_ESCAPE,
	KEY_SCROLLLOCK,
	KEY_INSERT,
	KEY_DELETE,
	KEY_HOME,
	KEY_END,
	KEY_PAGEUP,
	KEY_PAGEDOWN,
	KEY_BREAK,
	KEY_LSHIFT,
	KEY_RSHIFT,
	KEY_LALT,
	KEY_RALT,
	KEY_LCONTROL,
	KEY_RCONTROL,
	KEY_LWIN,
	KEY_RWIN,
	KEY_APP,
	KEY_UP,
	KEY_LEFT,
	KEY_DOWN,
	KEY_RIGHT,
	KEY_F1,
	KEY_F2,
	KEY_F3,
	KEY_F4,
	KEY_F5,
	KEY_F6,
	KEY_F7,
	KEY_F8,
	KEY_F9,
	KEY_F10,
	KEY_F11,
	KEY_F12,
	KEY_CAPSLOCKTOGGLE,
	KEY_NUMLOCKTOGGLE,
	KEY_SCROLLLOCKTOGGLE,

	KEY_LAST = KEY_SCROLLLOCKTOGGLE,
	KEY_COUNT = KEY_LAST - KEY_FIRST + 1,

	// Mouse
	MOUSE_FIRST = KEY_LAST + 1,

	MOUSE_LEFT = MOUSE_FIRST,
	MOUSE_RIGHT,
	MOUSE_MIDDLE,
	MOUSE_4,
	MOUSE_5,
	MOUSE_WHEEL_UP,		// A fake button which is 'pressed' and 'released' when the wheel is moved up
	MOUSE_WHEEL_DOWN,	// A fake button which is 'pressed' and 'released' when the wheel is moved down

	MOUSE_LAST = MOUSE_WHEEL_DOWN,
	MOUSE_COUNT = MOUSE_LAST - MOUSE_FIRST + 1,
};

class i_input_system /*: public IAppSystem*/
{
public:
	bool is_button_down(int code)
	{
		return utilities::find_vfunc<11, bool, int>(this, code);
	}
	void reset_input_state()
	{
		utilities::find_vfunc<25, void>(this);
	}
};