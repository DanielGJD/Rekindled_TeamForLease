/*!
    \file   MenuItems.cpp
    \author Sean McGeer
    \date   11/20/15

    \brief
        Implements menu items.

    \see MenuItems.h
*/

#include "MenuItems.h"
#include "Engine.h"

namespace ForLeaseEngine {

    MenuItem::MenuItem(MenuItemType type, std::string image) : Image(image), Type(type) {}

    void MenuItem::Serialize(Serializer& root) {
    }

    void MenuItem::Deserialize(Serializer& root) {
    }

    namespace MenuItems {

        LoadLevel::LoadLevel(std::string image, std::string stateName)
            : MenuItem(MenuItemType::LoadLevel, image), StateName(stateName) {}

        void LoadLevel::Action () {
            std::cout << StateName << std::endl;
            ForLease->GameStateManager().SetState(StateName);
        }

        void LoadLevel::Serialize(Serializer& root) {
        }

        void LoadLevel::Deserialize(Serializer& root) {
        }

    } // MenuItems

} // ForLeaseEngine