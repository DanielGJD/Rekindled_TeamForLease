/*!
    \file   Input.h
    \author Christopher Hudson
    \date   09/22/15

    \brief
        Defines a class to process OS events

    \see Input.cpp
*/
#ifndef INPUT_H
#define INPUT_H

#include "EventDispatcher.h"

namespace ForLeaseEngine
{
    /*!
        \class Input

        \brief
            Class that processes OS events
    */
    class Input {
        public:
            Input();
            ~Input();


            void ProcessAllInput();
            //void ProcessInput(int events);

            //! For testing only, do not use
            EventDispatcher* dispatcher;
        private:
    };
}

#endif // INPUT_H
