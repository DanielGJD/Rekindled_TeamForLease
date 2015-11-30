/*!
    \file   ComponentMenu.h
    \author Sean McGeer
    \date   11/21/15

    \brief
        Defines the menu level component.

    \see ComponentMenu.h
    \see LevelComponentMenu.h

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "LevelComponentMenu.h"
#include "Engine.h"

namespace ForLeaseEngine {

    namespace LevelComponents {

        Menu::Menu(State& owner, double pauseCD)
        : LevelComponent(owner), Paused(false), PauseTimer("Pause Timer"), PauseCooldown(pauseCD) {
            PauseTimer.Reset();

            Entity* pauseMenu = owner.AddEntity("PauseMenu");
            pauseMenu->IncludeInSerialize = false;
            pauseMenu->AddComponent(new Components::Transform(*pauseMenu));
            Components::Menu* pauseMenuComp = new Components::Menu(*pauseMenu);
            pauseMenu->AddComponent(pauseMenuComp);
            pauseMenuComp->AddItem(new MenuItems::ResumeGame("ButtonResume.png"));
            pauseMenuComp->AddItem(new MenuItems::ActivateAndDeactivate("ButtonHowTo.png", "HowToConfirm", "PauseMenu"));
            pauseMenuComp->AddItem(new MenuItems::ActivateAndDeactivate("ButtonQuit.png", "QuitConfirm", "PauseMenu"));

            Entity* quitConfirm = owner.AddEntity("QuitConfirm");
            quitConfirm->IncludeInSerialize = false;
            quitConfirm->AddComponent(new Components::Transform(*quitConfirm));
            Components::Menu* quitConfirmComp = new Components::Menu(*quitConfirm);
            quitConfirm->AddComponent(quitConfirmComp);
            quitConfirmComp->AddItem(new MenuItems::Quit("ButtonQuit.png"));
            quitConfirmComp->AddItem(new MenuItems::ActivateAndDeactivate("ButtonCancel.png", "PauseMenu", "QuitConfirm"));
            

            Entity* howToConfirm = owner.AddEntity("HowToConfirm");
            howToConfirm->IncludeInSerialize = false;
            howToConfirm->AddComponent(new Components::Transform(*howToConfirm));
            Components::Menu* howToConfirmComp = new Components::Menu(*howToConfirm);
            howToConfirm->AddComponent(howToConfirmComp);
            howToConfirmComp->AddItem(new MenuItems::Quit("ButtonQuit.png"));
            howToConfirmComp->AddItem(new MenuItems::ActivateAndDeactivate("ButtonCancel.png", "PauseMenu", "HowToConfirm"));
            
            
            ForLease->Dispatcher.Attach(NULL, this, "KeyDown", &Menu::OnKeyDown);
        }

        void Menu::Update(std::vector<Entity *>& entities) { }

        void Menu::Serialize(Serializer& root) {
            root.WriteUint("Type", static_cast<unsigned>(ComponentType::Menu));
            Serializer menu = root.GetChild("Menu");
            menu.WriteUint("Type", static_cast<unsigned>(ComponentType::Menu));
            root.Append(menu, "Menu");
        }

        void Menu::Deserialize(Serializer& root) {
            Serializer menu = root.GetChild("Menu");
        }

        void Menu::OnKeyDown(const Event* e) {
            if (PauseTimer.GetTime() > PauseCooldown) {
                PauseTimer.Reset();
                const KeyboardEvent* key_e = static_cast<const KeyboardEvent*>(e);
                if (key_e->Key == Keys::Escape) {
                    Paused ? Unpause() : Pause();
                }
            }
        }

        void Menu::Pause() {
            std::cout << std::endl << "HERE" << std::endl << std::endl;
            Entity* pauseMenu = ForLease->GameStateManager().CurrentState().GetEntityByName("PauseMenu");
            Components::Menu* pauseMenuComp = pauseMenu->GetComponent<Components::Menu>();
            pauseMenuComp->Activate();
            LastTimeScale = ForLease->FrameRateController().TimeScaling();
            ForLease->FrameRateController().TimeScaling(0);
            Paused = true;
        }

        void Menu::Unpause() {
            std::vector<Entity *> menus;

            menus.push_back(ForLease->GameStateManager().CurrentState().GetEntityByName("PauseMenu"));
            menus.push_back(ForLease->GameStateManager().CurrentState().GetEntityByName("QuitConfirm"));
            menus.push_back(ForLease->GameStateManager().CurrentState().GetEntityByName("HowToConfirm"));

            for (Entity* menu : menus) {
                Components::Menu* menuComp = menu->GetComponent<Components::Menu>();
                if (menuComp) menuComp->Deactivate();
            }
            
            ForLease->FrameRateController().TimeScaling(1);
            Paused = false;
        }

    } // LevelComponents

} // ForLeaseEngine