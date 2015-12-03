/*!
    \file   ComponentCollision.h
    \author Sean McGeer
    \date   9/25/15
    \brief
        Defines the Collision component.
    \see ComponentCollision.cpp

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef COMPONENT_COLLISION_H
#define COMPONENT_COLLISION_H

#include "Component.h"
#include "ComponentTransform.h"
#include "Event.h"

namespace ForLeaseEngine {

    class Entity;

    namespace Components {

        /*!
            \class Collision

            \brief
                A collision component.  Handles collision detection and resolution.
        */
        class Collision : public Component {
            public:

                enum class Side : char {
                    Left,
                    Right,
                    Top,
                    Bottom
                };

                static const ComponentType Type = ComponentType::Collision;
                virtual ComponentType GetType() { return Type; }

                Collision(Entity& owner, float width = 1, float height = 1, bool resolve = true, float offsetX = 0, float offsetY = 0);
                void Initialize();
                void Update() {};
                void OnCollide(const Event* e);

                float ScaledWidth();
                float ScaledHeight();
                float HalfWidth();
                float HalfHeight();
                float ScaledHalfWidth();
                float ScaledHalfHeight();

                void Serialize(Serializer& root);
                void Deserialize(Serializer& root);
                void DebugDraw();
                float Width;            //! Width of the collision box
                float Height;           //! Height of the collision box
                float OffsetX;          //! Offset of the collision box on the X axis
                float OffsetY;          //! Offset of the collision box on the Y axis
                bool CollidedLastFrame; //! Whether or not the entity collided on the last frame.  This should be handled by an event.
                Entity* CollidedWith;   //! The last entity this entity collided with
                Side CollidedWithSide;  //! The side of the last tneity this entity collided with
                bool ResolveCollisions; //! Whether we want to resolve collisions by moving entities    
        private:
                Collision() = delete;
        };

    } // Components

} // ForLeaseEngine

#endif
