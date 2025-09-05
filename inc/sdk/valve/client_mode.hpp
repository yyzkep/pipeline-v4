//
// Created by drev on 15/08/2025.
//

#pragma once
#include "utils.hpp"

class base_hud_chat
{
public:
    void chat_printf(int Index, int Filter, const char* Fmt, ...)
    {
        return utilities::find_vfunc<19, void, int, int, const char*>(this, Index, Filter, Fmt);

    }

    void start_message_mode(int MessageModeType)
    {
        return utilities::find_vfunc<20, void, int>(this, MessageModeType);
    }
};

class client_mode_shared
{
private:
    char m_szPad[28];
public:
    base_hud_chat* m_pChatElement;
};