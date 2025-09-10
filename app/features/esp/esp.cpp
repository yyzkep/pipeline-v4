#include "esp.hpp"
#include "../cfg.hpp"

// this is so ugly
// i hate myself
#define CFG_GETTER(name) \
bool get_##name(player_type type) { \
    switch (type) { \
        case player_type::enemy:   return config::esp_enemy_##name; \
        case player_type::team:    return config::esp_team_##name; \
        case player_type::friends: return config::esp_friends_##name; \
        case player_type::local:   return config::esp_local_##name; \
        default: return false; \
    } \
}
#define CFG_COLOR_GETTER(name) \
color_t get_##name(player_type type) { \
    switch(type) { \
        case player_type::enemy:   return config::esp_enemy_##name; \
        case player_type::team:    return config::esp_team_##name; \
        case player_type::friends: return config::esp_friends_##name; \
        case player_type::local:   return config::esp_local_##name; \
        default: return color_t::white(); \
    } \
}
CFG_GETTER(esp_active)
CFG_GETTER(name)
CFG_GETTER(box)
CFG_GETTER(bones)
CFG_GETTER(health_bar)
CFG_GETTER(health_text)
CFG_COLOR_GETTER(box_color)
player_type get_player_category(base_player* player, int local_team)
{
    if (!player)
        return player_type::count;

    int player_team = player->team_num();
    bool is_friend = player->steam_friend();

    if (player == ctx.entities.local_player)
        return player_type::local;

    if (is_friend)
        return player_type::friends;

    if (player_team == local_team)
        return player_type::team;

    return player_type::enemy;
}
color_t get_health_color(base_player* player, int health, int max_health)
{
    player_type category = get_player_category(player, ctx.entities.local_player->team_num());

    switch (category)
    {
    case player_type::enemy:
        return config::esp_enemy_custom_health_color ? config::esp_enemy_health_color : ctx.tf2.get_health_color(health, max_health);
    case player_type::team:
        return config::esp_team_custom_health_color ? config::esp_team_health_color : ctx.tf2.get_health_color(health, max_health);
    case player_type::friends:
        return config::esp_friends_custom_health_color ? config::esp_friends_health_color : ctx.tf2.get_health_color(health, max_health);
    case player_type::local:
        return config::esp_local_custom_health_color ? config::esp_local_health_color : ctx.tf2.get_health_color(health, max_health);
    default:
        return ctx.tf2.get_health_color(health, max_health);
    }
}


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
    if (!ctx.interfaces.engine->is_in_game() || !ctx.interfaces.vgui->is_game_uivisible())
        return;

    int x, y, w, h;

    for (int i = 0; i < ctx.interfaces.entity_list->get_highest_entity_index(); i++)
    {
        client_entity* entity = ctx.interfaces.entity_list->get_client_entity(i);
        if (!entity || !entity->is_player() || entity->is_dormant())
            continue;

        base_player* player = entity->as<base_player>();
        player_type type = get_player_category(player, ctx.entities.local_player->team_num());

        if (!get_esp_active(type))
            continue;

        if (!ctx.tf2.get_entity_bounds(player, x, y, w, h))
            continue;

        if (get_name(type)) {
            player_info_t pi;
            if (ctx.interfaces.engine->get_player_info(player->ent_idx(), &pi)) {
                ctx.renderer.render_queue.string(
                    FONTS::FONT_ESP,
                    x + w / 2,
                    y,
                    color_t::white(),
                    horizontal,
                    pi.name
                );
            }
        }

        if (get_box(type)) {
            ctx.renderer.render_queue.rect(
                x, y, w, h,
                get_box_color(type)
            );
        }

        if (get_bones(type)) {
            const model_t* model = player->get_model();
            if (!model) continue;

            studiohdr_t* hdr = ctx.interfaces.model_info->get_studio_model(model);
            if (!hdr) continue;

            matrix3x4_t matrix[128];
            if (!player->setup_bones(matrix, 128, BONE_USED_BY_ANYTHING, ctx.interfaces.globals->curtime))
                continue;

            for (int b = 0; b < hdr->numbones; ++b) {
                mstudiobone_t* bone = hdr->get_bone(b);
                if (!bone || !(bone->flags & BONE_USED_BY_HITBOX)) continue;

                int parent = bone->parent;
                if (parent == -1) continue;

                vector_t bone_pos, parent_pos;
                vector_t bone_pos_screen, parent_pos_screen;

                matrix->get_bone(bone_pos, b);
                matrix->get_bone(parent_pos, parent);

                if (ctx.tf2.w2s(bone_pos, bone_pos_screen) && ctx.tf2.w2s(parent_pos, parent_pos_screen)) {
                    ctx.renderer.render_queue.line(
                        bone_pos_screen.x, bone_pos_screen.y,
                        parent_pos_screen.x, parent_pos_screen.y,
                        color_t::white()
                    );
                }
            }
        }

        if (get_health_bar(type)) {
            const int health = player->health();
            const int max_health = player->get_max_health();
            const float ratio = std::clamp<float>(health, 1.0f, max_health) / max_health;

            constexpr int n_width = 2;
            const int n_height = h + (health < max_health ? 2 : 1);

            color_t clr_health = get_health_color(player, health, max_health);

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
        }

        if (get_health_text(type)) {
            const int health = player->health();
            const int max_health = player->get_max_health();
            const float ratio = std::clamp<float>(health, 1.0f, max_health) / max_health;
            const int n_height = h + (health < max_health ? 2 : 1);

            ctx.renderer.render_queue.string(
                FONTS::FONT_ESP,
                x,
                y + n_height - static_cast<int>(n_height * ratio),
                color_t::white(),
                reverse_horizontal,
                std::format("%d", health)
            );
        }
    }
}



void c_esp::draw_buildings()
{
}

void c_esp::draw_world()
{
}
