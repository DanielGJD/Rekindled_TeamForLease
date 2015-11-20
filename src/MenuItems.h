/*!
    \file   MenuItems.h
    \author Sean McGeer
    \date   11/20/15

    \brief
        Defines menu items.

    \see ComponentMenu.h
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
            MenuItem(std::string text, MenuItemType type);
            std::string Text;
            virtual void operator() () { }
            virtual void Serialize(Serializer& root);
            virtual void Deserialize(Serializer& root);
        private:
            MenuItemType Type;

    };

    namespace MenuItems {

        class LoadLevel : public MenuItem {
            public:
                LoadLevel(std::string text, std::string stateName);
                virtual void operator() ();

                virtual void Serialize(Serializer& root);
                virtual void Deserialize(Serializer& root);

            private:
                std::string StateName;
        };

    } // MenuItems

} // ForLeaseEngine

#endif // MENU_ITEMS_H