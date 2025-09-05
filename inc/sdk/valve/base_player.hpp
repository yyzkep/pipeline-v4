//
// Created by drev on 30/08/2025.
//

#pragma once
#include "base_entity.hpp"
#include "math.hpp"
#include "pred.hpp"

class base_combat_weapon;
using MyWeapons = std::array<c_handle<base_combat_weapon>, 48>;

class base_player : public base_entity {
public:
	netvar(life_state, uint8_t, "CBasePlayer", "m_lifeState");
	netvar(health, int, "CBasePlayer", "m_iHealth");
	netvar(view_offset, vector_t, "CBasePlayer", "m_vecViewOffset[0]");
	netvar(ducked, bool, "CBasePlayer", "m_bDucked");
	netvar(model_scale, float, "CBaseAnimating", "m_flModelScale");
	netvar(fall_velocity, float, "CBasePlayer", "m_flFallVelocity");
	netvar(ducking, bool, "CBasePlayer", "m_bDucking");
	netvar(velocity, vector_t, "CBasePlayer", "m_vecVelocity[0]");
	netvar(base_velocity, vector_t, "CBasePlayer", "m_vecBaseVelocity[0]");
	netvar(ground_entity, int, "CBasePlayer", "m_hGroundEntity");
	netvar(flags, int, "CBasePlayer", "m_fFlags");
	netvar(duck_jump, bool, "CBasePlayer", "m_bInDuckJump");
	netvar(ducktime, float, "CBasePlayer", "m_flDucktime");
	netvar(duck_jump_time, float, "CBasePlayer", "m_flDuckJumpTime");
	netvar(active_weapon, c_handle<base_combat_weapon>, "CBaseCombatCharacter", "m_hActiveWeapon");
	netvar(punch_angle, vector_t, "CBasePlayer", "m_vecPunchAngle");
	netvar(punch_angle_vel, vector_t, "CBasePlayer", "m_vecPunchAngleVel");
	netvar(hitbox_set, int, "CBaseAnimating", "m_nHitboxSet");
	netvar(next_attack, float, "CBaseCombatCharacter", "m_flNextAttack");
	netvar(tick_base, int, "CBasePlayer", "m_nTickBase");
	netvar(damage_done, int, "CTFPlayer", "m_iDamageDone");
	netvar(crits, int, "CTFPlayer", "m_iCrits");
	netvar(crit_mult, int, "CTFPlayer", "m_iCritMult");
	netvar(air_dash, int, "CTFPlayer", "m_iAirDash");
	netvar(headshots, int, "CTFPlayer", "m_iHeadshots");
	netvar(player_class, int, "CTFPlayer", "m_iClass");
	netvar(player_cond, int, "CTFPlayer", "m_nPlayerCond");
	netvar(condition_bits, int, "CTFPlayer", "_condition_bits");
	netvar(player_cond_ex, int, "CTFPlayer", "m_nPlayerCondEx");
	netvar(player_cond_ex_2, int, "CTFPlayer", "m_nPlayerCondEx2");
	netvar(player_cond_ex_3, int, "CTFPlayer", "m_nPlayerCondEx3");
	netvar(player_cond_ex_4, int, "CTFPlayer", "m_nPlayerCondEx4");
	netvar(fov, int, "CBasePlayer", "m_iFOV");
	netvar(hide_hud, int, "CBasePlayer", "m_iHideHUD");
	netvar(water_level, BYTE, "CTFPlayer", "m_nWaterLevel");
	netvar(force_taunt_cam, int, "CTFPlayer", "m_nForceTauntCam");
	netvar(eye_angles, vector_t, "CTFPlayer", "m_angEyeAngles[0]");
	netvar(next_think_tick, int, "CBasePlayer", "m_nNextThinkTick");
	netvar(kills, int, "CTFPlayer", "m_iKills");
	netvar(deaths, int, "CTFPlayer", "m_iDeaths");
	netvar(num_healers, int, "CTFPlayer", "m_nNumHealers");
	netvar(observer_mode, int, "CBasePlayer", "m_iObserverMode");
	netvar(observer_target, int, "CBasePlayer", "m_hObserverTarget");
	netvar(my_weapons, MyWeapons, "CBaseCombatCharacter", "m_hMyWeapons");
	netvar(constraint_entity, c_handle<base_entity>, "CBasePlayer", "m_hConstraintEntity");
	netvar(constraint_center, vector_t, "CBasePlayer", "m_vecConstraintCenter");
	netvar(constraint_radius, float, "CBasePlayer", "m_flConstraintRadius");
	netvar(constraint_width, float, "CBasePlayer", "m_flConstraintWidth");
	netvar(constraint_speed_factor, float, "CBasePlayer", "m_flConstraintSpeedFactor");
	netvar(maxspeed, float, "CBasePlayer", "m_flMaxspeed");

