//
// Created by drev on 30/08/2025.
//
#pragma once
#include "link.hpp"

//this is where we will do our hook calling for the hooks...
class c_hooks {
public:
    void init();
    void shutdown();
public:
    SafetyHookInline m_present{};
    SafetyHookInline m_reset{};
    SafetyHookInline m_paint{};
    SafetyHookInline m_createmove{}; //todo
    SafetyHookInline m_lockcursor{};
    SafetyHookInline m_getusrcmd{};
    SafetyHookInline m_validateusrcmd{};

    WNDPROC m_original = NULL;
    HWND m_window = NULL;
};

inline c_hooks hooks = {};

//loading the hooks
class c_hook_manager {
public:
    void load_hooks();
    void remove_hooks();
};

inline c_hook_manager hookmgr = {};