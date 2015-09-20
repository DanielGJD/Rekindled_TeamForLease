/*!
    \file   __test.cpp
    \author Sean McGeer
    \date   9/14/15
    \brief
        A test driver for the For Lease Engine.  Feel free to put whatever you
        want in this file to test stuff.
*/

#include <iostream>
#include <iomanip>

#include "Engine.h"
#include "FrameRateController.h"
#include "__test__state.h"

int main() {
    ForLeaseEngine::Engine engine;
    ForLeaseEngine::FrameRateController frc;

    frc.Start();
    frc.End();

    std::cout << frc.GetDt() << std::endl;
    std::cout << frc.GetFrameTime() << std::endl;

    TestState state;

    ForLeaseEngine::Entity * entity = state.AddEntity();

    std::cout << entity->GetComponentMask() << std::endl;

    entity = state.GetEntityByID(entity->GetID());

    ForLeaseEngine::AddComponentsToEntity(
        ForLeaseEngine::ComponentType::Transform
        | ForLeaseEngine::ComponentType::None,
        entity);

    std::cout << entity->GetID() << std::endl;
    std::cout << entity->GetComponentMask() << std::endl;
    std::cout << entity->GetComponent(ForLeaseEngine::ComponentType::Transform)->GetType() << std::endl;

    return 0;
}
