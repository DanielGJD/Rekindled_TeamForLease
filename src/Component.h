/*!
    \file   Component.h
    \author Sean McGeer
    \date   9/18/15
    \brief
        Defines the Component class.  Also defines the ComponentType enum and
        corresponding operator overloads.
    \see Component.cpp
*/

#ifndef COMPONENT_H
#define COMPONENT_H

#include <iostream>

namespace ForLeaseEngine {

    /*!
        \enum ComponentType
        \brief
            The type of a component.  We treat it as an unsigned long to allow
            for 64 components.  This is unlikely to be enough for a game of any
            scale unless we implement scripting or some other solution for
            loading game code.
    */
    enum class ComponentType : unsigned long {
        None             = 0,        //! No component
        Transform        = 1 << 0,   //! Transform
        PlayerController = 1 << 1,   //! Player controls
        Physics          = 1 << 2,   //! Physics
        Camera           = 1 << 3,   //! Camera
        Draw             = 1 << 4    //! Draw

    };

    ComponentType operator|(const ComponentType& lhs, const ComponentType& rhs);
    ComponentType operator&(const ComponentType& lhs, const ComponentType& rhs);
    ComponentType& operator|=(ComponentType& lhs, const ComponentType& rhs);
    std::ostream& operator<<(std::ostream& os, const ComponentType& type);

    /*!
        \class Component
        \brief
            A class that defines a Component.  All other components should
            inherit from this.
    */
    class Component
    {

        public:
            Component(ComponentType id = ComponentType::None);
            virtual ~Component() {};
            ComponentType GetType();
            virtual void Update() = 0;
        private:
            ComponentType Type; //! The type of component.

    };

} // ForLeaseEngine

#endif
