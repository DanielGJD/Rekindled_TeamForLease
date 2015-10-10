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

    Timer::Timer(std::string name) : Name(name) {
        Reset();
    }

    void Timer::Reset() {
        #ifdef WIN32
        QueryPerformanceCounter(&StartTime);
        #else
        StartTime = std::chrono::high_resolution_clock::now();
        #endif
    }
    
    double Timer::GetTime() {
        #ifdef WIN32
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
