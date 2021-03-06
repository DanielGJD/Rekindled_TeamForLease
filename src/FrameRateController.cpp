/*!
    \file   FrameRateController.cpp
    \author Sean McGeer
    \date   9/14/15
    \brief
        Contains implementations for the FrameRateController class defined in FrameRateController.h.
    \see FrameRateController.h

    \copyright ęCopyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "FrameRateController.h"

#ifdef FLE_WINDOWS
#include <windows.h>
#else
#include <chrono>
#include <thread>
#endif
#include "Timer.h"
#include <iostream>
// #include "Engine.h"

namespace ForLeaseEngine {

    /*!
    \namespace Modules
    \brief
        The wrapper namespace for every engine module.

        Examples:
            - FrameRateController
            - GameStateManager
            - Debug
    */
    namespace Modules {

        /*!
            Constructor for FrameRateController.  Creates a new instance of FrameRateController, using either a default value or a user-provided one.

            \param framesPerSecond
                The number of frames to draw per second.  Default is 60.

            \param timeScale
                The current time scale.  Default is 1.
        */
        FrameRateController::FrameRateController(int framesPerSecond, double timeScale)
            : FramesPerSecond(framesPerSecond), TimeScale(timeScale), FrameNumber(0) {
                #ifdef FLE_WINDOWS
                FrameTime = 1.0/FramesPerSecond;
                LastFrameTime = 0.0;
                #else
                FrameTime = std::chrono::duration_cast<std::chrono::microseconds>(seconds(1/double(FramesPerSecond))); // Set the frame time
                LastFrameTime = std::chrono::microseconds(0);
                #endif
            }

        /*!
            Start a new frame's timing.
        */
        void FrameRateController::Start() {
            #ifdef FLE_WINDOWS
            QueryPerformanceCounter(&StartTime);
            LARGE_INTEGER frequency;
            QueryPerformanceFrequency(&frequency);
            LARGE_INTEGER frameTicks;
            frameTicks.QuadPart = frequency.QuadPart * FrameTime;

            EndTime.QuadPart = StartTime.QuadPart + frameTicks.QuadPart;
            #else
            StartTime = std::chrono::high_resolution_clock::now();
            EndTime = StartTime + FrameTime;
            #endif

        }

        /*!
            End the current frame's timing, and sleep until the end of the frame's allotted time.
        */
        void FrameRateController::End() {
            ++FrameNumber;

            Timer muhTimer;
            #ifdef FLE_WINDOWS
            LARGE_INTEGER currentTime;
            QueryPerformanceCounter(&currentTime);
            while (currentTime.QuadPart < EndTime.QuadPart)
                QueryPerformanceCounter(&currentTime);

            LARGE_INTEGER frequency;
            QueryPerformanceFrequency(&frequency);
            LastFrameTime = double(currentTime.QuadPart - StartTime.QuadPart) / frequency.QuadPart;
            #else
            std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
            while (currentTime < EndTime)
                currentTime = std::chrono::high_resolution_clock::now();
            LastFrameTime = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - StartTime);
            #endif
            //std::cout << muhTimer.GetTime() << std::endl;
        }

        /*!
            Sleeps for a while.
        */
        void FrameRateController::SleepFor(double time) {
            #ifdef FLE_WINDOWS
            Sleep(time * 1000);
            #else
            std::this_thread::sleep_for(std::chrono::duration<double>(time));
            #endif
        }

        /*!
            Get the time used for the last frame as a double representing seconds.
            This is scaled according to the current TimeScale.
        */
        double FrameRateController::GetDt() {
            return GetUnscaledDt() * TimeScale;
        }

        double FrameRateController::GetUnscaledDt() {
            #ifdef FLE_WINDOWS
            return LastFrameTime;
            #else
            return std::chrono::duration_cast<seconds>(LastFrameTime).count();
            #endif
        }

        /*!
            Get the time allotted for each frame.
        */
        double FrameRateController::GetFrameTime() {
            #ifdef FLE_WINDOWS
            return FrameTime;
            #else
            return std::chrono::duration_cast<seconds>(FrameTime).count();
            #endif
        }

        /*!
            Set the time scale.

            \param timeScale
                The timescale to set.
        */
        void FrameRateController::TimeScaling(double timeScale) {
            TimeScale = timeScale;
        }

        /*!
            Get the current time scale.

            \return
                TimeScale, as a double.
        */
        double FrameRateController::TimeScaling() {
            return TimeScale;
        }

    } // Modules

} // ForLeaseEngine
