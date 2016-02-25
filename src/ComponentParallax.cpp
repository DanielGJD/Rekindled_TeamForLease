#include "ComponentParallax.h"

namespace ForLeaseEngine {
    namespace Components {
        Parallax::Parallax(Entity& parent, bool active, bool repeating) :
                           Component(parent, ComponentType::Transform), Active(active), Repeating(repeating) {}

        Parallax::~Parallax() {}

        void Parallax::Update() {}

        void Parallax::Serialize(Serializer& root) {
            root.WriteUint("Type", static_cast<unsigned int>(Type));
            Serializer parallax = root.GetChild("Parallax");
            parallax.WriteUint("Type", static_cast<unsigned int>(Type));
            parallax.WriteBool("Active", Active);
            parallax.WriteBool("Repeating", Repeating);
            root.Append(parallax, "Parallax");
        }

        void Parallax::Deserialize(Serializer& root) {
            Serializer parallax = root.GetChild("Parallax");
            parallax.ReadBool("Active", Active);
            parallax.ReadBool("Repeating", Repeating);
        }
    }
}
