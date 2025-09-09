#pragma once
#include "config_system/config_system.hpp"
#include "color.hpp"

namespace config
{
    inline struct esp_players
    {
        bool active = false;
        bool name = false;
        bool box = false;
        bool bones = false;
        bool health_bar = false;
        bool health_text = false;
        bool distance = false;
        bool color_scheme = false;
        bool custom_healthbar_color = false;

        color_t box_color = color_t(255, 255, 255);
        color_t healthbar_color = color_t(255, 255, 255);
    } esp_player[5];

    // above is a array of arrays
    // using it because why not
    // 0 - 4 goes from enemy, team, local, friends, cheaters

    inline bool auto_save = true;

    void load_config(std::string cfg);
    void save_config(std::string out_string);
}
