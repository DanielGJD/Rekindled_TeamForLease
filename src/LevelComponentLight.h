/*!
    \file   LevelComponentLight.h
    \author Christopher Hudson

    \brief
        Defines the Light level component.

    \see    LevelComponentLight.cpp

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef LEVEL_COMPONENT_LIGHT_H
#define LEVEL_COMPONENT_LIGHT_H

#include "LevelComponent.h"
#include "Color.h"
#include <unordered_set>

namespace ForLeaseEngine {

    namespace LevelComponents {

        class Light : public LevelComponent {
            public:
                static const ComponentType Type = ComponentType::Light;
                virtual ComponentType GetType() { return Type; }

                Light(State& owner, Color const& ambientLight = Color(0, 0, 0));
                void Update(std::vector<Entity *>& entities);

                virtual void Serialize(Serializer& root);
                virtual void Deserialize(Serializer& root);

                bool CheckIfLit(unsigned long id);

                Color AmbientLight;
            private:
                std::unordered_set<unsigned long> LitEntities;
        };

    } // LevelComponents

} // ForLeaseEngine


#endif // LEVEL_COMPONENT_LIGHT_H
