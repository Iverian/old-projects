#pragma once

#define dbgout (__dbgout__() << "[DEBUG]: (" << __FILE__ << "; " << __LINE__ << ")")

struct __dbgout__ {
};

template <class T>
__dbgout__ operator<<(__dbgout__ os, const T& x)
{

#ifndef NDEBUG
#include <iostream>
    std::cerr << x;
#endif

    return os;
}