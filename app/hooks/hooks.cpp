//
// Created by drev on 30/08/2025.
//

#include "hooks.hpp"
#include <wrl.h>

template <typename T> using com_ptr = Microsoft::WRL::ComPtr<T>;

HRESULT  __fastcall hkPresent(IDirect3DDevice9* pDevice, const RECT* pSource,
    const RECT* pDestination, const RGNDATA* pDirtyRegion) {
    ctx.renderer.state_manager.setup(pDevice);

    //imgui does set all of the previous states set here, 
    // but i c&pasted one more state that does the color correction
    //so that checks out
    ctx.renderer.state_manager.setup_states(pDevice);

    ctx.renderer.state_manager.start();

    ImGui::Begin("aaaa");
    ImGui::End();

    auto* list = ImGui::GetBackgroundDrawList();
    ctx.renderer.render_queue.render(list);
    
    ctx.renderer.state_manager.end();
    ctx.renderer.state_manager.backup_states(pDevice);

    return hooks.m_present.fastcall<HRESULT>(pDevice, pSource, pDestination, pDirtyRegion);
}

HRESULT __fastcall hkReset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* params) {
    ImGui_ImplDX9_InvalidateDeviceObjects();
    auto return_result = hooks.m_reset.fastcall<HRESULT>(device, params);
    ImGui_ImplDX9_CreateDeviceObjects();
    return return_result;
}

void __fastcall hkPaint(void* rcx, paint_mode_t mode) {
    ctx.tf2.grab_view_matrix();

    hooks.m_paint.fastcall<void>(rcx, mode);

    if (mode & paint_mode_t::PAINT_UIPANELS) {
        ctx.renderer.render_queue.string(FONTS::FONT_MENU, static_cast<int>(ctx.interfaces.client->get_screen_width() * 0.5f),
            5, color_t(255,255,255), horizontal, "pipeline v4 | 'rei made you' fuck off");

        ctx.renderer.render_queue.swap_commands();
    }
}


void c_hooks::init() {
    {
        com_ptr<IDirect3D9>   d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
        auto                  window = FindWindowA("Valve001", nullptr);
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

        m_present = safetyhook::create_inline(utilities::find_vfunc(d3d_device.Get(), 17), hkPresent);

        m_reset = safetyhook::create_inline(utilities::find_vfunc(d3d_device.Get(), 16), hkReset);

        d3d_device.Reset();
        d3d9.Reset();
    }
    //the benefits of hooking IVEngineVGUI::Paint over FSN::FRAME_START is that paint is called way after frame start, 
    // which is literally the start of the frame...
    //paint gives us latest data that is isnt rendered in frame_start.
    m_paint = safetyhook::create_inline(utilities::find_vfunc(ctx.interfaces.vgui, 14), hkPaint);
}

void c_hooks::shutdown() {
    //i love safetyhook
    m_present = {};
    m_reset = {};
    m_paint = {};
}

void c_hook_manager::load_hooks()
{
    hooks.init();
}
void c_hook_manager::remove_hooks()
{
    hooks.shutdown();
}