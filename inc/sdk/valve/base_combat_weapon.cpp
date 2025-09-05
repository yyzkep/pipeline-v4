//
// Created by drev on 30/08/2025.
//

#include "base_combat_weapon.hpp"
#include "link.hpp"

bool base_combat_weapon::can_primary_attack(base_player* pOwner)
{
    bool bOut = false;

    if (pOwner) {
        float flCurTime = static_cast<float>(pOwner->tick_base()) * ctx.interfaces.globals->intervalPerTick;
        bOut = next_primary_attack() <= flCurTime && pOwner->next_attack() <= flCurTime;
    }

    return bOut;
}

bool base_combat_weapon::can_secondary_attack(base_player* pOwner)
{
    bool bOut = false;

    if (pOwner) {
        float flCurTime = static_cast<float>(pOwner->tick_base()) * ctx.interfaces.globals->intervalPerTick;
        bOut = next_secondary_attack() <= flCurTime && pOwner->next_attack() <= flCurTime;
    }

    return bOut;
}

bool base_combat_weapon::has_primary_ammo_for_shot()
{
    if (is_energy_weapon())
        return energy() > 0.0f;

    int nClip1 = clip_1();

    if (nClip1 == -1)
    {
        if (auto pOwner = ctx.interfaces.entity_list->get_client_entity(owner_entity()))
        {
            int nAmmoCount = pOwner->as<base_player>()->get_ammo_count(primary_ammo_type());

            if (item_definition_index() == Engi_m_TheWidowmaker)
                return nAmmoCount > 29;

            return nAmmoCount > 0;
        }
    }

    return nClip1 > 0;
}