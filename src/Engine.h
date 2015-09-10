/*!
    \file   Engine.h
    \author Sean McGeer
    \date   9/9/15
    \brief
        Defines the Engine class, which is an instance of the For Lease Engine.
    \see Engine.cpp
*/

#ifndef ENGINE_H
#define ENGINE_H

namespace ForLeaseEngine {

    /*!
        \class Engine
        \brief
            The base of the game engine.
    */
    class Engine {

        public:
            Engine();
            Engine(int resolutionX, int resolutionY, int frameRate);

        private:
            //! Width of the screen
            int resolutionX;
            //! Height of the screen
            int resolutionY;
            //! Frame rate of the game
            int frameRate;

    };

} // ForLeaseEngine

#endif