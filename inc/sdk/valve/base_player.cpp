//
// Created by drev on 30/08/2025.
//

#include "base_player.hpp"
#include "link.hpp"

//todo

int base_player::get_max_health()
{
    return utilities::find_vfunc<107, int>(this);
}

void base_player::pre_think()
{
    return utilities::find_vfunc<260, void>(this);
}

void base_player::post_think()
{
    return utilities::find_vfunc<261, void>(this);
}

void base_player::think()
{
    return utilities::find_vfunc<174, void>(this);
}

vector_t base_player::get_hitbox_pos(int Hitbox)
{
    if (auto hdr = ctx.interfaces.model_info->get_studio_model(get_model()))
    {
        matrix3x4_t Matrix[128];
        setup_bones(Matrix, 128, 0x100, ctx.interfaces.globals->curtime);

        if (Matrix)
        {
            if (auto set = hdr->get_hitbox_set(hitbox_set()))
            {
                if (const auto& box = set->hitbox(Hitbox))
                {
                    vector_t pos = (box->bbmin + box->bbmax) * 0.5f;
                    vector_t out = {};
                    mathematics::vector_transform(pos, Matrix[box->bone], out);
                    return out;
                }
            }
        }
    }

    return {};
}

vector_t base_player::get_hitbox_pos(int Hitbox, matrix3x4_t* Matrix, model_t* model)
{
    if (auto hdr = ctx.interfaces.model_info->get_studio_model(model))
    {
        if (Matrix)
        {
            if (auto set = hdr->get_hitbox_set(hitbox_set()))
            {
                if (const auto& box = set->hitbox(Hitbox))
                {
                    vector_t pos = (box->bbmin + box->bbmax) * 0.5f;
                    vector_t out = {};
                    mathematics::vector_transform(pos, Matrix[box->bone], out);
                    return out;
                }
            }
        }
    }

    return {};
}

float base_player::max_speed(bool bIgnoreSpecialAbility)
{
    return 0.0f;
}

base_combat_weapon* base_player::get_weapon_from_slot(int slot)
{
    return nullptr;
}

const char* base_player::get_weapon_name()
{
    return nullptr;
}

void base_player::update_client_side_animation()
{
}