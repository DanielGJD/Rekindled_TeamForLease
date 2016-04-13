/*!
    \file   ComponentParallax.h
    \author Christopher Hudson

    \brief
        Defines a component that parallaxes sprites

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef COMPONENT_PARALLAX_H
#define COMPONENT_PARALLAX_H

#include "Component.h"

namespace ForLeaseEngine {
    namespace Components {
        class Parallax : public Component {
            public:
                static const ComponentType Type = ComponentType::Parallax;
                virtual ComponentType GetType() { return Type; }

                bool Active;
                bool Repeating;  // Unimplemented

                Parallax(Entity& parent, bool active = true, bool repeating = false);
                ~Parallax();
                void Update();
                void Serialize(Serializer& root);
                void Deserialize(Serializer& root);
            private:

        };
    }
}

#endif // COMPONENT_PARALLAX_H
