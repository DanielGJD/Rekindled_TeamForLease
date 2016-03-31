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
#include <unordered_set>

namespace ForLeaseEngine {

    class Entity;

    enum class CollisionSide : char {
        Left,
        Right,
        Top,
        Bottom,
        None
    };

    class SweptCollision {
        public:
            Vector Normal;
            float Distance;
            CollisionSide Side;
            CollisionSide SelfSide; // Awkward naming, but basically this denotes which side of the entity we're sweeping as collided with the other entity

            SweptCollision(Vector normal = Vector(0, 0), float distance = 1.0f, CollisionSide side = CollisionSide::None);
    };

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

                Collision(Entity& owner, float width = 1, float height = 1, bool resolve = true, float offsetX = 0, float offsetY = 0, bool pacingPlatform = false);
                ~Collision();
                void Initialize();
                void Update() {};
                void OnCollide(const Event* e);
                bool CollidedWith(Entity* entity);
                bool CollidedWith(std::string name);

                float ScaledWidth();
                float ScaledHeight();
                float HalfWidth();
                float HalfHeight();
                float ScaledHalfWidth();
                float ScaledHalfHeight();

                Point TopLeft();
                Point TopRight();
                Point BotLeft();
                Point BotRight();

                Vector Offset();
                Point Position();

                void Serialize(Serializer& root);
                void Deserialize(Serializer& root);
                void DebugDraw();

                float Width;              //! Width of the collision box
                float Height;             //! Height of the collision box
                float OffsetX;            //! Offset of the collision box on the X axis
                float OffsetY;            //! Offset of the collision box on the Y axis
                bool CollidedLastFrame;   //! Whether or not the entity collided on the last frame.  This should be handled by an event.
                //Entity* CollidedWith;   //! The last entity this entity collided with
                std::unordered_set<Entity *> CollidedWithLastFrame;
                CollisionSide CollidedWithSide;    //! The side of the last entity this entity collided with
                bool ResolveCollisions;   //! Whether we want to resolve collisions by moving entities

        private:
                Collision() = delete;
        };

    } // Components

} // ForLeaseEngine

#endif
