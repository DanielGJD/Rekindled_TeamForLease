/*!
    \file   LevelComponent.cpp
    \author Sean McGeer
    \date   9/24/15
    \brief
        Implements the LevelComponent class.
    \see LevelComponent.h
*/

#include "LevelComponent.h"
#include "Engine.h"

namespace ForLeaseEngine {

    LevelComponent::LevelComponent(Engine& owner, ComponentType componentMask)
        : Owner(owner), ComponentMask(componentMask) {}

    bool LevelComponent::CheckEntityCompatibility(Entity* entity) {
        return static_cast<bool>(entity->GetComponentMask() & ComponentMask);
    }

} // ForLeaseEngine
