/*!
    \file   MenuItems.cpp
    \author Sean McGeer
    \date   11/20/15

    \brief
        Implements menu items.

    \see MenuItems.h

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "MenuItems.h"
#include "Engine.h"
#include "GameStateManager.h"

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
            root.WriteUint("Type", static_cast<unsigned>(Type));
            Serializer loadLevel = root.GetChild("LoadLevel");
            loadLevel.WriteString("Image", Image);
            loadLevel.WriteString("StateName", StateName);
            loadLevel.WriteUint("Type", static_cast<unsigned>(Type));
            root.Append(loadLevel, "LoadLevel");
        }

        void LoadLevel::Deserialize(Serializer& root) {
            Serializer loadLevel = root.GetChild("LoadLevel");
            loadLevel.ReadString("Image", Image);
            loadLevel.ReadString("StateName", StateName);
        }

        // ==================================================================================

        NextLevel::NextLevel(std::string image) : MenuItem(MenuItemType::NextLevel, image) {}

        void NextLevel::Action() {
            ForLease->GameStateManager().SetAction(Modules::StateAction::Next);
        }

        void NextLevel::Serialize(Serializer& root) {
            root.WriteUint("Type", static_cast<unsigned>(Type));
            Serializer nextLevel = root.GetChild("NextLevel");
            nextLevel.WriteString("Image", Image);
            nextLevel.WriteUint("Type", static_cast<unsigned>(Type));
            root.Append(nextLevel, "NextLevel");
        }

        void NextLevel::Deserialize(Serializer& root) {
            Serializer nextLevel = root.GetChild("NextLevel");
            nextLevel.ReadString("Image", Image);
        }

        // ==================================================================================

        Quit::Quit(std::string image) : MenuItem(MenuItemType::Quit, image) {}

        void Quit::Action() {
            ForLease->GameStateManager().SetAction(Modules::StateAction::Quit);
        }

        void Quit::Serialize(Serializer& root) {
            root.WriteUint("Type", static_cast<unsigned>(Type));
            Serializer quit = root.GetChild("Quit");
            quit.WriteString("Image", Image);
            quit.WriteUint("Type", static_cast<unsigned>(Type));
            root.Append(quit, "Quit");
        }

        void Quit::Deserialize(Serializer& root) {
            Serializer quit = root.GetChild("Quit");
            quit.ReadString("Image", Image);
        }

    } // MenuItems

} // ForLeaseEngine