	int& buttons()
	{
		static int nOffset = net_vars->get_net_var("CBasePlayer", "m_hConstraintEntity") - 8;
		return *reinterpret_cast<int*>(reinterpret_cast<usize>(this) + nOffset);
	}

	inline user_cmd*& current_command()
	{
		static const int nOffset = (net_vars->get_net_var("CBasePlayer", "m_hConstraintEntity") - sizeof(uintptr_t));
		return *reinterpret_cast<user_cmd**>(reinterpret_cast<DWORD_PTR>(this) + nOffset);
	}

	inline int& impulse()
	{
		static const int nOffset = net_vars->get_net_var("CBasePlayer", "m_iBonusChallenge") + 0x68;
		return *reinterpret_cast<int*>(reinterpret_cast<DWORD_PTR>(this) + nOffset);
	}

	inline bool using_standard_weapons_in_vehicle()
	{
		static auto siggy = utilities::find_signature<uintptr_t>("client.dll", "48 89 5C 24 ? 57 48 83 EC ? 8B 91 ? ? ? ? 48 8B F9 85 D2 74 ? B8 ? ? ? ? 83 FA ? 74 ? 0F B7 C2 4C 8B 05");
		return reinterpret_cast<bool(__thiscall*)(void*)>(siggy)(this);
	}

	inline int get_ammo_count(int iAmmoType) {
		static auto siggy = utilities::find_signature<uintptr_t>("client.dll", "48 89 5C 24 ? 57 48 83 EC ? 48 63 DA 48 8B F9 83 FB");
		return reinterpret_cast<int(__fastcall*)(void*, int)>(siggy)(this, iAmmoType);
	}

	inline void update_button_state(int nUserCmdButtonMask)
	{
		static auto siggy = utilities::find_signature<uintptr_t>("client.dll", "44 8B 81 ? ? ? ? 89 91");
		reinterpret_cast<void(__thiscall*)(void*, int)>(siggy)(this, nUserCmdButtonMask);
	}

	bool high_kd()
	{
		int _deaths = deaths() ? kills() : 1;
		return (this->kills() / (float)_deaths) > 2.f && this->kills() > 6;
	}

	bool crit_boosted()
	{
		int nCond = player_cond(), nCondEx = player_cond_ex();

		return (nCond & TFCond_Kritzkrieged ||
			condition_bits() & TFCond_Kritzkrieged ||
			nCondEx & TFCondEx_CritCanteen ||
			nCondEx & TFCondEx_CritOnFirstBlood ||
			nCondEx & TFCondEx_CritOnWin ||
			nCondEx & TFCondEx_CritOnKill ||
			nCondEx & TFCondEx_CritDemoCharge ||
			nCondEx & TFCondEx_CritOnFlagCapture ||
			nCondEx & TFCondEx_HalloweenCritCandy ||
			nCondEx & TFCondEx_PyroCrits);
	}

	inline bool mini_crit_boosted() {
		return this->player_cond() & TFCond_MiniCrits;
	}

	inline bool bonked() {
		return this->player_cond() & TFCond_Bonked;
	}

	inline bool charging() {
		return this->player_cond() & TFCond_Charging;
	}

	inline bool taunting() {
		return this->player_cond() & TFCond_Taunting;
	}

	inline bool marked_for_death() {
		return this->player_cond() & TFCond_Jarated
			|| this->player_cond() & TFCond_MarkedForDeath
			|| this->player_cond_ex() & TFCondEx_MarkedForDeathSilent;
	}

	inline bool stunned() {
		return this->player_cond() & TFCond_Stunned;
	}

	inline bool in_kart() {
		return this->player_cond_ex() & TFCondEx_InKart;
	}

	inline bool ghost() {
		return this->player_cond_ex_2() & TFCondEx2_HalloweenGhostMode;
	}

