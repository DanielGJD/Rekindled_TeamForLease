/*!
    \file   Component.h
    \author Sean McGeer
    \date   9/18/15
    \brief
        Defines the Component class.  Also defines the ComponentID enum and
        corresponding operator overloads.
    \see Component.cpp
*/

#ifndef COMPONENT_H
#define COMPONENT_H

namespace ForLeaseEngine {

    /*!
        \enum ComponentID
        \brief
            The ID for a component.  We treat it as an unsigned long to allow
            for 64 components.  This is unlikely to be enough for a game of any
            scale unless we implement scripting or some other solution for
            loading game code.
    */
    enum class ComponentID : unsigned long {
        cidNone             = 0,        //! No component
        cidTransform        = 1 << 0,   //! Transform
        cidPlayerController = 1 << 1,   //! Player controls
        cidPhysics          = 1 << 2,   //! Physics
        cidCamera           = 1 << 3,   //! Camera
        cidDraw             = 1 << 4    //! Draw

    };

    ComponentID operator|(const ComponentID& lhs, const ComponentID& rhs);
    ComponentID operator&(const ComponentID& lhs, const ComponentID& rhs);
    ComponentID& operator|=(ComponentID& lhs, const ComponentID& rhs);

    /*!
        \class Component
        \brief
            A class that defines a Component.  All other components should
            inherit from this.
    */
    class Component
    {

        public:
            Component(ComponentID id = ComponentID::cidNone);
            ComponentID GetID();
            virtual void Update() = 0;
        private:
            ComponentID ID; //! The type of component.

    };

} // ForLeaseEngine

#endif
