/*!
    \file   ComponentMenu.h
    \author Sean McGeer
    \date   11/21/15

    \brief
        Defines the menu level component.

    \see ComponentMenu.h
    \see LevelComponentMenu.h

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "LevelComponentMenu.h"
#include "Engine.h"

namespace ForLeaseEngine {

    namespace LevelComponents {

        Menu::Menu(State& owner) : LevelComponent(owner) {}

        void Menu::Update(std::vector<Entity*>& entities) {
            //for (Entity* entity : entities) {
            //    if (!entity->HasComponent(ComponentType::Menu)) continue;

            //    Components::SpriteText text(*entity, "Arial.fnt", "HI!", Color(1, 1, 1));
            //    LevelComponents::Renderer* renderer = ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Renderer>();
            //    renderer->DrawSpriteText(&text, Point(0, 0), 3, 3, 0);
            //}
        }

        void Menu::Serialize(Serializer& root) {
        }

        void Menu::Deserialize(Serializer& root) {
        }

    } // LevelComponents

} // ForLeaseEngine