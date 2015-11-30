/*!
    \file   LevelComponent.cpp
    \author Sean McGeer
    \date   9/24/15
    \brief
        Implements the LevelComponent class.
    \see LevelComponent.h

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "LevelComponent.h"
#include "State.h"

namespace ForLeaseEngine {

    LevelComponent::LevelComponent(State& owner, ComponentType componentMask)
        : Owner(owner), ComponentMask(componentMask) {}

    bool LevelComponent::CheckEntityCompatibility(Entity* entity) {
        return static_cast<bool>(entity->GetComponentMask() & ComponentMask);
    }

} // ForLeaseEngine
