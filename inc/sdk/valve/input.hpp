//
// Created by drev on 15/08/2025.
//

#pragma once
//#include "Prediction.h"
#include "surface.hpp"
#include "pred.hpp"
#include <checksumcrc.hpp>

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

class verified_user_cmd
{
public:
	user_cmd m_cmd;
	CRC32_t m_crc;
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

enum class ESteamControllerPadIInput; //shut up clang
class base_combat_weapon;
class i_input
{
public:
	virtual void init_all() = 0;
	virtual void shutdown_all() = 0;
	virtual int get_button_bits(int button) = 0;
	virtual void create_move(int sequence_number, float input_sample_frametime, bool active) = 0;
	virtual void extra_mouse_sample(float frametime, bool active) = 0;
	virtual bool write_usercmd_delta_to_buffer(bf_write* buf, int from, int to, bool is_new_command) = 0;
	virtual void encode_user_cmd_to_buffer(bf_write& buf, int slot) = 0;
	virtual void decode_user_cmd_from_buffer(bf_read& buf, int slot) = 0;
	virtual user_cmd* get_user_cmd(int sequence_number) = 0;
	virtual void make_weapon_selection(void* weapon) = 0;
	virtual float key_state(kbutton_t* key) = 0;
	virtual int key_event(int event_code, ButtonCode_t keynum, const char* current_binding) = 0;
	virtual kbutton_t* find_key(const char* name) = 0;
	virtual void controller_commands() = 0;
	virtual void joystick_advanced() = 0;
	virtual void joystick_set_sample_time(float frametime) = 0;
	virtual void in_set_sample_time(float frametime) = 0;
	virtual void accumulate_mouse() = 0;
	virtual void activate_mouse() = 0;
	virtual void deactivate_mouse() = 0;
	virtual void clear_states() = 0;
	virtual float get_look_spring() = 0;
	virtual void get_fullscreen_mouse_pos(int* mx, int* my, int* unclamped_x = 0, int* unclamped_y = 0) = 0;
	virtual void set_fullscreen_mouse_pos(int mx, int my) = 0;
	virtual void reset_mouse() = 0;
	virtual float get_last_forward_move() = 0;
	virtual float joystick_get_forward() = 0;
	virtual float joystick_get_side() = 0;
	virtual float joystick_get_pitch() = 0;
	virtual float joystick_get_yaw() = 0;
	virtual void cam_think() = 0;
	virtual int cam_is_third_person() = 0;
	virtual void cam_to_third_person() = 0;
	virtual void cam_to_first_person() = 0;
	virtual void cam_start_mouse_move() = 0;
	virtual void cam_end_mouse_move() = 0;
	virtual void cam_start_distance() = 0;
	virtual void cam_end_distance() = 0;
	virtual int cam_intercepting_mouse() = 0;
	virtual void cam_to_orthographic() = 0;
	virtual bool cam_is_orthographic() const = 0;
	virtual void cam_orthographic_size(float& w, float& h) const = 0;
	virtual void set_preferred_game_action_set(GameActionSet_t action_set) = 0;
	virtual GameActionSet_t get_preferred_game_action_set() = 0;
	virtual void set_game_action_set_flags(GameActionSetFlags_t action_set_flags) = 0;
	virtual void level_init() = 0;
	virtual void clear_input_button(int bits) = 0;
	virtual void cam_set_camera_third_data(CameraThirdData_t* camera_data, const q_angle_t& camera_offset) = 0;
	virtual void cam_camera_third_think() = 0;
	virtual bool enable_joystick_mode() = 0;
	virtual bool is_steam_controller_active() = 0;

public:
	byte pad0[256];
	user_cmd* m_p_commands;
	verified_user_cmd* m_p_verified_commands;
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