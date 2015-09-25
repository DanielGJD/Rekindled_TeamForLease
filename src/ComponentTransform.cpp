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

    } // Components

} // ForLeaseEngine
