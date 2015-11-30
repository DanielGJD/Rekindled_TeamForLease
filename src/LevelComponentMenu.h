/*!
    \file   ComponentMenu.h
    \author Sean McGeer
    \date   11/21/15

    \brief
        Defines the menu level component.

    \see ComponentMenu.h
    \see LevelComponentMenu.cpp

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef LC_MENU_H
#define LC_MENU_H

#include "LevelComponent.h"
#include "ComponentMenu.h"
#include "Engine.h"
#include "LevelComponentRenderer.h"
#include "Timer.h"

namespace ForLeaseEngine {

    namespace LevelComponents {

        class Menu : public LevelComponent {
            public:
                static const ComponentType Type = ComponentType::Menu;
                virtual ComponentType GetType() { return Type; }

                Menu(State& owner, double pauseCD = 0.2);

                void Serialize(Serializer& root);
                void Deserialize(Serializer& root);

                void Update(std::vector<Entity *>& entities);

                void OnKeyDown(const Event* e);

                void Pause();
                void Unpause();

            private:
                bool Paused;
                float LastTimeScale;
                Timer PauseTimer;
                double PauseCooldown;
        };

    } // LevelComponents

} // ForLeaseEngine

#endif // LC_MENU_H