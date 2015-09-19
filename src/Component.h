 /*!
    \file   Component.h
    \author Sean McGeer
    \date   9/18/15
    \brief
        Defines the Component class.  Also defines the ComponentID enum and 
    \see Component.h
*/

#ifndef COMPONENT_H
#define COMPONENT_H

namespace ForLeaseEngine {

    enum class ComponentID : unsigned long {
        cidNone             = 0,
        cidTransform        = 1 << 0,
        cidPlayerController = 1 << 1,
        cidPhysics          = 1 << 2,
        cidCamera           = 1 << 3,
        cidDraw             = 1 << 4
    };

    ComponentID operator|(const ComponentID& lhs, const ComponentID& rhs);
    ComponentID& operator|=(ComponentID& lhs, const ComponentID& rhs);

    /*!
        \class Component
        \brief
            A class that defines a Component.  All other components should inherit from this.
    */
    class Component
    {

        public:
            Component(ComponentID id = ComponentID::cidNone);
            ComponentID GetID();
        private:
            ComponentID ID;

    };

} // ForLeaseEngine

#endif
