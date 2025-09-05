//
// Created by drev on 15/08/2025.
//

#pragma once
#include "client_unknown.hpp"
#include "client_class.hpp"

class client_class;
struct SpatializationInfo_t;

class client_entity : public client_unknown, public client_renderable, public client_networkable, public client_thinkable
{
public:
    virtual void             release(void) = 0;
    virtual const vector_t     get_abs_origin(void) const = 0;
    virtual const q_angle_t     get_abs_angles(void) const = 0;
    virtual void* get_mouth(void) = 0;
    virtual bool             get_sound_spatialization(SpatializationInfo_t info) = 0;
    virtual bool             is_blurred(void) = 0;

	inline bool is_player()
	{
		return get_client_class() && get_client_class()->ClassID == ETFClassID::CTFPlayer;
	}

	inline bool is_building()
	{
		auto ClassID = get_client_class()->ClassID;
		switch (ClassID)
		{
		case ETFClassID::CObjectSentrygun:
		case ETFClassID::CObjectDispenser:
		case ETFClassID::CObjectTeleporter: return true;
		default: return false;
		}
		return false;
	}

	inline bool is_intel()
	{
		return get_client_class() && get_client_class()->ClassID == ETFClassID::CCaptureFlag;
	}

	inline bool is_bomb()
	{
		return get_client_class() && (get_client_class()->ClassID == ETFClassID::CTFGenericBomb || get_client_class()->ClassID == ETFClassID::CTFPumpkinBomb);
	}

	inline bool is_tank()
	{
		return get_client_class() && get_client_class()->ClassID == ETFClassID::CTFTankBoss;
	}

	inline bool is_projectile()
	{
		if (!get_client_class())
			return false;

		auto ClassID = get_client_class()->ClassID;
		switch (ClassID)
		{
		case ETFClassID::CTFProjectile_Arrow:
		case ETFClassID::CTFProjectile_Cleaver:
		case ETFClassID::CTFProjectile_Flare:
		case ETFClassID::CTFProjectile_HealingBolt:
		case ETFClassID::CTFProjectile_Jar:
		case ETFClassID::CTFProjectile_JarGas:
		case ETFClassID::CTFProjectile_JarMilk:
		case ETFClassID::CTFProjectile_Rocket:
		case ETFClassID::CTFProjectile_SentryRocket:
		case ETFClassID::CTFGrenadePipebombProjectile: return true;
		default: return false;
		}
		return false;
	}

	template < typename T >
	inline T* as()
	{
		return reinterpret_cast<T*>(this);
	}
};