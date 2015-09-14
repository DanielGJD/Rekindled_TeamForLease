/*!
    \file   FrameRateController.h
    \author Sean McGeer
    \date   9/14/15
    \brief
        Defines the FrameRateController class.
    \see FrameRateController.cpp
*/

#ifndef FRAMERATECONTROLLER_H
#define FRAMERATECONTROLLER_H

#include <chrono>

namespace ForLeaseEngine {

    /*!
        \class FrameRateController
        \brief
            Handles elapsed timing, as well as sleeping to maintain a stable framerate.
    */
    class FrameRateController {

        public:
            FrameRateController(int framesPerSecond = 60);

            void Start();
            void End();

            double GetDt();
            double GetFrameTime();
        private:
            //! Frames per second
            int FramesPerSecond;
            //! The time allotted for each frame -- 1 / FramesPerSecond
            std::chrono::microseconds FrameTime;
            //! The start time of the current frame
            std::chrono::high_resolution_clock::time_point StartTime;
            //! The end time of the current frame
            std::chrono::high_resolution_clock::time_point EndTime;
            //! The time of the last frame, in microseconds
            std::chrono::microseconds LastFrameTime;

    };

} // ForLeaseEngine

#endif
