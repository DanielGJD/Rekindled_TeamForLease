/*!
    \file   MenuItems.h
    \author Sean McGeer
    \date   11/20/15

    \brief
        Defines menu items.

    \see ComponentMenu.h
    \see MenuItems.cpp

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef MENU_ITEMS_H
#define MENU_ITEMS_H

#include "Serializable.h"
#include <string>

namespace ForLeaseEngine {

    enum class MenuItemType : unsigned {
        None                    = 0,
        LoadLevel               = 1,
        NextLevel               = 2,
        Quit                    = 3,
        ActivateOther           = 4,
        ActivateAndDeactivate   = 5,
        ResumeGame              = 6
    };

    class MenuItem : public Serializable {
        public:
            MenuItem() {}
            MenuItem(MenuItemType type, std::string image);
            std::string Image;
            MenuItemType Type;
            virtual void Action() { }
            virtual void Serialize(Serializer& root);
            virtual void Deserialize(Serializer& root);

    };

    namespace MenuItems {

        class LoadLevel : public MenuItem {
            public:
                LoadLevel(std::string image, std::string stateName);
                virtual void Action();

                virtual void Serialize(Serializer& root);
                virtual void Deserialize(Serializer& root);

                std::string StateName;
        };

        class NextLevel : public MenuItem {
            public:
                NextLevel(std::string image);
                virtual void Action();

                virtual void Serialize(Serializer& root);
                virtual void Deserialize(Serializer& root);
        };

        class Quit : public MenuItem {
            public:
                Quit(std::string image);
                virtual void Action();

                virtual void Serialize(Serializer& root);
                virtual void Deserialize(Serializer& root);
        };

        class ActivateOther : public MenuItem {
            public:
                ActivateOther(std::string image, std::string otherMenu);

                virtual void Action();

                virtual void Serialize(Serializer& root);
                virtual void Deserialize(Serializer& root);

                std::string OtherMenu;
        };

        class ActivateAndDeactivate : public MenuItem {
            public:
                ActivateAndDeactivate(std::string image, std::string toActivate, std::string toDeactivate);

                virtual void Action();

                virtual void Serialize(Serializer& root);
                virtual void Deserialize(Serializer& root);

                std::string ToDeactivate;
                std::string ToActivate;
        };

        class ResumeGame : public MenuItem {
            public:
                ResumeGame(std::string image);

                virtual void Action();

                virtual void Serialize(Serializer& root);
                virtual void Deserialize(Serializer& root);
        };

    } // MenuItems

} // ForLeaseEngine

#endif // MENU_ITEMS_H