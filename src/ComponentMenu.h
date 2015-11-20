/*!
    \file   ComponentMenu.h
    \author Sean McGeer
    \date   11/20/15

    \brief
        Defines the menu component.

    \see ComponentMenu.cpp
    \see MenuItems.h
*/

#ifndef COMPONENT_MENU_H
#define COMPONENT_MENU_H

#include "Component.h"
#include "Serializable.h"
#include "Serialize.h"
#include "MenuItems.h"
#include <string>

namespace ForLeaseEngine {
    
    namespace Components {

        class Menu : public Component {

            public:
                static const ComponentType Type = ComponentType::Menu;
                virtual ComponentType GetType() { return Type; }

                void Serialize(Serializer& root);
                void Deserialize(Serializer& root);

            private:
                std::vector<MenuItem> Items;

        };

    } // Components

} // ForLeaseEngine

#endif // COMPONENT_MENU_H