	inline bool cloaked() {
		return this->player_cond() & TFCond_Cloaked
			|| this->player_cond() & TFCond_CloakFlicker
			|| this->player_cond_ex_2() & TFCondEx2_Stealthed
			|| this->player_cond_ex_2() & TFCondEx2_StealthedUserBuffFade;
	}

	inline bool invulnerable() {
		return this->player_cond() & TFCond_Ubercharged
			//|| PlayerCond() & TFCond_UberchargeFading
			//|| this->PlayerCondEx() & TFCondEx_UberchargedHidden
			|| this->player_cond_ex() & TFCondEx_UberchargedCanteen;
	}

	inline bool zoomed() {
		return this->player_cond() & TFCond_Zoomed;
	}

	inline bool overhealed() {
		return this->health() > this->get_max_health();
	}

	inline bool quick_fix_uber() {
		return this->player_cond() & TFCond_MegaHeal;
	}

	inline bool concheror_range() {
		return this->player_cond() & TFCond_RegenBuffed;
	}

	inline bool battalions_range() {
		return this->player_cond() & TFCond_DefenseBuffed;
	}

	inline bool buff_banner_range() {
		return this->player_cond() & TFCond_Buffed;
	}

	inline bool bleeding() {
		return this->player_cond() & TFCond_Bleeding;
	}

	inline bool fire()
	{
		return this->player_cond() & TFCond_OnFire || this->player_cond_ex_3() & (1 << 28);
	}

	inline bool covered_in_milk() {
		return this->player_cond() & TFCond_Milked;
	}

	inline bool slowed() {
		return this->player_cond() & TFCond_Slowed;
	}

	inline bool bullet_resistant() {
		return this->player_cond_ex() & TFCondEx_BulletResistance;
	}

	inline bool explosive_resistant() {
		return this->player_cond_ex() & TFCondEx_ExplosiveResistance;
	}

	inline bool fire_resistant() {
		return this->player_cond_ex() & TFCondEx_FireResistance;
	}

	inline bool bullet_charged() {
		return this->player_cond_ex() & TFCondEx_BulletCharge;
	}

	inline bool explosive_charged() {
		return this->player_cond_ex() & TFCondEx_ExplosiveCharge;
	}

	inline bool fire_charged() {
		return this->player_cond_ex() & TFCondEx_FireCharge;
	}

	bool condition(const ETFCond& condition)
	{
		const int cond = static_cast<int>(condition);
		switch (cond / 32)
		{
		case 0:
		{
			const int bit = (1 << cond);
			if ((player_cond() & bit) == bit)
			{
				return true;
			}

			if ((condition_bits() & bit) == bit)
			{
				return true;
			}

			break;
		}
		case 1:
		{
			const int bit = 1 << (cond - 32);
			if ((player_cond_ex() & bit) == bit)
			{
				return true;
			}

			break;
		}
		case 2:
		{
			const int bit = 1 << (cond - 64);
			if ((player_cond_ex_2() & bit) == bit)
			{
				return true;
			}

			break;
		}
		case 3:
		{
			const int bit = 1 << (cond - 96);
			if ((player_cond_ex_3() & bit) == bit)
			{
				return true;
			}

			break;
		}
		case 4:
		{
			const int bit = 1 << (cond - 128);
			if ((player_cond_ex_4() & bit) == bit)
			{
				return true;
			}

			break;
		}
		default:
			break;
		}

		return false;
	}

	bool alive()
	{
		return life_state() == LIFE_ALIVE;
	}

	bool grounded()
	{
		return flags() & FL_ONGROUND;
	}

	inline bool steam_friend()
	{
		using Fn = bool(__stdcall*)(base_player*);
		static auto pattern = utilities::find_signature<void*>("client.dll",
			"40 57 48 81 EC ? ? ? ? 48 8B FA E8");
		return reinterpret_cast<Fn>(pattern)(this);
	}

	vector_t eye_position()
	{
		return origin() + view_offset();
	}

	int get_max_health();
	void pre_think();
	void post_think();
	void think();
	vector_t get_hitbox_pos(int hitbox);
	vector_t get_hitbox_pos(int hitbox, matrix3x4_t* Matrix, model_t* model);
	float max_speed(bool bIgnoreSpecialAbility = false);
	base_combat_weapon* get_weapon_from_slot(int slot);
	const char* get_weapon_name();
	void update_client_side_animation();
};
