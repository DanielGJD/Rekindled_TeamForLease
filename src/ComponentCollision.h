/*!
    \file   ComponentCollision.h
    \author Sean McGeer
    \date   9/25/15
    \brief
        Defines the Collision component.
    \see ComponentCollision.cpp
*/

#ifndef COMPONENT_COLLISION_H
#define COMPONENT_COLLISION_H

#include "Component.h"
#include "ComponentTransform.h"

namespace ForLeaseEngine {

    namespace Components {

        /*!
            \class Collision

            \brief
                A collision component.  Handles collision detection and resolution.
        */
        class Collision : public Component {
            public:
                static const ComponentType Type = ComponentType::Collision;
                virtual ComponentType GetType() { return Type; }
                Collision(Entity& owner, float width = 1, float height = 1, bool resolve = true);
                void Initialize();
                void Update() {}
                void OnCollide(const Event* e);
                void Serialize(Serializer& root);
                void Deserialize(Serializer& root);
                float Width;            //! Width of the collision box
                float Height;           //! Height of the collision box
                bool CollidedLastFrame; //! Whether or not the entity collided on the last frame.  This should be handled by an event.
                bool ResolveCollisions; //! Whether we want to resolve collisions by moving entities
            private:
                Collision() = delete;
        };

    } // Components

} // ForLeaseEngine

#endif
