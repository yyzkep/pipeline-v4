//
// Created by drev on 15/08/2025.
//

#pragma once
#include "client_networkable.hpp"
#include "client_renderable.hpp"
#include "client_thinkable.hpp"
#include "c_handle.hpp"

class IClientAlphaProperty;
class base_entity;
class client_entity;

class i_collideable
{
public:
    virtual void pad0();
    virtual const vector_t& mins() const;
    virtual const vector_t& maxs() const;
};

class client_unknown : public handle_entity
{
public:
    virtual i_collideable* get_collideable() = 0;
    virtual client_networkable* get_client_networkable() = 0;
    virtual client_renderable* get_client_renderable() = 0;
    virtual client_entity* get_client_entity() = 0;
    virtual base_entity* get_base_entity() = 0;
    virtual client_thinkable* get_client_thinkable() = 0;
    virtual IClientAlphaProperty* get_client_alpha_property() = 0;
};