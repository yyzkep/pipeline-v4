#include <iostream>
#include "../inc/link.hpp"
#include "hooks/hooks.hpp"

DWORD WINAPI main_thread(LPVOID lpParam)
{
    MessageBoxA(0, "wtf am i doing wrong", "debug", 0);
    while (!GetModuleHandleA("GameUI.dll"))
        Sleep(100);

    //grab interfaces
    ctx.tf2.get_interfaces();

    //hook
    hookmgr.load_hooks();

    while (!GetAsyncKeyState(VK_DELETE)) Sleep(100);

    hookmgr.remove_hooks();

    FreeConsole();

    FreeLibraryAndExitThread(static_cast<HMODULE>(lpParam), EXIT_SUCCESS);
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    DisableThreadLibraryCalls(hinstDLL);
    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        //alloc console
        AllocConsole();
        FILE* fDummy;
        freopen_s(&fDummy, "CONOUT$", "w", stdout);
        freopen_s(&fDummy, "CONOUT$", "w", stderr);
        freopen_s(&fDummy, "CONIN$",  "r", stdin);
        SetConsoleTitleA("debug");

        if (const auto hMainThread = CreateThread(nullptr, 0, main_thread, hinstDLL, 0, nullptr)) {
            CloseHandle(hMainThread);
        }
    }

    return TRUE;
}