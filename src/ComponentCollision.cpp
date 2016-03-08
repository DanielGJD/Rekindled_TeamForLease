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
#include "Engine.h"
#include "Entity.h"
#include "CollisionEvent.h"
#include "LevelComponentRenderer.h"
#include "Exception.h"

namespace ForLeaseEngine {

    namespace Components {

        /*!
            Constructor for the Collision class.

            \param owner
                The entity that uses this Collision component.
        */
        Collision::Collision(Entity& owner, float width, float height, bool resolve, float offsetX, float offsetY, bool pacingPlatform)
            : Component(owner, ComponentType::Transform), Width(width), Height(height),
            OffsetX(offsetX), OffsetY(offsetY), CollidedLastFrame(false), CollidedWith(0), ResolveCollisions(resolve),
            PacingPlatform(pacingPlatform && owner.HasComponent(ComponentType::EnemyPace)) {}

        void Collision::Initialize() {
            std::cout << Parent.GetName() << " collision init." << std::endl;
            ForLease->Dispatcher.Attach(NULL, this, "Collision", &Collision::OnCollide);
        }

        void Collision::OnCollide(const Event* e) {
            //const CollisionEvent* event = static_cast<const CollisionEvent *>(e);
            //std::cout << Parent.GetName() << " collided with " << event->Other->GetName() << std::endl;
        }

        float Collision::ScaledWidth() {
            Components::Transform* transform = Parent.GetComponent<Components::Transform>(true);
            return Width * transform->ScaleX;
        }

        float Collision::ScaledHeight() {
            Components::Transform* transform = Parent.GetComponent<Components::Transform>(true);
            return Height * transform->ScaleY;
        }

        float Collision::HalfWidth() {
            return Width / 2;
        }

        float Collision::HalfHeight() {
            return Height / 2;
        }

        float Collision::ScaledHalfWidth() {
            return ScaledWidth() / 2;
        }

        float Collision::ScaledHalfHeight() {
            return ScaledHeight() / 2;
        }

        Vector Collision::Offset() {
            return Vector(OffsetX, OffsetY);
        }

        Point Collision::Position() {
            Components::Transform* transform = Parent.GetComponent<Components::Transform>(true);
            return transform->Position + Offset();
        }

        void Collision::Serialize(Serializer& root) {
            root.WriteUint("Type", static_cast<unsigned>(Type));
            Serializer collision = root.GetChild("Collision");
            collision.WriteFloat("Height", Height);
            collision.WriteFloat("Width", Width);
            collision.WriteFloat("OffsetX", OffsetX);
            collision.WriteFloat("OffsetY", OffsetY);
            collision.WriteBool("ResolveCollisions", ResolveCollisions);
            collision.WriteBool("PacingPlatform", PacingPlatform);
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
            collision.ReadBool("PacingPlatform", PacingPlatform);
            CollidedLastFrame = false;
        }

        void Collision::DebugDraw() {
            LevelComponents::Renderer* renderer = ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Renderer>(true);
//            renderer->SetDrawingColor(Color(1, 0, 0));
            Components::Transform* transform = Parent.GetComponent<Components::Transform>(true);
            Point position = transform->Position;
            position[0] += OffsetX;
            position[1] += OffsetY;
            renderer->DrawRectangle(position, ScaledWidth(), ScaledHeight(), transform->Rotation);
        }

        bool Collision::IsPacingPlatform() {
            return PacingPlatform;
        }

        bool Collision::SetPacingPlatform(bool setPacing, bool throwOnFail) {
            if (setPacing) {
                if (Parent.HasComponent(ComponentType::EnemyPace)) {
                    PacingPlatform = true;
                    return true;
                }
                else if (throwOnFail)
                    throw Exception("No enemy pace component on this!");
                else
                    return false;
            }
            else {
                PacingPlatform = false;
                return true;
            }
        }
    } // Components

} // ForLeaseEngine
