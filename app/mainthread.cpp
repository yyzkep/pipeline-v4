#include <iostream>
#include "../inc/link.hpp"
#include "hooks/hooks.hpp"
#include "../inc/crashlog.hpp" // did this so that i dont have to attach a debugger.

static bool close_from_dll = false;

DWORD WINAPI main_thread(LPVOID lpParam)
{
    //GameUI is the last module to be loaded in tf2.
    crashlog.initialize();
    while (!GetModuleHandleA("GameUI.dll"))
        Sleep(100);

    //alloc console
    AllocConsole();
    FILE* fDummy;
    freopen_s(&fDummy, "CONOUT$", "w", stdout);
    freopen_s(&fDummy, "CONOUT$", "w", stderr);
    freopen_s(&fDummy, "CONIN$", "r", stdin);
    SetConsoleTitleA("debug");

    //grab interfaces
    ctx.tf2.get_interfaces();

    //load every hook and hook it ofc ofc
    hookmgr.load_hooks();

    //sleep to make sure we can tab back in time to hear it lol
    Sleep(2500);

    //log that we loaded
    utilities::log("[hack] locked tf in");

    //play audio saying that we loaded
    ctx.interfaces.engine->client_cmd_unrestricted("play vo/items/wheatley_sapper/wheatley_sapper_attached14.mp3"); //baller refrence to legendary cheat. 

    //we're done with our shit go sleep
    while (!GetAsyncKeyState(VK_DELETE)) Sleep(100);

    close_from_dll = true;

    //hehe
    ctx.interfaces.engine->client_cmd_unrestricted("play vo/items/wheatley_sapper/wheatley_sapper_hacked02.mp3");

    //remove the fucking hook
    hookmgr.remove_hooks();

    FreeConsole();

    crashlog.unload();

    FreeLibraryAndExitThread(static_cast<HMODULE>(lpParam), EXIT_SUCCESS);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    DisableThreadLibraryCalls(hinstDLL);
    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        if (const auto hMainThread = CreateThread(nullptr, 0, main_thread, hinstDLL, 0, nullptr)) {
            CloseHandle(hMainThread);
        }
    }

    if (fdwReason == DLL_PROCESS_DETACH) {
        if (!close_from_dll) {
            //only do this if we arennt detaching too early, to not cause program crash due to access violation.
            if(ctx.interfaces.engine)
                ctx.interfaces.engine->client_cmd_unrestricted("play vo/items/wheatley_sapper/wheatley_sapper_hacked02.mp3");
            hookmgr.remove_hooks();
            crashlog.unload();
            FreeConsole();
            FreeLibraryAndExitThread(hinstDLL, EXIT_SUCCESS);
        }
    }

    return TRUE;
}