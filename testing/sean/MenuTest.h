/*!
    \file   MenuTest.h
    \author Sean McGeer
    \date   4/03/16
    \brief
        Defines the menu test state.
    \see State.h

    \copyright ©Copyright 2016 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef MENU_TEST_H
#define MENU_TEST_H

#include "Engine.h"
#include "Entity.h"
#include "Event.h"
#include "LevelComponentsInclude.h"
#include "State.h"

class MenuTest : public ForLeaseEngine::State {
    public:
        MenuTest();
        void Load();
        void Initialize();
        void Update();
        void Deinitialize();
        void Unload();

        void OnKeyDown(const ForLeaseEngine::Event* e);
};

#endif // MENU_TEST_H
