/*!
    \file   ComponentCollision.cpp
    \author Sean McGeer
    \date   9/25/15
    \brief
        Implements the Collision component.
    \see ComponentCollision.h
*/

#include "ComponentCollision.h"
#include "Engine.h"
#include "Entity.h"
#include "CollisionEvent.h"

namespace ForLeaseEngine {

    namespace Components {

        /*!
            Constructor for the Collision class.

            \param owner
                The entity that uses this Collision component.
        */
        Collision::Collision(Entity& owner, float width, float height, bool resolve)
            : Component(owner, ComponentType::Transform), Width(width), Height(height),
            ResolveCollisions(resolve) {}

        void Collision::Initialize() {
            std::cout << Parent.GetName() << " collision init." << std::endl;
            ForLease->Dispatcher.Attach(NULL, this, "Collision", &Collision::OnCollide);
        }

        void Collision::OnCollide(const Event* e) {
            const CollisionEvent* event = static_cast<const CollisionEvent *>(e);
            std::cout << Parent.GetName() << " collided with " << event->Other->GetName() << std::endl;
        }

        void Collision::Serialize(Serializer& root) {
            root.WriteUint("Type", static_cast<unsigned>(Type));
            Serializer collision = root.GetChild("Collision");
            collision.WriteFloat("Height", Height);
            collision.WriteFloat("Width", Width);
            collision.WriteBool("ResolveCollisions", ResolveCollisions);
            collision.WriteUint("Type", static_cast<unsigned>(Type));
            root.Append(collision, "Collision");
        }

        void Collision::Deserialize(Serializer& root) {
            Serializer collision = root.GetChild("Collision");
            collision.ReadFloat("Height", Height);
            collision.ReadFloat("Width", Width);
            collision.ReadBool("ResolveCollisions", ResolveCollisions);
            CollidedLastFrame = false;
        }
    } // Components

} // ForLeaseEngine
