//
// Created by drev on 30/08/2025.
//

#include "hooks.hpp"
#include <wrl.h>

#include "../features/esp/esp.hpp"
#include "..\features\menu\menu.hpp"
#include "..\features\cfg.hpp"

template <typename T>
using com_ptr = Microsoft::WRL::ComPtr<T>;

HRESULT __fastcall hkPresent(IDirect3DDevice9 *pDevice, const RECT *pSource,
                             const RECT *pDestination, HWND hWindow, const RGNDATA *pDirtyRegion)
{
    ctx.renderer.state_manager.setup(pDevice);

    // imgui does set all of the previous states set here,
    // but i c&pasted one more state that does the color correction
    // so that checks out
    ctx.renderer.state_manager.setup_states(pDevice);

    ctx.renderer.state_manager.start();

    features::menu.draw_menu(); //yey

    auto *list = ImGui::GetBackgroundDrawList();
    ctx.renderer.render_queue.render(list);

    ctx.renderer.state_manager.end();
    ctx.renderer.state_manager.backup_states(pDevice);

    return hooks.m_present.fastcall<HRESULT>(pDevice, pSource, pDestination, hWindow, pDirtyRegion);
}

HRESULT __fastcall hkReset(IDirect3DDevice9 *device, D3DPRESENT_PARAMETERS *params)
{
    ImGui_ImplDX9_InvalidateDeviceObjects();
    auto return_result = hooks.m_reset.fastcall<HRESULT>(device, params);
    ImGui_ImplDX9_CreateDeviceObjects();
    return return_result;
}

void __fastcall hkPaint(void *rcx, paint_mode_t mode)
{
    ctx.tf2.grab_view_matrix();

    hooks.m_paint.fastcall<void>(rcx, mode);

    if (mode & paint_mode_t::PAINT_UIPANELS)
    {
        ctx.renderer.render_queue.string(FONTS::FONT_MENU, static_cast<int>(ctx.interfaces.client->get_screen_width() * 0.5f),
                                         5, color_t(255, 255, 255), horizontal, "pipeline v4 | 'rei made you' fuck off");

        // render esp
        features::esp.draw();

        ctx.renderer.render_queue.swap_commands();
    }
}

void __fastcall hkCHLCCreateMove(base_client_dll *rcx, int sequence_number, float input_sample_frametime, bool active)
{
    hooks.m_createmove.fastcall<void>(rcx, sequence_number, input_sample_frametime, active);

    ctx.entities.local_player = ctx.interfaces.entity_list->get_client_entity(ctx.interfaces.engine->get_local_player())->as<base_player>();
    ctx.entities.local_weapon = ctx.interfaces.entity_list->get_client_entity_from_handle(ctx.entities.local_player->active_weapon())->as<base_combat_weapon>();

    user_cmd* cmd = &ctx.interfaces.input->get_cmds()[sequence_number % MULTIPLAYER_BACKUP];


    ctx.interfaces.pred->update(ctx.interfaces.state->m_nDeltaTick, ctx.interfaces.state->m_nDeltaTick > 0, ctx.interfaces.state->last_command_ack, ctx.interfaces.state->lastoutgoingcommand + ctx.interfaces.state->chokedcommands);
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LONG __stdcall hkWndProc(HWND window, UINT msg, WPARAM wparam, LPARAM lparam) {
    if (cfg::menu_open) {
        ImGui_ImplWin32_WndProcHandler(window, msg, wparam, lparam);

        if ((ImGui::GetIO().WantTextInput) && WM_KEYFIRST <= msg && msg <= WM_KEYLAST)
        {
            ctx.interfaces.input_system->reset_input_state();
            return 1;
        }

        if (WM_MOUSEFIRST <= msg && msg <= WM_MOUSELAST)
            return 1;
    }

    return CallWindowProcA(hooks.m_original, window, msg, wparam, lparam);
}

void __fastcall hkLockCursor(void* rcx) {
    if (cfg::menu_open)
        return ctx.interfaces.surface->lock_cursor();

    hooks.m_lockcursor.fastcall<void>(rcx);
}


void c_hooks::init()
{
    {
        com_ptr<IDirect3D9> d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
        auto window = FindWindowA("Valve001", nullptr);
        D3DPRESENT_PARAMETERS params{
            .BackBufferFormat = D3DFMT_UNKNOWN,
            .SwapEffect = D3DSWAPEFFECT_DISCARD,
            .Windowed = TRUE,
            .EnableAutoDepthStencil = TRUE,
            .AutoDepthStencilFormat = D3DFMT_D16,
            .PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT,
        };

        com_ptr<IDirect3DDevice9> d3d_device{};

        d3d9->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, window, D3DCREATE_HARDWARE_VERTEXPROCESSING, &params,
                           &d3d_device);

        m_present = safetyhook::create_inline(utilities::find_vfunc(d3d_device.Get(), 17), reinterpret_cast<void *>(hkPresent));

        m_reset = safetyhook::create_inline(utilities::find_vfunc(d3d_device.Get(), 16), reinterpret_cast<void *>(hkReset));
    }

    /*
                    the benefits of hooking IVEngineVGUI::Paint over FSN::FRAME_START is that paint is called way after frame start,
                    which is literally the start of the frame...
                    paint gives us latest data that is isnt rendered in frame_start.
    */

    m_paint = safetyhook::create_inline(utilities::find_vfunc(ctx.interfaces.vgui, 14), reinterpret_cast<void *>(hkPaint));

    m_createmove = safetyhook::create_inline(utilities::find_vfunc(ctx.interfaces.client, 21), reinterpret_cast<void*>(hkCHLCCreateMove));

    m_lockcursor = safetyhook::create_inline(utilities::find_vfunc(ctx.interfaces.surface, 62), reinterpret_cast<void*>(hkLockCursor));

    m_window = FindWindowA("Valve001", nullptr);

    m_original = reinterpret_cast<WNDPROC>(SetWindowLongPtr(m_window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(hkWndProc)));
}

void c_hooks::shutdown()
{
    // i love safetyhook
    m_present = {};
    m_reset = {};
    m_paint = {};
    m_createmove = {};
    m_lockcursor = {};

    SetWindowLongPtr(m_window, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(m_original));
}

void c_hook_manager::load_hooks()
{
    hooks.init();
}
void c_hook_manager::remove_hooks()
{
    hooks.shutdown();
}