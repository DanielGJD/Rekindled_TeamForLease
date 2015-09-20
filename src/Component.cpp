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

    /*!
        Overload for the | operator, to allow the user to OR two ComponentIDs.
        \param lhs
            A constant reference to the ComponentID on the left.
        \param rhs
            A constant reference to the ComponentID on the right.
        \return
            A ComponentID that is the result of the OR on the two ComponentIDs.
    */
    ComponentID operator|(const ComponentID& lhs, const ComponentID& rhs) {
        return static_cast<ComponentID>(static_cast<unsigned long>(lhs)
            | static_cast<unsigned long>(rhs));
    }

    /*!
        Oerload for the & operator, to allow the user to AND two ComponentIDs.
        \param lhs
            A constant reference to the ComponentID on the left.
        \param rhs
            A constant reference to the ComponentID on the right.
        \return
            A ComponentID that is the result of the AND on the two ComponentIDs.
    */
    ComponentID operator&(const ComponentID& lhs, const ComponentID& rhs) {
        return static_cast<ComponentID>(static_cast<unsigned long>(lhs)
            & static_cast<unsigned long>(rhs));
    }

    /*!
        Overload for the |= operator.  Simply calls | on lhs and rhs and stores
        it back into lhs.
        \param lhs
            A reference to the ComponentID we want to OR with rhs and assign the
            result to.
        \param rhs
            A constant reference to the ComponentID on the right.
        \return
            A reference to the modified lhs.
    */
    ComponentID& operator|=(ComponentID& lhs, const ComponentID& rhs) {
        lhs = lhs | rhs;
        return lhs;
    }

    /*!
        Constructor for a new Component.
        \param id
            A ComponentID that we want to assign to this Component.
    */
    Component::Component(ComponentID id) : ID(id) {}

    /*!
        Returns the ID of this component, which lets the user know the type of
        Component that this is.
        \return
            A ComponentID that determines this Component's type.
    */
    ComponentID Component::GetID() { return ID; }

} // ForLeaseEngine
