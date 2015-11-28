/*!
    \file   ComponentCamera.cpp
    \author Christopher Hudson

    \brief
        Defines a camera representing a clipping rectangle for rendering

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/
#include "ComponentCamera.h"
#include "Serializable.h"
#include "Serialize.h"
#include "ComponentTransform.h"
#include "Entity.h"
#include "Matrix.h"
#include "Engine.h"
#include "Window.h"


namespace ForLeaseEngine {
    namespace Components {
        Camera::Camera(Entity& parent, float near, float far, float size)
                      : Component(parent, ComponentType::Transform),
                      Near(near), Far(far), Size(size) {}

        Camera::~Camera() {
            //std::cout << "CAMERA DELETED" << std::endl;
        }

        void Camera::Update() {}

        void Camera::Serialize(Serializer& root) {
            root.WriteUint("Type", static_cast<unsigned>(Type));
            Serializer camera = root.GetChild("Camera");
            camera.WriteFloat("Near", Near);
            camera.WriteFloat("Far", Far);
            camera.WriteFloat("Size", Size);
            camera.WriteUint("Type", static_cast<unsigned>(Type));
            root.Append(camera, "Camera");
        }

        void Camera::Deserialize(Serializer& root) {
            Serializer camera = root.GetChild("Camera");
            camera.ReadFloat("Near", Near);
            camera.ReadFloat("Far", Far);
            camera.ReadFloat("Size", Size);
        }

        Point Camera::WorldToScreen(const Point& point) {
            Transform* trans = Parent.GetComponent<Transform>();
            int xres = ForLease->GameWindow->GetXResolution();
            int yres = ForLease->GameWindow->GetYResolution();
            Matrix m = Matrix::Translation(Point(xres / 2.0f, yres / 2.0f)) * Matrix::Scale(yres / Size, yres / Size) * Matrix::RotationRad(-trans->Rotation) * Matrix::Translation(-trans->Position);
            return m * point;
        }

        Point Camera::ScreenToWorld(const Point& point) {
            Transform* trans = Parent.GetComponent<Transform>();
            int xres = ForLease->GameWindow->GetXResolution();
            int yres = ForLease->GameWindow->GetYResolution();
            Matrix m = Matrix::Translation(trans->Position) * Matrix::RotationRad(trans->Rotation) * Matrix::Scale(Size / yres, Size / yres) * Matrix::Translation(Point(-xres / 2.0f, -yres / 2.0f));
            return m * point;
        }
    }
}
