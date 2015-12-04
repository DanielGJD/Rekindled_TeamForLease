/*!
    \file   HowToPlay.h
    \author Sean McGeer
    \date   12/3/15
    \brief
        Defines the how to play screen state.
    \see State.h

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef HOW_TO_PLAY_H
#define HOW_TO_PLAY_H

#include "Engine.h"
#include "Entity.h"
#include "Event.h"
#include "LevelComponentsInclude.h"
#include "State.h"

class HowToPlay : public ForLeaseEngine::State {
public:
    HowToPlay();
    void Load();
    void Initialize();
    void Update();
    void Deinitialize();
    void Unload();
};

#endif // HOW_TO_PLAY_H