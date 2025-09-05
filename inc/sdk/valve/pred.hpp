//
// Created by drev on 15/08/2025.
//

#pragma once
#include "engine_trace.hpp"
#include "c_handle.hpp"
#include "c_handle.hpp"
#include "qangle.hpp"
#include "vector.hpp"

class user_cmd
{
public:
	virtual ~user_cmd() {}

	void reset()
	{
		command_number = 0;
		tick_count = 0;
		viewangles = { 0.0f, 0.0f, 0.0f };
		forwardmove = 0.0f;
		sidemove = 0.0f;
		upmove = 0.0f;
		buttons = 0;
		impulse = 0;
		weaponselect = 0;
		weaponsubtype = 0;
		random_seed = 0;
		mousedx = 0;
		mousedy = 0;
		hasbeenpredicted = false;
	}

	user_cmd& operator =(const user_cmd& src)
	{
		if (this == &src)
			return *this;

		command_number = src.command_number;
		tick_count = src.tick_count;
		viewangles = src.viewangles;
		forwardmove = src.forwardmove;
		sidemove = src.sidemove;
		upmove = src.upmove;
		buttons = src.buttons;
		impulse = src.impulse;
		weaponselect = src.weaponselect;
		weaponsubtype = src.weaponsubtype;
		random_seed = src.random_seed;
		mousedx = src.mousedx;
		mousedy = src.mousedy;
		hasbeenpredicted = src.hasbeenpredicted;

		return *this;
	}

	void make_inert(void)
	{
		viewangles = { 0.0f, 0.0f, 0.0f };
		forwardmove = 0.f;
		sidemove = 0.f;
		upmove = 0.f;
		buttons = 0;
		impulse = 0;
	}

	int	command_number;
	int	tick_count;
	vector_t viewangles;
	float forwardmove;
	float sidemove;
	float upmove;
	int	buttons;
	unsigned char impulse;
	int	weaponselect;
	int	weaponsubtype;
	int	random_seed;
	short mousedx;
	short mousedy;
	bool hasbeenpredicted;
};

class IPhysicsSurfaceProps;
using EntityHandle_t = base_handle;
class base_entity;
class game_trace;
enum class PLAYER_ANIM;
class i_move_helper
{
public:
	virtual	char const* get_name(EntityHandle_t handle) const = 0;
	virtual void reset_touch_list(void) = 0;
	virtual bool add_to_touched(const game_trace& tr, const vector_t& impactvelocity) = 0;
	virtual void process_impacts(void) = 0;
	virtual void con_nprintf(int idx, char const* fmt, ...) = 0;
	virtual void start_sound(const vector_t& origin, int channel, char const* sample, float volume, soundlevel_t soundlevel, int fFlags, int pitch) = 0;
	virtual void start_sound(const vector_t& origin, const char* soundname) = 0;
	virtual void playback_event_full(int flags, int clientindex, unsigned short eventindex, float delay, vector_t& origin, vector_t& angles, float fparam1, float fparam2, int iparam1, int iparam2, int bparam1, int bparam2) = 0;
	virtual bool player_falling_damage(void) = 0;
	virtual void player_set_animation(PLAYER_ANIM playerAnim) = 0;
	virtual IPhysicsSurfaceProps* get_surface_props(void) = 0;
	virtual bool is_world_entity(const base_handle& handle) = 0;
	virtual void set_host(base_entity* host) = 0;

protected:
	virtual ~i_move_helper() {}
};

class c_move_data
{
public:
	bool m_bFirstRunOfFunctions : 1;
	bool m_bGameCodeMovedPlayer : 1;
	EntityHandle_t m_nPlayerHandle;
	int m_nImpulseCommand;
	q_angle_t m_vecViewAngles;
	q_angle_t m_vecAbsViewAngles;
	int m_nButtons;
	int m_nOldButtons;
	float m_flForwardMove;
	float m_flOldForwardMove;
	float m_flSideMove;
	float m_flUpMove;
	float m_flMaxSpeed;
	float m_flClientMaxSpeed;
	vector_t m_vecVelocity;
	q_angle_t m_vecAngles;
	q_angle_t m_vecOldAngles;
	float m_outStepHeight;
	vector_t m_outWishVel;
	vector_t m_outJumpVel;
	vector_t m_vecConstraintCenter;
	float m_flConstraintRadius;
	float m_flConstraintWidth;
	float m_flConstraintSpeedFactor;
	vector_t m_vecAbsOrigin;
};


class i_pred
{
public:
	virtual ~i_pred(void) {};
	virtual void init(void) = 0;
	virtual void shutdown(void) = 0;
	virtual void update(int startframe, bool validframe, int incoming_acknowledged, int outgoing_command) = 0;
	virtual void pre_entity_packet_received(int commands_acknowledged, int current_world_update_packet) = 0;
	virtual void post_entity_packet_received(void) = 0;
	virtual void post_network_data_received(int commands_acknowledged) = 0;
	virtual void on_received_uncompressed_packet(void) = 0;
	virtual void get_view_origin(vector_t& org) = 0;
	virtual void set_view_origin(vector_t& org) = 0;
	virtual void get_view_angles(q_angle_t& ang) = 0;
	virtual void set_view_angles(q_angle_t& ang) = 0;
	virtual void get_local_view_angles(q_angle_t& ang) = 0;
	virtual void set_local_view_angles(q_angle_t& ang) = 0;
};

class c_pred : public i_pred
{
public:
	virtual ~c_pred() {};
	virtual void init() = 0;
	virtual void shutdown() = 0;
	virtual void update(int startframe, bool validframe, int incoming_acknowledged, int outgoing_command) = 0;
	virtual void on_received_uncompressed_packet() = 0;
	virtual void pre_entity_packet_received(int commands_acknowledged, int current_world_update_packet) = 0;
	virtual void post_entity_packet_received() = 0;
	virtual void post_network_data_received(int commands_acknowledged) = 0;
	virtual bool in_prediction() = 0;
	virtual bool is_first_time_predicted() = 0;
	virtual int	get_incoming_packet_number() = 0;
	virtual void get_view_origin(vector_t& org) = 0;
	virtual void set_view_origin(vector_t& org) = 0;
	virtual void get_view_angles(q_angle_t& ang) = 0;
	virtual void set_view_angles(q_angle_t& ang) = 0;
	virtual void get_local_view_angles(q_angle_t& ang) = 0;
	virtual void set_local_view_angles(q_angle_t& ang) = 0;
	virtual void run_command(base_entity* player, user_cmd* ucmd, i_move_helper* moveHelper) = 0;
	virtual void setup_move(base_entity* player, user_cmd* ucmd, i_move_helper* pHelper, c_move_data* move) = 0;
	virtual void finish_move(base_entity* player, user_cmd* ucmd, c_move_data* move) = 0;
	virtual void set_ideal_pitch(base_entity* player, const vector_t& origin, const vector_t& angles, const vector_t& viewheight) = 0;
	virtual void _update(bool received_new_world_update, bool validframe, int incoming_acknowledged, int outgoing_command) = 0;

	c_handle<base_entity> m_hLastGround;
	bool m_bInPrediction;
	bool m_bFirstTimePredicted;
	bool m_bOldCLPredictValue;
	bool m_bEnginePaused;
	int m_nPreviousStartFrame;
	int m_nCommandsPredicted;
	int m_nServerCommandsAcknowledged;
	int m_bPreviousAckHadErrors;
	int m_nIncomingPacketNumber;
	float m_flIdealPitch;
};