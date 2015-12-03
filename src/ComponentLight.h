/*!
    \file   ComponentLight.h
    \author Sean McGeer
    \date   10/30/15
    \brief
        Defines the Light component.
    \see ComponentLight.cpp

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/


#ifndef COMPONENT_LIGHT_H
#define COMPONENT_LIGHT_H

#include "Component.h"
#include "ComponentTransform.h"
#include "Vector.h"
#include "Color.h"

namespace ForLeaseEngine {

    class Entity;

    namespace Components {

        /*!
            \class Light

            \brief
                A basic Light component that stores the Mass, Velocity, Acceleration,
                and net Force on the owning entity.
        */
        class Light : public Component {
            public:
                static const ComponentType Type = ComponentType::Light;
                virtual ComponentType GetType() { return Type; }
                Light(Entity& owner, Vector direction = Vector(0,-1), Color drawColor = Color(1,1,1), float sweep = 1, unsigned rays = 100);
                void Update();
                void Serialize(Serializer& root);
                void Deserialize(Serializer& root);
                Vector Direction;
                Color DrawColor;
                float Sweep;
                unsigned Rays;
            private:
                Light() = delete;
        };

    } // Components

} // ForLeaseEngine

#endif
