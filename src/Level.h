/*!
    \file   State.h

    \author Sean McGeer

    \date   9/17/15

    \brief
        Defines the Level class.

    \see Level.cpp
    \see State.h

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef LEVEL_H
#define LEVEL_H

#include "State.h"
#include <string>

namespace ForLeaseEngine {

    class Level : public State {
        public:
            Level(std::string file);
            ~Level();
            virtual void Load();
            virtual void Initialize();
            virtual void Update();
            virtual void Deinitialize();
            virtual void Unload();
        private:
            std::string File;
    };

}

#endif // LEVEL_H
