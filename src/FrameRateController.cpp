/*!
    \file   FrameRateController.cpp
    \author Sean McGeer
    \date   9/14/15
    \brief
        Contains implementations for the FrameRateController class defined in FrameRateController.h.
    \see FrameRateController.h
*/

#include "FrameRateController.h"
#include <chrono>
#include <thread>

namespace ForLeaseEngine {
    
    /*!
        Constructor for FrameRateController.  Creates a new instance of FrameRateController, using either a default value or a user-provided one.
        
        \param framesPerSecond
            The number of frames to draw per second.  Default is 60.
    */
    FrameRateController::FrameRateController(int framesPerSecond)
        : FramesPerSecond(framesPerSecond) {
            FrameTime = std::chrono::microseconds(1000000/FramesPerSecond); // Set the frame time
            LastFrameTime = std::chrono::microseconds(0);
        }

    /*!
        Start a new frame's timing.
    */
    void FrameRateController::Start() {
        StartTime = std::chrono::high_resolution_clock::now();
        EndTime = StartTime + FrameTime;
    }

    /*!
        End the current frame's timing, and sleep until the end of the frame's allotted time.
    */
    void FrameRateController::End() {
        std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();

        //! \bug Polling the time each second is _bad_
        while (currentTime < EndTime)
            currentTime = std::chrono::high_resolution_clock::now();

        LastFrameTime = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - StartTime);
    }

    /*!
        Get the time used for the last frame as a double representing milliseconds.
    */
    double FrameRateController::GetDt() { return LastFrameTime.count() / double(1000); }

    /*!
        Get the time allotted for each frame.
    */
    double FrameRateController::GetFrameTime() { return FrameTime.count() / double(1000); }

} // ForLeaseEngine
