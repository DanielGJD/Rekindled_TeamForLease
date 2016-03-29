/*!
    \file   LevelComponentUsefulObject.h
    \author Sean McGeer
    \date   3/29/16

    \brief
        Defines the useful object level component.

    \see ComponentUsefulObject.h
    \see LevelComponentUsefulObject.cpp

    \copyright ©Copyright 2016 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef LC_USEFUL_OBJECT_H
#define LC_USEFUL_OBJECT_H

#include "LevelComponent.h"
#include "ComponentUsefulObject.h"
#include <string>

namespace ForLeaseEngine {

    namespace LevelComponents {

        class UsefulObject : public LevelComponent {
            public:
                static const ComponentType Type = ComponentType::UsefulObject;
                virtual ComponentType GetType() { return Type; }

                UsefulObject(State& owner, std::string balloon = std::string(""), std::string distraction = std::string(""));

                void Serialize(Serializer& root);
                void Deserialize(Serializer& root);

                void Update(std::vector<Entity *>& entities) {}

                std::string BalloonArchetypeName;
                std::string DistractionArchetypeName;
        };

    } // LevelComponents

} // ForLeaseEngine

#endif // LC_USEFUL_OBJECT_H