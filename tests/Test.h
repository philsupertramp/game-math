#pragma once

#include <cassert>
#include <iostream>


class Test
{
public:
    bool extended                    = false;
    virtual void run(bool _extended) = 0;
};
