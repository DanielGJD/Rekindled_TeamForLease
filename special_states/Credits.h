/*!
    \file   Credits.h
    \author Sean McGeer
    \date   4/05/16
    \brief
        Defines the credits screen state.
    \see State.h

    \copyright ©Copyright 2016 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef CREDITS_H
#define CREDITS_H

#include "Engine.h"
#include "Entity.h"
#include "Event.h"
#include "LevelComponentsInclude.h"
#include "State.h"

class Credits : public ForLeaseEngine::State {
public:
    Credits();
    void Load();
    void Initialize();
    void Update();
    void Deinitialize();
    void Unload();
    std::string MainMenuBGM;
private:
    int Health;
};

#endif // CREDITS_H
