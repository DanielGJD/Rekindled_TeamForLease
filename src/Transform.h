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

namespace ForLeaseEngine {

    namespace Components {

        /*!
            \class Transform
            \brief
                Basic Transform component.  Inerhits from components, and stores
                the X and Y coordinates in world space.
        */
        class Transform : public Component {

            public:
                Transform();
                ~Transform() {};
                void Update() {};
                
        };

    } // Components

} // ForLeaseEngine

#endif
