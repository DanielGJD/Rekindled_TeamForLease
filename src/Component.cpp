/*!
    \file   Component.cpp
    \author Sean McGeer
    \date   9/18/15
    \brief
        Implements the Component class defined in Component.h.
    \see Component.h
*/

#include "Component.h"
#include "Entity.h"

namespace ForLeaseEngine {

    /*!
        Overload for the | operator, to allow the user to OR two ComponentTypes.
        \param lhs
            A constant reference to the ComponentType on the left.
        \param rhs
            A constant reference to the ComponentType on the right.
        \return
            A ComponentType that is the result of the OR on the two ComponentTypes.
    */
    ComponentType operator|(const ComponentType& lhs, const ComponentType& rhs) {
        return static_cast<ComponentType>(static_cast<unsigned long>(lhs)
            | static_cast<unsigned long>(rhs));
    }

    /*!
        Oerload for the & operator, to allow the user to AND two ComponentTypes.
        \param lhs
            A constant reference to the ComponentType on the left.
        \param rhs
            A constant reference to the ComponentType on the right.
        \return
            A ComponentType that is the result of the AND on the two ComponentTypes.
    */
    ComponentType operator&(const ComponentType& lhs, const ComponentType& rhs) {
        return static_cast<ComponentType>(static_cast<unsigned long>(lhs)
            & static_cast<unsigned long>(rhs));
    }

    /*!
        Overload for the |= operator.  Simply calls | on lhs and rhs and stores
        it back into lhs.
        \param lhs
            A reference to the ComponentType we want to OR with rhs and assign the
            result to.
        \param rhs
            A constant reference to the ComponentType on the right.
        \return
            A reference to the modified lhs.
    */
    ComponentType& operator|=(ComponentType& lhs, const ComponentType& rhs) {
        lhs = lhs | rhs;
        return lhs;
    }

    std::ostream& operator<<(std::ostream& os, const ComponentType& type) {
        os << static_cast<unsigned long>(type);
        return os;
    }

    /*!
        Constructor for a new Component.
        \param type
            A ComponentType that we want to assign to this Component.
        \param required
            A ComponentType mask of all Components that must already be on the
            parent Entity to allow this Component to be added too.
    */
    Component::Component(Entity& parent, ComponentType type, ComponentType required)
        : Parent(parent), Type(type), Required(required) {}

    void Component::Serialize(Serializer& root) {}

    void Component::Deserialize(Serializer& root) {}

    /*!
        Returns the Type of this component, which lets the user know the type of
        Component that this is.
        \return
            A ComponentType that determines this Component's type.
    */
    ComponentType Component::GetType() { return Type; }

    /*!
        Returns a mask of the Components that need to be on the Entity for this
        component to work.
        \return
            A ComponentType that contains all of the ComponentTypes of the
            required components.
    */
    ComponentType Component::GetRequired() { return Required; }

} // ForLeaseEngine
