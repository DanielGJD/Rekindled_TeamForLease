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

    // void Timer::Reset() { StartTime = std::chrono::high_resolution_clock::now(); }
    
    void Timer::Reset() { QueryPerformanceCounter(&StartTime); }

    // double Timer::GetTime() {
        // return seconds(std::chrono::high_resolution_clock::now() - StartTime).count();
    // }
    
    double Timer::GetTime() {
        LARGE_INTEGER current;
        QueryPerformanceCounter(&current);
        LARGE_INTEGER ticks;
        ticks.QuadPart = current.QuadPart - StartTime.QuadPart;
        double seconds = double(ticks.QuadPart);
        LARGE_INTEGER frequency;
        QueryPerformanceFrequency(&frequency);
        seconds /= frequency.QuadPart;
        
        return seconds;
    }

    std::string Timer::GetName() {
        return Name;
    }

}
