#pragma once

// C standard library
#include <cassert>
#include <cctype>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cmath>

// C++ standard library
#include <algorithm>
#include <array>
#include <chrono>
#include <deque>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <set>
#include <sstream>
#include <string>
#include <string_view>
#include <thread>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>

// windows bs
#include <Windows.h>
#include <d3d9.h>

// 3rd party
#include <imgui.h>
#include <imgui_impl_dx9.h>
#include <misc/freetype/imgui_freetype.h>
#include <imgui_impl_win32.h>
#include <freetype/freetype.h>
#include <safetyhook.hpp>

//idk
#include "vector.hpp"
#include "matrix.hpp"
#include "color.hpp"
#include "draw_manager/draw_manager.hpp"
#include "utils.hpp"
#include "fnv1a.hpp"

//interfaces
#include "sdk/valve/base_client_dll.hpp"
#include "sdk/valve/base_player.hpp"
#include "sdk/valve/client_mode.hpp"
#include "sdk/valve/client_state.hpp"
#include "sdk/valve/engine_client.hpp"
#include "sdk/valve/engine_vgui.hpp"
#include "sdk/valve/entity_list.hpp"
#include "sdk/valve/model_info.hpp"
#include "sdk/valve/cvar.hpp"
#include "sdk/valve/debug.hpp"
#include "sdk/valve/game_events.hpp"
#include "sdk/valve/material_system.hpp"


class c_ctx {
public:
    struct interfaces_t {
        base_client_dll* client = nullptr;
        iv_engine_client* engine = nullptr;
        i_engine_trace* trace = nullptr;
        i_engine_vgui* vgui = nullptr;
        global_vars_base* globals = nullptr;
        iv_model_info* model_info = nullptr;
        iv_render_view* render_view = nullptr;
        i_mat_system_surface* surface = nullptr;
        client_entity_list* entity_list = nullptr;
        i_cvar* cvar = nullptr;
        iv_debug_overlay* debug = nullptr;
        c_pred* pred = nullptr;
        i_game_event_manager* event = nullptr;
        i_material_system* material_system = nullptr;
        c_client_state* state = nullptr;
        client_mode_shared* shared = nullptr;
        i_input* input = nullptr;
        i_input_system* input_system = nullptr;
    } interfaces;

    struct entities_t {
        base_player* local_player = nullptr;
        base_combat_weapon* local_weapon = nullptr;
    } entities;

    struct renderer_t {
        render_state_manager state_manager{};
        draw_commands_executor command_executor{};
        render_command_queue render_queue{};
    } renderer;

    struct tf2_t {
        void get_interfaces();
        void grab_view_matrix();
        bool w2s(const vector_t& origin, vector_t& screen);
        bool get_entity_bounds(base_entity* entity, int& x, int& y, int& w, int& h);
        color_t get_health_color(int health, int max_health);
        D3DMATRIX w2s_matrix;
    } tf2;
};

inline c_ctx ctx = {};


//greentek_  6:38 PM
// 
//pay me to use your cheat
//so i can make a profit
// 
//ts pmo

