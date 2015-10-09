/*!
    \file   Timer.cpp
    \author Sean McGeer
    \date   10/08/15
    \brief
        Contains implementations for the Timer class defined in Timer.h.
    \see Timer.h
*/

#include "Timer.h"

namespace ForLeaseEngine {

    Timer::Timer(std::string name)
        : Name(name), StartTime(std::chrono::high_resolution_clock::now()) {}

    void Timer::Reset() { StartTime = std::chrono::high_resolution_clock::now(); }

    double Timer::GetTime() {
        return seconds(std::chrono::high_resolution_clock::now() - StartTime).count();
    }

    std::string Timer::GetName() {
        return Name;
    }

}
