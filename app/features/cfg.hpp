#pragma once

#include "../config_system/config_system.hpp"
#include <array>

enum class player_type {
    enemy,
    team,
    friends,
    local,
    count
};

namespace cfg {
    CFGVAR(esp_enemy_esp_active, false);
    CFGVAR(esp_enemy_name, false);
    CFGVAR(esp_enemy_box, false);
    CFGVAR(esp_enemy_bones, false);
    CFGVAR(esp_enemy_health_bar, false);
    CFGVAR(esp_enemy_health_text, false);
    CFGVAR(esp_enemy_health_color, color_t(255, 255, 255, 255));
    CFGVAR(esp_enemy_custom_health_color, false);

    CFGVAR(esp_team_esp_active, false);
    CFGVAR(esp_team_name, false);
    CFGVAR(esp_team_box, false);
    CFGVAR(esp_team_bones, false);
    CFGVAR(esp_team_health_bar, false);
    CFGVAR(esp_team_health_text, false);
    CFGVAR(esp_team_health_color, color_t(255, 255, 255, 255));
    CFGVAR(esp_team_custom_health_color, false);

    CFGVAR(esp_friends_esp_active, false);
    CFGVAR(esp_friends_name, false);
    CFGVAR(esp_friends_box, false);
    CFGVAR(esp_friends_bones, false);
    CFGVAR(esp_friends_health_bar, false);
    CFGVAR(esp_friends_health_text, false);
    CFGVAR(esp_friends_health_color, color_t(255, 255, 255, 255));
    CFGVAR(esp_friends_custom_health_color, false);

    CFGVAR(esp_local_esp_active, false);
    CFGVAR(esp_local_name, false);
    CFGVAR(esp_local_box, false);
    CFGVAR(esp_local_bones, false);
    CFGVAR(esp_local_health_bar, false);
    CFGVAR(esp_local_health_text, false);
    CFGVAR(esp_local_health_color, color_t(255, 255, 255, 255));
    CFGVAR(esp_local_custom_health_color, false);

    CFGVAR(esp_enemy_box_color, color_t(255, 0, 0, 255));
    CFGVAR(esp_team_box_color, color_t(0, 255, 0, 255));
    CFGVAR(esp_friends_box_color, color_t(0, 0, 255, 255));
    CFGVAR(esp_local_box_color, color_t(255, 255, 255, 255));

    CFGVAR(esp_enemy_custom_box_color, false);
    CFGVAR(esp_team_custom_box_color, false);
    CFGVAR(esp_friends_custom_box_color, false);
    CFGVAR(esp_local_custom_box_color, false);

    inline bool menu_open = false;
}
