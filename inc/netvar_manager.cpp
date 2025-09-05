//
// Created by drev on 30/08/2025.
//

#include "netvar_manager.hpp"

#include "link.hpp"


int c_net_vars::get_offset(RecvTable* Table, const char* Name)
{
    for (int i = 0; i < Table->PropsCount; i++)
    {
        auto* Prop = &Table->Props[i];

        if (std::string_view(Prop->VarName).compare(Name) == 0) // if the prop matches the name. return the offset
        {
            return Prop->Offset;
        }

        if (auto DataTable = Prop->DataTable)
        {
            auto Offset = get_offset(DataTable, Name); // or else recursively loop through data table in prop

            if (Offset)
            {
                return Offset + Prop->Offset;
            }
        }
    }

    return 0;
}

int c_net_vars::get_net_var(const char* Class, const char* NetVar)
{
    const auto& Classes = ctx.interfaces.client->get_all_classes();

    for (auto Current = Classes; Current; Current = Current->pNext)
    {
        if (std::string_view(Class).compare(Current->pNetworkName) == 0)
        {
            return get_offset(Current->pRecvTable, NetVar);
        }
    }

    return 0;
}

RecvProp* c_net_vars::get_net_var_prop(RecvTable* table, const char* name)
{
    // loop through props in RecvTable
    for (int i = 0; i < table->PropsCount; i++)
    {
        RecvProp* Prop = &table->Props[i];

        if (!Prop)
        {
            continue;
        }

        if (std::string_view(Prop->VarName).compare(name) == 0) // if it matches the name return the prop. if it doesnt. try harder in the datatable of the prop
        {
            return Prop;
        }

        if (auto datatable = Prop->DataTable)
        {
            RecvProp* SubProp = get_net_var_prop(datatable, name);

            if (SubProp && std::string_view(SubProp->VarName).compare(name) == 0)
            {
                return SubProp;
            }
        }
    }

    return nullptr;
}

RecvProp* c_net_vars::find_net_var_prop(const char* classname, const char* netvar)
{
    auto classes = ctx.interfaces.client->get_all_classes();

    for (auto current = classes; current; current = current->pNext)
    {
        if (std::string_view(classname).compare(current->pNetworkName) == 0)
        {
            return get_net_var_prop(current->pRecvTable, netvar);
        }
    }

    return nullptr;
}