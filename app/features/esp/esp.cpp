#include "esp.hpp"
#include "../config.hpp"

void c_esp::draw()
{
	// todo
	//jod bless c++ standard library

	draw_entities();
}

void c_esp::draw_entities()
{
	if (!ctx.interfaces.engine->is_in_game() || !ctx.interfaces.vgui->is_game_uivisible())
		return;

	draw_players();
	draw_buildings();
	draw_world();
}

void c_esp::draw_players()
{
    int x, y, w, h;
    for (auto i = 0; i < ctx.interfaces.entity_list->get_highest_entity_index(); i++)
    {
        client_entity* entity = ctx.interfaces.entity_list->get_client_entity(i);
        if (!entity || !entity->is_player() || entity->is_dormant())
            continue;

        base_player* player = entity->as<base_player>();

        int category = 0; 
        if (player == ctx.entities.local_player) category = 2;
        else if (player->team_num() == ctx.entities.local_player->team_num()) category = 1;
        else if (player->steam_friend()) category = 3;
        //4 would be cheater, but we dont have db yet so dont bother.

        if (!config::esp_player[category].active)
            continue;

        if (!ctx.tf2.get_entity_bounds(player, x, y, w, h))
            continue;

        player_info_t pi;
        if (ctx.interfaces.engine->get_player_info(player->ent_idx(), &pi) &&
            config::esp_player[category].name)
        {
            ctx.renderer.render_queue.string(
                FONTS::FONT_ESP,
                x + (w / 2),
                y,
                color_t::white(),
                horizontal,
                pi.name
            );
        }

        if (config::esp_player[category].box)
            ctx.renderer.render_queue.rect(
                x, y, w, h,
                config::esp_player[category].box_color
            );
    }
}

void c_esp::draw_buildings()
{
}

void c_esp::draw_world()
{
}
