/*!
    \file   ComponentBackgroundMask.h
    \author Sean McGeer
    \date   4/05/16
    \brief
        Defines the Background Mask component.
    \see ComponentBackgroundMask.cpp

    \copyright ©Copyright 2016 DigiPen Institute of Technology, All Rights Reserved
*/


#ifndef COMPONENT_BACKGROUND_MASK_H
#define COMPONENT_BACKGROUND_MASK_H

#include "Component.h"
#include "Vector.h"

namespace ForLeaseEngine {

    class Entity;

    namespace Components {

        /*!
            \class BackgroundMask
        */
        class BackgroundMask : public Component {
                enum class ModifyingColor : char {
                    Red,
                    Green,
                    Blue
                };
            public:
                static const ComponentType Type = ComponentType::BackgroundMask;
                virtual ComponentType GetType() { return Type; }
                BackgroundMask(Entity& owner, ModifyingColor mod = ModifyingColor::Red);
                void Update();
                void Serialize(Serializer& root);
                void Deserialize(Serializer& root);

                ModifyingColor CurrentState;
            private:
                BackgroundMask() = delete;
        };

    } // Components

} // ForLeaseEngine

#endif
