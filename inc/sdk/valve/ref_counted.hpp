//
// Created by drev on 15/08/2025.
//

#pragma once

class i_ref_counted
{
public:
    virtual int add_ref() = 0;
    virtual int release() = 0;
};