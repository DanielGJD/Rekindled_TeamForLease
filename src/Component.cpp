/*!
    \file   Component.cpp
    \author Sean McGeer
    \date   9/18/15
    \brief
        Implements the Component class defined in Component.h.
    \see Component.h
*/

#include "Component.h"

namespace ForLeaseEngine {

    ComponentID operator|(const ComponentID& lhs, const ComponentID& rhs) {
        ComponentID result;
        result = static_cast<ComponentID>(static_cast<unsigned long>(lhs)
            | static_cast<unsigned long>(rhs));
        return result;
    }

    ComponentID& operator|=(ComponentID& lhs, const ComponentID& rhs) {
        lhs = lhs | rhs;
        return lhs;
    }

    Component::Component(ComponentID id) : ID(id) {}

    ComponentID Component::GetID() { return ID; }

} // ForLeaseEngine
