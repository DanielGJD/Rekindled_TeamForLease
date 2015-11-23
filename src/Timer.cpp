/*!
    \file   Timer.cpp
    \author Sean McGeer
    \date   10/08/15
    \brief
        Contains implementations for the Timer class defined in Timer.h.
    \see Timer.h

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "Timer.h"

namespace ForLeaseEngine {

    Timer::Timer(std::string name) : Name(name) {
        Reset();
    }

    void Timer::Reset() {
        #ifdef FLE_WINDOWS
        QueryPerformanceCounter(&StartTime);
        #else
        StartTime = std::chrono::high_resolution_clock::now();
        #endif
    }
    
    double Timer::GetTime() {
        #ifdef FLE_WINDOWS
        LARGE_INTEGER current;
        QueryPerformanceCounter(&current);
        LARGE_INTEGER ticks;
        ticks.QuadPart = current.QuadPart - StartTime.QuadPart;
        double seconds = double(ticks.QuadPart);
        LARGE_INTEGER frequency;
        QueryPerformanceFrequency(&frequency);
        seconds /= frequency.QuadPart;
        
        return seconds;
        #else
        return seconds(std::chrono::high_resolution_clock::now() - StartTime).count();
        #endif
    }

    std::string Timer::GetName() {
        return Name;
    }

}
