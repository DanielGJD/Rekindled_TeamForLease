/*!
    \file   ComponentCollision.cpp
    \author Sean McGeer
    \date   9/25/15
    \brief
        Implements the Collision component.
    \see ComponentCollision.h
*/

#include "ComponentCollision.h"

namespace ForLeaseEngine {
    
    namespace Components {
        
        /*!
            Constructor for the Collision class.

            \param owner
                The entity that uses this Collision component.
        */
        Collision::Collision(Entity& owner)
            : Component(owner, ComponentType::Collision, ComponentType::Transform) {}
        
        void Collision::Serialize(Serializer& root) {
            Serializer collision = root.GetChild("Collision");
            collision.WriteFloat("Height", Height);
            collision.WriteFloat("Width", Width);
            collision.WriteBool("ResolveCollisions", ResolveCollisions);
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
