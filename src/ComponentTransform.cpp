/*!
    \file   Transform.cpp
    \author Sean McGeer
    \date   9/19/15
    \brief
        Implements the Transform component.
    \see Transform.h
*/

#include "ComponentTransform.h"

namespace ForLeaseEngine {
    
    namespace Components {
        //! I'm going to document all of this, I swear.

        Transform::Transform(Entity& parent, float positionX, float positionY,
            float scaleX, float scaleY, float rotation)
            : Component(parent, ComponentType::Transform),
              Position(positionX, positionY), ScaleX(scaleX), ScaleY(scaleY),
              Rotation(rotation) {}

        Transform::Transform(Entity& parent, Point position, float scaleX,
            float scaleY, float rotation)
            : Component(parent, ComponentType::Transform), Position(position),
              ScaleX(scaleX), ScaleY(scaleY), Rotation(rotation) {}

        Transform::Transform(Entity& parent, Point position, Vector scale,
            float rotation)
            : Component(parent, ComponentType::Transform), Position(position),
            ScaleX(scale[0]), ScaleY(scale[1]), Rotation(rotation) {}

        void Transform::SetPositionX(float x) { Position[0] = x; }

        float Transform::GetPositionX() { return Position[1]; }

        void Transform::SetPositionY(float y) { Position[1] = y; }

        float Transform::GetPositionY() { return Position[1]; }

        void Transform::SetPosition(float x, float y) {
            Position[0] = x;
            Position[1] = y;
        }

        void Transform::SetPosition(Point position) { Position = position; }

        Point Transform::GetPosition() { return Position; }

        void Transform::SetScaleX(float x) { ScaleX = x; }

        float Transform::GetScaleX() { return ScaleX; }

        void Transform::SetScaleY(float y) { ScaleY = y; }

        float Transform::GetScaleY() { return ScaleY; }

        void Transform::SetScale(float x, float y) {
            ScaleX = x;
            ScaleY = y;
        }

        void Transform::SetScale(Vector scale) {
            ScaleX = scale[0];
            ScaleY = scale[1];
        }

        Vector Transform::GetScale() { return Vector(ScaleX, ScaleY); }

        void Transform::SetRotation(float rotation) { Rotation = rotation; }

        float Transform::GetRotation() { return Rotation; }

    } // Components

} // ForLeaseEngine
