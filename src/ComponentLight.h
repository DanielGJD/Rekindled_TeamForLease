/*!
    \file   ComponentLight.h
    \author Sean McGeer
    \date   10/30/15
    \brief
        Defines the Light component.
    \see ComponentLight.cpp
*/


#ifndef COMPONENT_LIGHT_H
#define COMPONENT_LIGHT_H

#include "Component.h"
#include "ComponentTransform.h"
#include "Vector.h"

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
                Light(Entity& owner, Vector start = Vector(-1,1),
                    Vector end = Vector(1,1), unsigned additional = 100,
                    float length = 500);
                void Update();
                void Serialize(Serializer& root);
                void Deserialize(Serializer& root);
                Vector Start;
                Vector End;
                float Length;
                unsigned Additional;
            private:
                Light() = delete;
        };

    } // Components

} // ForLeaseEngine

#endif
