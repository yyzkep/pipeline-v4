//
// Created by drev on 16/08/2025.
//

#pragma once
#include "client_entity.hpp"

class client_entity;
class client_networkable;
class client_unknown;
class client_renderable;
#include "c_handle.hpp"

class client_entity_list
{
public:
	virtual client_networkable* get_client_networkable(int entnum) = 0;
	virtual client_networkable* get_client_networkable_from_handle(base_handle hEnt) = 0;
	virtual client_unknown* get_client_unknown_from_handle(base_handle hEnt) = 0;
	virtual client_entity* get_client_entity(int entnum) = 0;
	virtual client_entity* get_client_entity_from_handle(base_handle hEnt) = 0;
	virtual int	number_of_entities(bool bIncludeNonNetworkable) = 0;
	virtual int	get_highest_entity_index(void) = 0;
	virtual void set_max_entities(int maxents) = 0;
	virtual int	get_max_entities() = 0;
};