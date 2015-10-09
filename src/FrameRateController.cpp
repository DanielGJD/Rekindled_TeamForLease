/*!
    \file   FrameRateController.cpp
    \author Sean McGeer
    \date   9/14/15
    \brief
        Contains implementations for the FrameRateController class defined in FrameRateController.h.
    \see FrameRateController.h
*/

#include "FrameRateController.h"
#include <windows.h>
// #include "Engine.h"
// #include <chrono>

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
            
            \param parent
                A reference to the parent engine.  Saved in Parent.
            \param framesPerSecond
                The number of frames to draw per second.  Default is 60.
        */
        FrameRateController::FrameRateController(int framesPerSecond)
            : FramesPerSecond(framesPerSecond) {
                // FrameTime = std::chrono::duration_cast<std::chrono::microseconds>(seconds(1/double(FramesPerSecond))); // Set the frame time
                FrameTime = 1.0/FramesPerSecond;
                // LastFrameTime = std::chrono::microseconds(0);
                LastFrameTime = 0.0;
            }

        /*!
            Start a new frame's timing.
        */
        void FrameRateController::Start() {
            // StartTime = std::chrono::high_resolution_clock::now();
            QueryPerformanceCounter(&StartTime);
            LARGE_INTEGER frequency;
            QueryPerformanceFrequency(&frequency);
            LARGE_INTEGER frameTicks;
            frameTicks.QuadPart = frequency.QuadPart * FrameTime;
            
            EndTime.QuadPart = StartTime.QuadPart + frameTicks.QuadPart;
        }

        /*!
            End the current frame's timing, and sleep until the end of the frame's allotted time.
        */
        void FrameRateController::End() {
            // std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();

            LARGE_INTEGER currentTime;
            QueryPerformanceCounter(&currentTime);
            
            //! \bug Polling the time each second is _bad_
            while (currentTime.QuadPart < EndTime.QuadPart)
                QueryPerformanceCounter(&currentTime);

            // LastFrameTime = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - StartTime);
            LARGE_INTEGER frequency;
            QueryPerformanceFrequency(&frequency);
            LastFrameTime = double(currentTime.QuadPart - StartTime.QuadPart) / frequency.QuadPart;
        }

        /*!
            Get the time used for the last frame as a double representing seconds.
        */
        // double FrameRateController::GetDt() { return std::chrono::duration_cast<seconds>(LastFrameTime).count(); }
        double FrameRateController::GetDt() { return LastFrameTime; }
        
        /*!
            Get the time allotted for each frame.
        */
        // double FrameRateController::GetFrameTime() { return std::chrono::duration_cast<seconds>(FrameTime).count(); }
        double FrameRateController::GetFrameTime() { return FrameTime; }
    
    } // Modules

} // ForLeaseEngine
