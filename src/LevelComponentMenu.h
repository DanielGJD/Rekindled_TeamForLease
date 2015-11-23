/*!
    \file   ComponentMenu.h
    \author Sean McGeer
    \date   11/21/15

    \brief
        Defines the menu level component.

    \see ComponentMenu.h
    \see LevelComponentMenu.cpp

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef LC_MENU_H
#define LC_MENU_H

#include "LevelComponent.h"
#include "ComponentMenu.h"
#include "Engine.h"
#include "LevelComponentRenderer.h"

namespace ForLeaseEngine {

    namespace LevelComponents {

        class Menu : public LevelComponent {
            public:
                static const ComponentType Type = ComponentType::Menu;
                virtual ComponentType GetType() { return Type; }

                Menu(State& owner);

                void Serialize(Serializer& root);
                void Deserialize(Serializer& root);

                void Update(std::vector<Entity*>& entities);

        };

    } // LevelComponents

} // ForLeaseEngine

#endif // LC_MENU_H