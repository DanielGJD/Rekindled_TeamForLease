/*!
    \file   ComponentTransform.cpp
    \author Sean McGeer
    \date   9/19/15
    \brief
        Implements the Transform component.
    \see Transform.h
*/

#include "ComponentTransform.h"

namespace ForLeaseEngine {

    namespace Components {

        /*!
            Constructor for a new Transform component.

            \param owner
                The Entity that this Transform component belongs to.

            \param positionX
                A float denoting the initial X position of this object.
                Defaults to 0.

            \param positionY
                A float denoting the initial Y position of this object.
                Defaults to 0.

            \param scaleX
                A float denoting the initial X scaling of this object.
                Defaults to 1.

            \param scaleY
                A float denoting the initial Y scaling of this object.
                Defaults to 1.

            \param rotation
                A flaot denoting the initial rotation of this object.
                Defaults to 0.
        */
        Transform::Transform(Entity& owner, float positionX, float positionY,
            float scaleX, float scaleY, float rotation, int zOrder)
            : Component(owner), Position(positionX, positionY), ScaleX(scaleX),
              ScaleY(scaleY), Rotation(rotation), ZOrder(zOrder) {}

        /*!
            Constructor for a new Transform component.

            \param owner
                The Entity that this Transform component belongs to.

            \param position
                A Point denoting the initial position of this object.

            \param scaleX
                A float denoting the initial X scaling of this object.
                Defaults to 1.

            \param scaleY
                A float denoting the initial Y scaling of this object.
                Defaults to 1.

            \param rotation
                A flaot denoting the initial rotation of this object.
                Defaults to 0.
        */
        Transform::Transform(Entity& owner, Point position, float scaleX,
            float scaleY, float rotation, int zOrder)
            : Component(owner), Position(position), ScaleX(scaleX),
              ScaleY(scaleY), Rotation(rotation), ZOrder(zOrder) {}

        void Transform::Serialize(Serializer& root) {
            root.WriteUint("Type", static_cast<unsigned>(Type));
            Serializer transform = root.GetChild("Transform");
            transform.WriteVec("Position", Position);
            transform.WriteFloat("ScaleX", ScaleX);
            transform.WriteFloat("ScaleY", ScaleY);
            transform.WriteFloat("Rotation", Rotation);
            transform.WriteInt("ZOrder", ZOrder);
            transform.WriteUint("Type", static_cast<unsigned>(Type));
            root.Append(transform, "Transform");
        }

        void Transform::Deserialize(Serializer& root) {
            Serializer transform = root.GetChild("Transform");
            transform.ReadVec("Position", Position);
            transform.ReadFloat("ScaleX", ScaleX);
            transform.ReadFloat("ScaleY", ScaleY);
            transform.ReadFloat("Rotation", Rotation);
            transform.ReadInt("ZOrder", ZOrder);
        }

    } // Components

} // ForLeaseEngine
