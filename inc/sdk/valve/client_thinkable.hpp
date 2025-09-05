#pragma once

class client_unknown;
class CClientThinkHandlePtr;
typedef CClientThinkHandlePtr* ClientThinkHandle_t;

class client_thinkable
{
public:
    virtual client_unknown* get_i_client_unknown() = 0;
    virtual void				client_think() = 0;
    virtual ClientThinkHandle_t	get_think_handle() = 0;
    virtual void				set_think_handle(ClientThinkHandle_t hThink) = 0;
    virtual void				release() = 0;
};