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

        if (config::esp_player[category].bones) {
			const model_t* model;
			studiohdr_t* hdr;
			mstudiobone_t* bone;
			int           parent;
			matrix3x4_t     matrix[128];
			vector_t        bone_pos, parent_pos;
			vector_t        bone_pos_screen, parent_pos_screen;

			model = player->get_model();
			if (!model)
				return;

			hdr = ctx.interfaces.model_info->get_studio_model(model);
			if (!hdr)
				return;

			if (!player->setup_bones(matrix, 128, BONE_USED_BY_ANYTHING, ctx.interfaces.globals->curtime))
				return;

			for (int i{ }; i < hdr->numbones; ++i) {
				// get bone.
				bone = hdr->get_bone(i);
				if (!bone || !(bone->flags & BONE_USED_BY_HITBOX))
					continue;

				// get parent bone.
				parent = bone->parent;
				if (parent == -1)
					continue;


				// resolve main bone and parent bone positions.
				matrix->get_bone(bone_pos, i);
				matrix->get_bone(parent_pos, parent);

				color_t clr = color_t(255, 255, 255, 255);

				if (ctx.tf2.w2s(bone_pos, bone_pos_screen) && ctx.tf2.w2s(parent_pos, parent_pos_screen)) {
					ctx.renderer.render_queue.line(bone_pos_screen.x, bone_pos_screen.y, parent_pos_screen.x, parent_pos_screen.y, clr);
				}
			}
        }


        //simple is good
		if (config::esp_player[category].health_bar) {
            --x;

            const int health = player->health();
            const int max_health = player->get_max_health();

            const float fl_health = std::clamp<float>(health, 1.0f, max_health);
            const float ratio = fl_health / max_health;

            constexpr int n_width = 2;
            const int n_height = h + (fl_health < max_health ? 2 : 1);

            color_t clr_health = config::esp_player[category].custom_healthbar_color
                ? config::esp_player[category].healthbar_color
                : ctx.tf2.get_health_color(health, max_health);

            ctx.renderer.render_queue.filled_rect(
                x - n_width - 2,
                y + n_height - static_cast<int>(n_height * ratio),
                n_width,
                static_cast<int>(n_height * ratio),
                clr_health
            );

            ctx.renderer.render_queue.rect(
                x - n_width - 3,
                y + n_height - static_cast<int>(n_height * ratio) - 1,
                n_width + 2,
                static_cast<int>(n_height * ratio) + 1,
                color_t::black()
            );

            ++x;
		}

        if (config::esp_player[category].health_text) {
            ctx.renderer.render_queue.string(FONTS::FONT_ESP, x, y - h /*i hope this is right*/, color_t(255, 255, 255), reverse_horizontal, std::format("%s", player->health()));
        }
    }
}

void c_esp::draw_buildings()
{
}

void c_esp::draw_world()
{
}
