#include "MenuItems.h"
#include "Engine.h"

namespace ForLeaseEngine {

    MenuItem::MenuItem(std::string text, MenuItemType type) : Text(text), Type(type) {}

    void MenuItem::Serialize(Serializer& root) {
    }

    void MenuItem::Deserialize(Serializer& root) {
    }

    namespace MenuItems {

        LoadLevel::LoadLevel(std::string text, std::string stateName)
            : MenuItem(text, MenuItemType::LoadLevel), StateName(stateName) {}

        void LoadLevel::operator() () {
            ForLease->GameStateManager().SetState(StateName);
        }

        void LoadLevel::Serialize(Serializer& root) {
        }

        void LoadLevel::Deserialize(Serializer& root) {
        }

    } // MenuItems

} // ForLeaseEngine