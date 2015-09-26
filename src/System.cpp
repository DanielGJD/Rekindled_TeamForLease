/*!
    \file   System.cpp
    \author Sean McGeer
    \date   9/24/15
    \brief
        Implements the System class.
    \see System.h
*/

#include "System.h"
#include "Engine.h"

namespace ForLeaseEngine {

    System::System(Engine& owner, ComponentType componentMask)
        : Owner(owner), ComponentMask(componentMask) {}

    bool System::CheckEntityCompatibility(Entity* entity) {
        return static_cast<bool>(entity->GetComponentMask() & ComponentMask);
    }

} // ForLeaseEngine
