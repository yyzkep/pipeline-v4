//
// Created by drev on 30/08/2025.
//

#pragma once
#include "base_player.hpp"
#include "entity_list.hpp"


typedef enum
{
	EMPTY,
	SINGLE,
	SINGLE_NPC,
	WPN_DOUBLE,
	DOUBLE_NPC,
	BURST,
	RELOAD,
	RELOAD_NPC,
	MELEE_MISS,
	MELEE_HIT,
	MELEE_HIT_WORLD,
	SPECIAL1,
	SPECIAL2,
	SPECIAL3,
	TAUNT,
	DEPLOY,
	NUM_SHOOT_SOUND_TYPES,
} weapon_sound_t;

#define MAX_SHOOT_SOUNDS			16
#define MAX_WEAPON_STRING			80
#define MAX_WEAPON_PREFIX			16
#define MAX_WEAPON_AMMO_NAME		32
#define WEAPON_PRINTNAME_MISSING	"!!! Missing printname on weapon"

struct CHudTexture;
class file_weapon_info_t
{
public:
	file_weapon_info_t();
	virtual void parse(void* pKeyValuesData, const char* szWeaponName);
public:
	bool parsed_script;
	bool loaded_hud_elements;
	char class_name[MAX_WEAPON_STRING];
	char print_name[MAX_WEAPON_STRING];
	char view_model[MAX_WEAPON_STRING];
	char world_model[MAX_WEAPON_STRING];
	char animation_prefix[MAX_WEAPON_PREFIX];
	int slot;
	int position;
	int max_clip_1;
	int max_clip_2;
	int default_clip_1;
	int default_clip_2;
	int weight;
	int rumble_effect;
	bool auto_switch_to;
	bool auto_switch_from;
	int flags;
	char ammo_1[MAX_WEAPON_AMMO_NAME];
	char ammo_2[MAX_WEAPON_AMMO_NAME];

	char a_shoot_sounds[NUM_SHOOT_SOUND_TYPES][MAX_WEAPON_STRING];

	int ammo_type;
	int ammo_2_type;
	bool melee_weapon;
	bool built_right_handed;
	bool allow_flipping;

	bool show_usage_hint;
};

struct fire_bullets_info_t
{
	int m_iShots;
	Vector m_vecSrc;
	Vector m_vecDirShooting;
	Vector m_vecSpread;
	float m_flDistance;
	int m_iAmmoType;
	int m_iTracerFreq;
	float m_flDamage;
	int m_iPlayerDamage;
	int m_nFlags;
	float m_flDamageForceScale;
	base_entity* m_pAttacker;
	base_entity* m_pAdditionalIgnoreEnt;
	bool m_bPrimaryAttack;
	bool m_bUseServerRandomSeed;
};


struct weapon_data_t
{
	int	damage;
	int	bullets_per_shot;
	float range;
	float spread;
	float punch_angle;
	float time_fire_delay;
	float time_idle;
	float time_idle_empty;
	float time_reload_start;
	float time_reload;
	bool draw_crosshair;
	int	projectile;
	int	ammo_per_shot;
	float projectile_speed;
	float smack_delay;
	bool use_rapid_fire_crits;

	void init(void)
	{
		damage = 0;
		bullets_per_shot = 0;
		range = 0.0f;
		spread = 0.0f;
		punch_angle = 0.0f;
		time_fire_delay = 0.0f;
		time_idle = 0.0f;
		time_idle_empty = 0.0f;
		time_reload_start = 0.0f;
		time_reload = 0.0f;
		projectile = 0;
		ammo_per_shot = 0;
		projectile_speed = 0.0f;
		smack_delay = 0.0f;
		use_rapid_fire_crits = false;
	};
};

class tf_weapon_info : public file_weapon_info_t
{
public:
	tf_weapon_info();
	~tf_weapon_info();

	virtual void parse(void* pKeyValuesData, const char* szWeaponName);
	weapon_data_t const& get_weapon_data(int iWeapon) const { return weapon_data[iWeapon]; }
public:
	weapon_data_t	weapon_data[2];
	int		weapon_type;
	bool	grenade;
	float	damage_radius;
	float	primer_time;
	bool	lower_weapon;
	bool	suppress_gren_timer;
	bool	has_team_skins_viewmodel;
	bool	has_team_skins_worldmodel;
	char	muzzle_flash_model[128];
	float	muzzle_flash_model_duration;
	char	muzzle_flash_particle_effect[128];
	char	tracer_effect[128];
	bool	do_instant_eject_brass;
	char	brass_model[128];
	char	explosion_sound[128];
	char	explosion_effect[128];
	char	explosion_player_effect[128];
	char	explosion_water_effect[128];
	bool	dont_drop;
};

