#include "ComponentCamera.h"
#include "Serializable.h"
#include "Serialize.h"

namespace ForLeaseEngine {
    namespace Components {
        Camera::Camera(Entity& parent, float near, float far, float size)
                      : Component(parent, ComponentType::Transform),
                      Near(near), Far(far), Size(size) {}

        Camera::~Camera() {}

        void Camera::Update() {}

        void Camera::Serialize(Serializer& root) {
            root.WriteFloat("Near", Near);
            root.WriteFloat("Far", Far);
            root.WriteFloat("Size", Size);
        }

        void Camera::Deserialize(Serializer& root) {
            root.ReadFloat("Near", Near);
            root.ReadFloat("Far", Far);
            root.ReadFloat("Size", Size);
        }
    }
}
