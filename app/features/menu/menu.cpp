#include "menu.hpp"
#include "..\cfg.hpp"

//i used chatgpt to save time, changing this in the widget update, this is so that i dont have to suffer...
void c_menu::draw_menu()
{
    if (!cfg::menu_open)
        return;

    ImGui::Begin("pipeline v4");

    if (ImGui::BeginTabBar("Tabs"))
    {
        if (ImGui::BeginTabItem("Aimbot"))
        {
            aimbot_tab();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Visuals"))
        {
            visuals_tab();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Misc"))
        {
            misc_tab();
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem("Settings"))
        {
            settings_tab();
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
}

void c_menu::aimbot_tab()
{
    ImGui::Text("Aimbot Options");
}

void c_menu::visuals_tab()
{
    ImGui::Text("ESP Options");
    //ai just smoking
    auto draw_esp_header = [](const char* label,
        bool& active,
        bool& name,
        bool& box,
        bool& bones,
        bool& health_bar,
        bool& health_text,
        color_t& health_color,
        bool& custom_health_color,
        color_t& box_color,
        bool& custom_box_color)
        {
            if (ImGui::CollapsingHeader(label))
            {
                ImGui::Checkbox("Enable", &active);
                ImGui::Checkbox("Name", &name);
                ImGui::Checkbox("Box", &box);
                ImGui::Checkbox("Bones", &bones);
                ImGui::Checkbox("Health Bar", &health_bar);
                ImGui::Checkbox("Health Text", &health_text);

                float hc[4];
                health_color.to_float4(hc);
                if (ImGui::ColorEdit4("Health Color", hc))
                {
                    health_color = color_t(static_cast<int>(hc[0] * 255.f),
                        static_cast<int>(hc[1] * 255.f),
                        static_cast<int>(hc[2] * 255.f),
                        static_cast<int>(hc[3] * 255.f));
                }
                ImGui::Checkbox("Custom Health Color", &custom_health_color);

                float bc[4];
                box_color.to_float4(bc);
                if (ImGui::ColorEdit4("Box Color", bc))
                {
                    box_color = color_t(static_cast<int>(bc[0] * 255.f),
                        static_cast<int>(bc[1] * 255.f),
                        static_cast<int>(bc[2] * 255.f),
                        static_cast<int>(bc[3] * 255.f));
                }
                ImGui::Checkbox("Custom Box Color", &custom_box_color);
            }
        };


    draw_esp_header("Enemy ESP",
        cfg::esp_enemy_esp_active,
        cfg::esp_enemy_name,
        cfg::esp_enemy_box,
        cfg::esp_enemy_bones,
        cfg::esp_enemy_health_bar,
        cfg::esp_enemy_health_text,
        cfg::esp_enemy_health_color,
        cfg::esp_enemy_custom_health_color,
        cfg::esp_enemy_box_color,
        cfg::esp_enemy_custom_box_color);

    draw_esp_header("Team ESP",
        cfg::esp_team_esp_active,
        cfg::esp_team_name,
        cfg::esp_team_box,
        cfg::esp_team_bones,
        cfg::esp_team_health_bar,
        cfg::esp_team_health_text,
        cfg::esp_team_health_color,
        cfg::esp_team_custom_health_color,
        cfg::esp_team_box_color,
        cfg::esp_team_custom_box_color);

    draw_esp_header("Friends ESP",
        cfg::esp_friends_esp_active,
        cfg::esp_friends_name,
        cfg::esp_friends_box,
        cfg::esp_friends_bones,
        cfg::esp_friends_health_bar,
        cfg::esp_friends_health_text,
        cfg::esp_friends_health_color,
        cfg::esp_friends_custom_health_color,
        cfg::esp_friends_box_color,
        cfg::esp_friends_custom_box_color);

    draw_esp_header("Local ESP",
        cfg::esp_local_esp_active,
        cfg::esp_local_name,
        cfg::esp_local_box,
        cfg::esp_local_bones,
        cfg::esp_local_health_bar,
        cfg::esp_local_health_text,
        cfg::esp_local_health_color,
        cfg::esp_local_custom_health_color,
        cfg::esp_local_box_color,
        cfg::esp_local_custom_box_color);

}

void c_menu::misc_tab()
{
    ImGui::Text("Misc Options");
}

void c_menu::settings_tab()
{
    ImGui::Text("Settings");
    if (ImGui::Button("Save Config"))
    {
        // save_config();
    }
    if (ImGui::Button("Load Config"))
    {
        // load_config();
    }
}
