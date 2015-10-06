#include "ComponentCamera.h"

namespace ForLeaseEngine {
    namespace Components {
        Camera::Camera(Entity& parent, float near, float far, float size)
                      : Component(parent, ComponentType::Camera, ComponentType::Transform),
                      Near(near), Far(far), Size(size) {}

        Camera::~Camera() {}

        void Camera::Update() {}
    }
}
