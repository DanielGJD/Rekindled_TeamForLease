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

#ifdef WIN32
#include <windows.h>
#else
#include <chrono>
#endif

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
            #ifndef WIN32
            typedef std::chrono::duration<double, std::chrono::seconds::period> seconds;
            #endif

            Timer(std::string name = "Timer");
            void Reset();
            double GetTime();
            std::string GetName();
        private:
            #ifdef WIN32
            LARGE_INTEGER StartTime;
            #else
            std::chrono::high_resolution_clock::time_point StartTime;
            #endif
            
            std::string Name;
    };

} // ForLeaseEngine

#endif