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

    Component::Component(ComponentID id) : ID(id) {}

    ComponentID Component::GetID() { return ID; }

} // ForLeaseEngine