#include "ComponentOccluder.h"

namespace ForLeaseEngine {
    namespace Components {
        Occluder::Occluder(Entity& parent, bool blocksVision, bool blocksLight) :
                          Component(parent, ComponentType::Transform | ComponentType::Collision),
                          BlocksVision(blocksVision), BlocksLight(blocksLight) {}

        Occluder::~Occluder() {}

        void Occluder::Update() {}

        void Occluder::Serialize(Serializer& root) {
            Serializer occluder = root.GetChild("Occluder");
            occluder.WriteUint("Type", static_cast<unsigned int>(Type));
            occluder.WriteBool("BlocksVision", BlocksVision);
            occluder.WriteBool("BlocksLight", BlocksLight);
            root.Append(occluder, "Occluder");
        }

        void Occluder::Deserialize(Serializer& root) {
            Serializer occluder = root.GetChild("Occluder");
            occluder.ReadBool("BlocksVision", BlocksVision);
            occluder.ReadBool("BlocksLight", BlocksLight);
        }
    }
}
