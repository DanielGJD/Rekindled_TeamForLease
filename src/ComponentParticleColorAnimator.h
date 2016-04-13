/*!
    \file   ComponentParticleColorAnimator.h
    \author Christopher Hudson

    \brief
        Defines a component that animates particle colors

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef COMPONENT_PARTICLE_COLOR_ANIMATOR_H
#define COMPONENT_PARTICLE_COLOR_ANIMATOR_H

#include "Component.h"
#include "Color.h"
#include <vector>

namespace ForLeaseEngine {
    namespace Components {
        class ParticleColorAnimator : public Component {
            public:
                static const ComponentType Type = ComponentType::ParticleColorAnimator;
                bool Active;

                ParticleColorAnimator(Entity& parent, bool active = true);
                ~ParticleColorAnimator();
                virtual ComponentType GetType();
                void Update();
                void Serialize(Serializer& root);
                void Deserialize(Serializer& root);

                void AddColor(Color const& color);
                void RemoveColor(unsigned int pos);
                void SetColor(unsigned int pos, Color const& color);
                Color GetColor(unsigned int pos);
                unsigned int GetColorCount();
            private:
                std::vector<Color> Colors;
        };
    }
}

#endif // COMPONENT_PARTICLE_COLOR_ANIMATOR_H
