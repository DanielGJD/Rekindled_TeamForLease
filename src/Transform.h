/*!
    \file   Transform.h
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
                Transform(Entity& parent, float positionX = 0,
                    float positionY = 0, float scaleX = 1, float scaleY = 1,
                    float rotation = 0);

                Transform(Entity& parent, Point position, float scaleX = 1,
                    float scaleY = 1, float rotation = 0);

                Transform(Entity& parent, Point position, Vector scale,
                    float rotation = 0);

                ~Transform() {};
                void Update() {};

                void SetPositionX(float x);
                float GetPositionX();
                void SetPositionY(float y);
                float GetPositionY();
                void SetPosition(float x, float y);
                void SetPosition(Point position);
                Point GetPosition();

                void SetScaleX(float x);
                float GetScaleX();
                void SetScaleY(float y);
                float GetScaleY();
                void SetScale(float x, float y);
                void SetScale(Vector scale);
                Vector GetScale();

                void SetRotation(float rotation);
                float GetRotation();
            private:
                Point Position; //! The position of the object
                float ScaleX;   //! Scale on the X axis
                float ScaleY;   //! Scale on the Y axis
                float Rotation; //! Rotation in radians

                Transform() = delete;

        };

    } // Components

} // ForLeaseEngine

#endif
