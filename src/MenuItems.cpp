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

    MenuItem::MenuItem(MenuItemType type, std::string text) : Text(text), Type(type) {}

    void MenuItem::Serialize(Serializer& root) {
    }

    void MenuItem::Deserialize(Serializer& root) {
    }

    namespace MenuItems {

        LoadLevel::LoadLevel(std::string text, std::string stateName)
            : MenuItem(MenuItemType::LoadLevel, text), StateName(stateName) {}

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