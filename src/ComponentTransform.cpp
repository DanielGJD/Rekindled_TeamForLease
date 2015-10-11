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
            : Component(owner, ComponentType::Transform),
              Position(positionX, positionY), ScaleX(scaleX), ScaleY(scaleY),
              Rotation(rotation), ZOrder(zOrder) {}

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
            : Component(owner, ComponentType::Transform), Position(position),
              ScaleX(scaleX), ScaleY(scaleY), Rotation(rotation), ZOrder(zOrder) {}

    } // Components

} // ForLeaseEngine