class CTFWeaponBase;
class base_combat_weapon : public base_entity {
public:
	netvar(next_primary_attack, float, "CBaseCombatWeapon", "m_flNextPrimaryAttack");
	netvar(time_weapon_idle, float, "CBaseCombatWeapon", "m_flTimeWeaponIdle ");
	netvar(next_secondary_attack, float, "CBaseCombatWeapon", "m_flNextSecondaryAttack");
	netvar(clip_1, int, "CBaseCombatWeapon", "m_iClip1");
	netvar(item_definition_index, int, "CEconEntity", "m_iItemDefinitionIndex");
	netvar(charged_damage, float, "CTFSniperRifle", "m_flChargedDamage");
	netvar(last_fire_time, float, "CTFWeaponBase", "m_flLastFireTime");
	netvar(charge_begin_time, float, "CTFPipebombLauncher", "m_flChargeBeginTime");
	netvar(detonate_time, float, "CWeaponGrenadeLauncher", "m_flDetonateTime");
	netvar(healing_target, unsigned int, "CWeaponMedigun", "m_hHealingTarget");
	netvar(energy, float, "CTFWeaponBase", "m_flEnergy");
	netvar(charge_resist_type, int, "CWeaponMedigun", "m_nChargeResistType");
	netvar(charge_level, float, "CWeaponMedigun", "m_flChargeLevel");
	netvar(primary_ammo_type, int, "CBaseCombatWeapon", "m_iPrimaryAmmoType");
	netvar(observed_crit_chance, float, "CTFWeaponBase", "m_flObservedCritChance");
	netvar(reload_mode, int, "CTFWeaponBase", "m_iReloadMode");

	offset_pure(minigun_state, int, 0xC48);
	offset_pure(crit_token_bucket, float, 0xA54);
	offset_pure(crit_checks, int, 0xA58);
	offset_pure(crit_seed_requests, int, 0xA5C);
	offset_pure(weapon_mode, int, 0xB24);
	offset_pure(crit_time, float, 0xB50);
	offset_pure(last_crit_check_time, float, 0xB54);
	offset_pure(last_crit_check_frame, int, 0xB58);
	offset_pure(last_rapidfire_crit_check_time, float, 0xB60);
	offset_pure(clip_2, int, 0xAAC);
	offset_pure(crit_shot, bool, 0xB36);
	offset_pure(random_seed, int, 0xB5C);

	inline int get_weapon_id()
	{
		return utilities::find_vfunc<383, int>(this);
	}

	int get_damage_type() {
		return utilities::find_vfunc<384, int>(this);
	}

	float get_swing_range() {
		return get_weapon_id() == TF_WEAPON_SWORD ? 72.0f : 48.0f;
	}

	float& smack_time() {
		static int nOffset = net_vars->get_net_var("CTFWeaponBase", "m_nInspectStage") + 28;
		return *reinterpret_cast<float*>(reinterpret_cast<std::uintptr_t>(this) + nOffset);
	}

	bool can_fire_critical_shot(base_player* pOwner, bool bIsHeadshot = false)
	{
		bool bOut = false;

		if (pOwner)
		{
			int& iFOV = pOwner->fov(), nFovBackup = iFOV;
			iFOV = 70;
			bOut = utilities::find_vfunc<428, bool, bool, void*>(this, bIsHeadshot, nullptr); /*reinterpret_cast<bool(__fastcall*)(void*, bool, void*)>(Memory::GetVFunc(this, 428))(this, bIsHeadshot, nullptr);*/
			iFOV = nFovBackup;
		}

		return bOut;
	}

	bool can_head_shot(base_player* pOwner)
	{
		bool bOut = false;

		if (pOwner)
			bOut = (get_damage_type() & DMG_USE_HITLOCATIONS) && can_fire_critical_shot(pOwner, true);

		return bOut;
	}

	bool can_primary_attack(base_player* pOwner);

	bool can_secondary_attack(base_player* pOwner);

	tf_weapon_info* get_weapon_info() {
		static int nOffset = net_vars->get_net_var("CTFWeaponBase", "m_flEffectBarRegenTime") + 16;
		return *reinterpret_cast<tf_weapon_info**>(reinterpret_cast<std::uintptr_t>(this) + nOffset);
	}

	bool is_energy_weapon() {
		return utilities::find_vfunc<435, bool>(this);
	}

	bool has_primary_ammo_for_shot();

	int& current_seed() {
		static int nOffset = net_vars->get_net_var("CTFWeaponBase", "m_flLastCritCheckTime") + 8;
		return *reinterpret_cast<int*>(reinterpret_cast<std::uintptr_t>(this) + nOffset);
	}

	void get_projectile_fire_setup(void* pPlayer, Vector vecOffset, Vector* vecSrc, q_angle_t* angForward, bool bHitTeammates = true, float flEndDist = 2000.0f) {
		return utilities::find_vfunc<402, void, void*, Vector, Vector*, q_angle_t*, bool, float>(this, pPlayer, vecOffset, vecSrc, angForward, bHitTeammates, flEndDist);
	}

	void get_spread_angles(Vector& out) {
		static auto siggy = utilities::find_signature<uintptr_t>("client.dll", "48 89 5C 24 ? 57 48 83 EC ? 48 63 DA 48 8B F9 83 FB");
		reinterpret_cast<void(__fastcall*)(void*, Vector&)>(siggy)(this, out);
	}

};