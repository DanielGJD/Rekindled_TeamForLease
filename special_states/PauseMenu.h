/*!
    \file   PauseMenu.h
    \author Sean McGeer
    \date   1/24/16
    \brief
        Defines the pause menu screen state.
    \see State.h

    \copyright ©Copyright 2016 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef PAUSE_MENU_H
#define PAUSE_MENU_H

#include "Engine.h"
#include "Entity.h"
#include "Event.h"
#include "LevelComponentsInclude.h"
#include "State.h"

class PauseMenu : public ForLeaseEngine::State {
    public:
        PauseMenu();
        void Load();
        void Initialize();
        void Update();
        void Deinitialize();
        void Unload();

        bool continueFromPause = false;
        bool continueFromFreeze = false;

        void OnKeyDown(const ForLeaseEngine::Event* e);
};

#endif // PAUSE_MENU_H
