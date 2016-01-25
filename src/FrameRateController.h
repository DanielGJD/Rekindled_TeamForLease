/*!
    \file   FrameRateController.h
    \author Sean McGeer
    \date   9/14/15
    \brief
        Defines the FrameRateController class.
    \see FrameRateController.cpp

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef FRAMERATECONTROLLER_H
#define FRAMERATECONTROLLER_H

#include "Platforms.h"

#ifdef FLE_WINDOWS
#include <windows.h>
#else
#include <chrono>
#endif


namespace ForLeaseEngine {

    // class Engine;

    namespace Modules {

        /*!
            \class FrameRateController
            \brief
                Handles elapsed timing, as well as sleeping to maintain a stable framerate.
        */
        class FrameRateController {

            public:
                #ifndef FLE_WINDOWS
                typedef std::chrono::duration<double, std::chrono::seconds::period> seconds;
                #endif
                
                FrameRateController(int framesPerSecond = 60, double timeScale = 1);

                void Start();
                void End();

                void SleepFor(double time);

                double GetDt();
                double GetUnscaledDt();

                double GetFrameTime();

                void TimeScaling(double timeScale);
                double TimeScaling();
            private:
                //! A reference to the engine
                // Engine& Parent;

                //! Frames per second
                int FramesPerSecond;
                double TimeScale;

                //! The time allotted for each frame -- 1 / FramesPerSecond
                #ifdef FLE_WINDOWS
                double FrameTime;
                #else
                std::chrono::microseconds FrameTime;
                #endif

                //! The start time of the current frame
                #ifdef FLE_WINDOWS
                LARGE_INTEGER StartTime;
                #else
                std::chrono::high_resolution_clock::time_point StartTime;
                #endif

                //! The end time of the current frame
                #ifdef FLE_WINDOWS
                LARGE_INTEGER EndTime;
                #else
                std::chrono::high_resolution_clock::time_point EndTime;
                #endif

                //! The time of the last frame, in seconds
                #ifdef FLE_WINDOWS
                double LastFrameTime;
                #else
                std::chrono::microseconds LastFrameTime;
                #endif

                //! Made private and deactivated because we NEED Parent
                // FrameRateController() = delete;

        };

    } // Modules

} // ForLeaseEngine

#endif
