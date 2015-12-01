/*!
    \file   RaycastTest.h
    \author Sean McGeer

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef RAYCAST_TEST_H
#define RAYCAST_TEST_H

#include "Engine.h"
#include "Entity.h"
#include "Event.h"
#include "LevelComponentsInclude.h"
#include "State.h"

class RaycastTest : public ForLeaseEngine::State {
public:
    RaycastTest();
    void Load();
    void Initialize();
    void Update();
    void Deinitialize();
    void Unload();
private:
    int Health;
};

#endif // RAYCAST_TEST_H
