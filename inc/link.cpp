//
// Created by drev on 15/08/2025.
//

#include "link.hpp"

void c_ctx::tf2_t::get_interfaces() {
    check_interface(ctx.interfaces.client, base_client_dll, "client.dll", "VClient017");
    check_interface(ctx.interfaces.cvar, i_cvar, "vstdlib.dll", "VEngineCvar004");
    check_interface(ctx.interfaces.debug, iv_debug_overlay, "engine.dll", "VDebugOverlay003");
    check_interface(ctx.interfaces.engine, iv_engine_client, "engine.dll", "VEngineClient013");
    check_interface(ctx.interfaces.entity_list, client_entity_list, "client.dll", "VClientEntityList003");
    check_interface_signature(ctx.interfaces.globals, global_vars_base, "engine.dll", "48 8D 05 ? ? ? ? C3 CC CC CC CC CC CC CC CC 48 8B CA");
    check_interface(ctx.interfaces.model_info, iv_model_info, "engine.dll", "VModelInfoClient006");
    check_interface(ctx.interfaces.render_view, iv_render_view, "engine.dll", "VEngineRenderView014");
    check_interface(ctx.interfaces.surface, i_mat_system_surface, "vguimatsurface.dll", "VGUI_Surface030");
    check_interface(ctx.interfaces.trace, i_engine_trace, "engine.dll", "EngineTraceClient003");
    check_interface(ctx.interfaces.vgui, i_engine_vgui, "engine.dll", "VEngineVGui002");
    //past was check_interface.. lmao
    check_interface_signature(ctx.interfaces.state, c_client_state, "engine.dll", "48 8D 0D ? ? ? ? E8 ? ? ? ? F3 0F 5E 05");
    check_interface_signature_dereferenced(ctx.interfaces.shared, client_mode_shared, "client.dll", "48 8B 0D ? ? ? ? 48 8B 10 48 8B 19 48 8B C8 FF 92", 1);
    check_interface(ctx.interfaces.pred, c_pred, "client.dll", "VClientPrediction001");
    check_interface(ctx.interfaces.material_system, i_material_system, "materialsystem.dll", "VMaterialSystem082");
    check_interface_signature_dereferenced(ctx.interfaces.input, i_input, "client.dll", "48 8B 0D ? ? ? ? 48 8B 01 FF 90 ? ? ? ? 85 C0 0F 84 ? ? ? ? F3 0F 10 05", 1);
    check_interface(ctx.interfaces.input_system, i_input_system, "inputsystem.dll", "InputSystemVersion001");
}

void c_ctx::tf2_t::grab_view_matrix()
{
    static c_view_setup view = {};

    if (ctx.interfaces.client->get_player_view(view)) {
        static D3DMATRIX w1 = {};
        static D3DMATRIX w2 = {};
        static D3DMATRIX w3 = {};
        ctx.interfaces.render_view->get_matrices_for_view(view, &w1, &w2, &w2s_matrix, &w3);
    }
}

bool c_ctx::tf2_t::get_entity_bounds(base_entity* entity, int& x, int& y, int& w, int& h)
{

    const auto& mins = entity->mins();
    const auto& maxs = entity->maxs();

    //this might be retarded.
    vector_t points[8] = {
        vector_t(mins.x, mins.y, mins.z),
        vector_t(mins.x, maxs.y, mins.z),
        vector_t(maxs.x, maxs.y, mins.z),
        vector_t(maxs.x, mins.y, mins.z),
        vector_t(maxs.x, maxs.y, maxs.z),
        vector_t(mins.x, maxs.y, maxs.z),
        vector_t(mins.x, mins.y, maxs.z),
        vector_t(maxs.x, mins.y, maxs.z)
    };

    //static_cast and neither reinterpret_cast but fucking c style works? wtf.
    auto& transform = (matrix3x4_t&)(entity->renderable_to_world_transform());

    //fix camera warps by local player.
    if (entity && entity == ctx.entities.local_player) {
        auto angles = ctx.interfaces.engine->get_view_angles();

        angles.x = angles.z = 0.0f;

        mathematics::angle_matrix(angles, transform);

        mathematics::matrix_set_column(entity->get_render_origin(), 3, transform);
    }

    auto left = std::numeric_limits< float >::max();
    auto top = std::numeric_limits< float >::max();
    auto right = std::numeric_limits< float >::lowest();
    auto bottom = std::numeric_limits< float >::lowest();

    vector_t screen[8] = {};

    vector_t* vec_out = {};
    
    for (std::size_t i = 0; i < 8; i++) {
        mathematics::vector_transform(points[i], transform, vec_out[i]);
        if (!w2s(vec_out[i], screen[i]))
        {
            return false;
        }

        left = std::min(left, screen[i].x);
        top = std::min(top, screen[i].y);
        right = std::max(right, screen[i].x);
        bottom = std::max(bottom, screen[i].y);
    }

    auto x_ = left;
    auto y_ = top;
    auto w_ = right - left;
    auto h_ = bottom - top;

    x = static_cast<int>(x_);
    y = static_cast<int>(y_);
    w = static_cast<int>(w_);
    h = static_cast<int>(h_);

    return true;
}

color_t c_ctx::tf2_t::get_health_color(int health, int max_health)
{
    if (health > max_health)
        return { 15, 150, 150, 255 };

    const int nHP = std::max(0, std::min(health, max_health));

    const unsigned int nR = static_cast<unsigned int>(std::min((510 * (max_health - nHP)) / max_health, 200));
    const unsigned int nG = static_cast<unsigned int>(std::min((510 * nHP) / max_health, 200));

    return { static_cast<byte>(nR), static_cast<byte>(nG), 0, 255 };
}

bool c_ctx::tf2_t::w2s(const vector_t& origin, vector_t& screen)
{
    const auto screenTransform = [&origin, &screen, this]() -> bool
    {
        const D3DMATRIX& w2sMatrix = w2s_matrix;
        screen.x = w2sMatrix.m[0][0] * origin.x + w2sMatrix.m[0][1] * origin.y + w2sMatrix.m[0][2] * origin.z + w2sMatrix.m[0][3];
        screen.y = w2sMatrix.m[1][0] * origin.x + w2sMatrix.m[1][1] * origin.y + w2sMatrix.m[1][2] * origin.z + w2sMatrix.m[1][3];
        screen.z = 0.0f;

        float w = w2sMatrix.m[3][0] * origin.x + w2sMatrix.m[3][1] * origin.y + w2sMatrix.m[3][2] * origin.z + w2sMatrix.m[3][3];

        if (w < 0.001f)
        {
            screen.x *= 100000;
            screen.y *= 100000;
            return true;
        }

        float invw = 1.f / w;
        screen.x *= invw;
        screen.y *= invw;

        return false;
    };

    if (!screenTransform())
    {
        int iScreenWidth, iScreenHeight;
        ctx.interfaces.engine->get_screen_size(iScreenWidth, iScreenHeight);

        screen.x = (iScreenWidth * 0.5f) + (screen.x * iScreenWidth) * 0.5f;
        screen.y = (iScreenHeight * 0.5f) - (screen.y * iScreenHeight) * 0.5f;

        return true;
    }
    return false;
}
