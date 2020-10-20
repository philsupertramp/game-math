#pragma once


class Test
{
public:
    bool extended = false;
    virtual void run(bool _extended) = 0;
};
