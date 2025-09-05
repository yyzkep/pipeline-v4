//
// Created by drev on 30/08/2025.
//

#pragma once
#include "client_entity.hpp"
#include "netvar_manager.hpp"
#include "typdefs.hpp"

class base_player;

enum thinkmethods_t
{
	THINK_FIRE_ALL_FUNCTIONS,
	THINK_FIRE_BASE_ONLY,
	THINK_FIRE_ALL_BUT_BASE,
};

class base_entity : public client_entity {
public:
	netvar(simulation_time, float, "CBaseEntity", "m_flSimulationTime");
	netvar(anim_time, float, "CBaseEntity", "m_flAnimTime");
	netvar(team_num, int, "CBaseEntity", "m_iTeamNum");
	netvar(mins, vector_t, "CBaseEntity", "m_vecMins");
	netvar(maxs, vector_t, "CBaseEntity", "m_vecMaxs");
	netvar(origin, vector_t, "CBaseEntity", "m_vecOrigin");
	netvar(critical, bool, "CTFWeaponBaseGrenadeProj", "m_bCritical");
	netvar(client_side_animation, bool, "CBaseAnimating", "m_bClientSideAnimation");
	netvar(thrower, int, "CBaseGrenade", "m_hThrower");
	netvar(owner, int, "CBaseGrenade", "m_hOwner");
	netvar(owner_entity, int, "CBaseEntity", "m_hOwnerEntity");
	netvar(initial_velocity, vector_t, "CTFWeaponBaseGrenadeProj", "m_vInitialVelocity");
	netvar(cycle, float, "CBaseAnimating", "m_flCycle");
	netvar(sequence, int, "CBaseAnimating", "m_nSequence");
	netvar(pose_parameter, float*, "CBaseAnimating", "m_flPoseParameter");
	netvar(playback_rate, float, "CBaseAnimating", "m_flPlaybackRate");
	netvar(next_attack, float, "CBaseCombatCharacter", "m_flNextAttack");
	netvar(effects, int, "CBaseEntity", "m_fEffects");
	netvar(flag_status, int, "CCaptureFlag", "m_nFlagStatus");
	netvar(reset_time, float, "CCaptureFlag", "m_flResetTime");
	netvar(type, int, "CTFGrenadePipebombProjectile", "m_iType");
	netvar(defensive_bomb, int, "CTFGrenadePipebombProjectile", "m_bDefensiveBomb");
	netvar(touched, int, "CTFGrenadePipebombProjectile", "m_bTouched");

	offset_from(old_simulation_time, float, "CBaseEntity", "m_flSimulationTime", sizeof(int));


	vector_t estimate_abs_velocity()
	{
		static auto Func = utilities::find_signature< void(__thiscall*)(base_entity*, vector_t&) >("client.dll", "48 89 5C 24 ? 57 48 83 EC ? 48 8B FA 48 8B D9 E8 ? ? ? ? 48 3B D8");
		vector_t v;
		Func(this, v);
		return v;
	}

	vector_t fucking_center()
	{
		vector_t temp = origin();
		temp.z += (mins().z + maxs().z) * 0.5f;
		return temp;
	}

	inline void set_abs_origin(const vector_t& origin)
	{
		static auto FN = utilities::find_signature< void(__thiscall*)(base_entity*, const vector_t&) >("client.dll", "48 89 5C 24 ? 57 48 83 EC ? 48 8B FA 48 8B D9 E8 ? ? ? ? F3 0F 10 83");
		return FN(this, origin);
	}

	inline void set_abs_angles(vector_t angles)
	{
		static auto FN = utilities::find_signature< void(__thiscall*)(base_entity*, const vector_t&) >("client.dll", "48 89 5C 24 ? 57 48 81 EC ? ? ? ? 48 8B FA 48 8B D9 E8 ? ? ? ? F3 0F 10 83");
		return FN(this, angles);
	}

	inline void invalidate_bone_cache()
	{
		static auto FN = utilities::find_signature< void(__thiscall*)(void*) >("client.dll", "8B 05 ? ? ? ? FF C8 C7 81");
		return FN(this);
	}

	static void set_prediction_random_seed(const user_cmd* cmd)
	{
		static auto siggy = utilities::find_signature<uintptr_t>("client.dll", "48 85 C9 75 ? C7 05 ? ? ? ? ? ? ? ? C3");
		reinterpret_cast<void(*)(const user_cmd*)>(siggy)(cmd);
	}

	static void set_prediction_player(base_player* player)
	{
		static auto siggy = utilities::find_signature<uintptr_t>("client.dll", "48 89 3D ? ? ? ? 66 0F 6E 87");
		* reinterpret_cast<base_player**>(*reinterpret_cast<DWORD*>(siggy + 0x03) + siggy + 0x07) = player;
	}

	inline bool physics_run_think(thinkmethods_t thinkMethod = THINK_FIRE_ALL_FUNCTIONS)
	{
		static auto siggy = utilities::find_signature<uintptr_t>("client.dll", "4C 8B DC 49 89 73 ? 57 48 81 EC ? ? ? ? 8B 81");
		return reinterpret_cast<bool(__thiscall*)(void*, thinkmethods_t)>(siggy)(this, thinkMethod);
	}

	CUtlVector<matrix3x4_t>* get_cached_bone_data()
	{
		return reinterpret_cast<CUtlVector<matrix3x4_t> *>(reinterpret_cast<usize>(this) + 0x848); //0x844?
	}

	base_entity* move_child();
	base_entity* next_move_peer();
	base_entity* move_parent();
	bool is_entity_on_screen();
};