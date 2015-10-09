/*!
    \file   Timer.h
    \author Sean McGeer
    \date   10/08/15
    \brief
        Defines the Timer class.
    \see Timer.cpp
*/

#ifndef TIMER_H
#define TIMER_H

// #include <chrono>
#include <windows.h>
#include <string>

namespace ForLeaseEngine {

    /*!
        \class Timer

        \brief
            Handles timing--essentially a loose wrapper around std::chrono time
            points.
    */
    class Timer {
        public:
            // typedef std::chrono::duration<double, std::chrono::seconds::period> seconds;
            Timer(std::string name = "Timer");
            void Reset();
            double GetTime();
            std::string GetName();
        private:
            // std::chrono::high_resolution_clock::time_point StartTime;
            LARGE_INTEGER StartTime;
            std::string Name;
    };

} // ForLeaseEngine

#endif