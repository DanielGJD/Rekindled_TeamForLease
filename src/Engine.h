/*!
    \file   Engine.h
    \author Sean McGeer
    \date   9/9/15
    \brief
        Defines the Engine class.
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
            void Run();
        private:
            //! Width of the screen
            int ResolutionX;
            //! Height of the screen
            int ResolutionY;
            //! Frame rate of the game
            int FrameRate;

    };

} // ForLeaseEngine

#endif
