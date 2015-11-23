/*!
    \file   ComponentMenu.h
    \author Sean McGeer
    \date   11/20/15

    \brief
        Defines the menu component.

    \see ComponentMenu.cpp
    \see MenuItems.h

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef COMPONENT_MENU_H
#define COMPONENT_MENU_H

#include "Component.h"
#include "Serializable.h"
#include "Serialize.h"
#include "MenuItems.h"
#include "Vector.h"
#include "Event.h"
#include "MouseMotionEvent.h"
#include <string>

namespace ForLeaseEngine {
    
    namespace Components {

        class Menu : public Component {

            public:
                static const ComponentType Type = ComponentType::Menu;
                virtual ComponentType GetType() { return Type; }

                Menu(Entity& owner, float unfocusedScale = 0.05, float focusedScale = 0.08, Vector spacing = Vector(0, -1), bool active = false);
                ~Menu();
                virtual void Update();

                void OnMouseMotion(const Event* e);
                void OnMouseDown(const Event* e);

                void Menu::AddItem(MenuItem* item);
                void Menu::AddLoadLevel(std::string image, std::string stateName);
                void Activate();
                void Deactivate();

                Entity* GetRepresentationAtPosition(Point position, bool throwOnFail = false);

                void Serialize(Serializer& root);
                void Deserialize(Serializer& root);
                MenuItem* DeserializeItem(Serializer& root);

                Vector Spacing;
                float UnfocusedScale;
                float FocusedScale;

            private:
                std::vector<Entity*> Representations;
                std::vector<MenuItem*> Items;
                Entity* LastActive;
                bool Active;

        };

    } // Components

} // ForLeaseEngine

#endif // COMPONENT_MENU_H