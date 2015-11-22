/*!
    \file   MenuItems.h
    \author Sean McGeer
    \date   11/20/15

    \brief
        Defines menu items.

    \see ComponentMenu.h
    \see MenuItems.cpp
*/

#ifndef MENU_ITEMS_H
#define MENU_ITEMS_H

#include "Serializable.h"
#include <string>

namespace ForLeaseEngine {

    enum class MenuItemType : unsigned {
        None = 0,
        LoadLevel = 1
    };

    class MenuItem : public Serializable {
        public:
            MenuItem(MenuItemType type, std::string text);
            std::string Text;
            virtual void Action() { }
            virtual void Serialize(Serializer& root);
            virtual void Deserialize(Serializer& root);
        private:
            MenuItemType Type;

    };

    namespace MenuItems {

        class LoadLevel : public MenuItem {
            public:
                LoadLevel(std::string text, std::string stateName);
                virtual void Action();

                virtual void Serialize(Serializer& root);
                virtual void Deserialize(Serializer& root);

                std::string StateName;
        };

    } // MenuItems

} // ForLeaseEngine

#endif // MENU_ITEMS_H