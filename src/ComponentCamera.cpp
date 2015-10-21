#include "ComponentCamera.h"
#include "Serializable.h"
#include "Serialize.h"

namespace ForLeaseEngine {
    namespace Components {
        Camera::Camera(Entity& parent, float near, float far, float size)
                      : Component(parent, ComponentType::Camera, ComponentType::Transform),
                      Near(near), Far(far), Size(size) {}

        Camera::~Camera() {}

        void Camera::Update() {}

        void Camera::Serialize(Serializer& root) {
            Serializer camera = root.GetChild("Camera");
            camera.WriteFloat("Near", Near);
            camera.WriteFloat("Far", Far);
            camera.WriteFloat("Size", Size);
            root.Append(camera, "Camera");
        }

        void Camera::Deserialize(Serializer& root) {
            Serializer camera = root.GetChild("Camera");
            camera.ReadFloat("Near", Near);
            camera.ReadFloat("Far", Far);
            camera.ReadFloat("Size", Size);
        }
    }
}
