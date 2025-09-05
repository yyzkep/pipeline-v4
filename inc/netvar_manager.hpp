//
// Created by drev on 30/08/2025.
//

#pragma once
#include "netvar_registry.hpp"
#include "utils.hpp"
#include "sdk/valve/base_client_dll.hpp"

class RecvTable;
class RecvProp;
class c_net_vars
{
public:
    int get_offset(RecvTable* Table, const char* Name);
    int get_net_var(const char* Class, const char* NetVar);

    RecvProp* get_net_var_prop(RecvTable* table, const char* name);
    RecvProp* find_net_var_prop(const char* classname, const char* netvar);
};

inline c_net_vars* net_vars = new c_net_vars;

class networkable_variable_base {
protected:
    int offset;

    int get_netvar_offset(const char* table, const char* name) {
        return net_vars->get_net_var(table, name);
    }

public:
    networkable_variable_base(const char* table, const char* name) {
        offset = get_netvar_offset(table, name);
    }
};

template<typename T>
class networkable_variable : public networkable_variable_base {
    std::string member_name;
    std::string table;
    std::string var;
public:
    networkable_variable(const char* table_, const char* name_, const char* memberName_)
        : networkable_variable_base(table_, name_), member_name(memberName_), table(table_), var(name_)
    {
        netvar_info info;
        info.member = member_name;
        info.table  = table;
        info.var    = var;
        info.type   = typeid(T).name();
        info.offset     = offset;

        g_netvar_registry.register_netvar(info);
    }

    inline T& get(void* instance) const {
        return *reinterpret_cast<T*>(reinterpret_cast<uintptr_t>(instance) + offset);
    }
};

#define netvar(_name, type, table, name) \
networkable_variable<type> _name##_{table, name, #_name}; \
inline type& _name() { return _name##_.get(this); }

#define offset_from(_name, type, table, name, addition) inline type &_name() \
{ \
static int offset = net_vars->get_net_var(table, name); \
return *reinterpret_cast<type *>(reinterpret_cast<usize>(this) + offset + addition); \
}

#define offset_pure(_name, type, offset) inline type &_name() \
{ \
return *reinterpret_cast<type *>(reinterpret_cast<usize>(this) + offset); \
}