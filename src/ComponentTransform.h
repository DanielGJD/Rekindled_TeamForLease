/*!
    \file   ComponentTransform.h
    \author Sean McGeer
    \date   9/19/15
    \brief
        Defines the Transform component.
    \see Transform.cpp
*/

#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Component.h"
#include "Vector.h"

namespace ForLeaseEngine {

    class Entity;

    namespace Components {

        /*!
            \class Transform
            \brief
                Basic Transform component.  Inerhits from components, and stores
                the X and Y coordinates in world space.
        */
        class Transform : public Component {

            public:
                Transform(Entity& owner, float positionX = 0,
                    float positionY = 0, float scaleX = 1, float scaleY = 1,
                    float rotation = 0, int zOrder = 0);

                Transform(Entity& owner, Point position, float scaleX = 1,
                    float scaleY = 1, float rotation = 0, int zOrder = 0);

                ~Transform() {};
                void Update() {};
                void Serialize(Serializer& root);
                void Deserialize(Serializer& root);

                Point Position; //! The position of the object
                float ScaleX;   //! Scale on the X axis
                float ScaleY;   //! Scale on the Y axis
                float Rotation; //! Rotation in radians
                int   ZOrder;   //! Draw order, higher drawn first
            private:
                Transform() = delete;

        };

    } // Components

} // ForLeaseEngine

#endif
