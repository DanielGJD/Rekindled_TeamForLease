#include "ComponentParallax.h"

namespace ForLeaseEngine {
    namespace Components {
        Parallax::Parallax(Entity& parent, bool active) : Component(parent, ComponentType::Transform), Active(active) {}

        Parallax::~Parallax() {}

        void Parallax::Update() {}

        void Parallax::Serialize(Serializer& root) {}

        void Parallax::Deserialize(Serializer& root) {}
    }
}
