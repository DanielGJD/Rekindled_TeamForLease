/*!
    \file   ComponentCollision.cpp
    \author Sean McGeer
    \date   9/25/15
    \brief
        Implements the Collision component.
    \see ComponentCollision.h

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "ComponentCollision.h"
#include "LevelComponentRenderer.h"

namespace ForLeaseEngine {

    namespace Components {

        /*!
            Constructor for the Collision class.

            \param owner
                The entity that uses this Collision component.
        */
        Collision::Collision(Entity& owner, float width, float height, bool resolve, float offsetX, float offsetY)
            : Component(owner, ComponentType::Transform), Width(width), Height(height),
            OffsetX(offsetX), OffsetY(offsetY), CollidedLastFrame(false), CollidedWith(0), ResolveCollisions(resolve) {}

        void Collision::Serialize(Serializer& root) {
            root.WriteUint("Type", static_cast<unsigned>(Type));
            Serializer collision = root.GetChild("Collision");
            collision.WriteFloat("Height", Height);
            collision.WriteFloat("Width", Width);
            collision.WriteFloat("OffsetX", OffsetX);
            collision.WriteFloat("OffsetY", OffsetY);
            collision.WriteBool("ResolveCollisions", ResolveCollisions);
            collision.WriteUint("Type", static_cast<unsigned>(Type));
            root.Append(collision, "Collision");
        }

        void Collision::Deserialize(Serializer& root) {
            Serializer collision = root.GetChild("Collision");
            collision.ReadFloat("Height", Height);
            collision.ReadFloat("Width", Width);
            collision.ReadFloat("OffsetX", OffsetX);
            collision.ReadFloat("OffsetY", OffsetY);
            collision.ReadBool("ResolveCollisions", ResolveCollisions);
            CollidedLastFrame = false;
        }

        void Collision::DebugDraw() {
            LevelComponents::Renderer* renderer = ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Renderer>(true);
            Components::Transform* transform = Parent.GetComponent<Components::Transform>(true);
            Point position = transform->Position;
            position[0] += OffsetX;
            position[1] += OffsetY;
            renderer->DrawRectangle(position, Width, Height, transform->Rotation);
        }
    } // Components

} // ForLeaseEngine
