//
// Created by drev on 15/08/2025.
//

#pragma once
#include "defs.hpp"
#include "client_networkable.hpp"
#include "netchan.hpp"

class client_class;

typedef client_networkable* (*CreateClientClassFn)(int entnum, int serialNum);
typedef client_networkable* (*CreateEventFn)();

//im NOT going to mess with this im fucking want to finish the base porting from sln to cmake ong
class client_class
{
public:
    CreateClientClassFn      pCreateFn;
    CreateEventFn            pCreateEventFn;
    char* pNetworkName;
    RecvTable* pRecvTable;
    client_class* pNext;
    ETFClassID                ClassID;
};