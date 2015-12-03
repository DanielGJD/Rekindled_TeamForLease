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

            long cameraID = ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Renderer>(true)->GetCameraID();
            Entity* camera = ForLease->GameStateManager().CurrentState().GetEntityByID(cameraID, true);
            float camScale = camera->GetComponent<Components::Camera>(true)->Size / 50;

            float unfocusedScale = 0.05 * camScale;
            float focusedScale = 0.08 * camScale;

            Entity* pauseMenu = owner.AddEntity("PauseMenu");
            pauseMenu->IncludeInSerialize = false;
            pauseMenu->AddComponent(new Components::Transform(*pauseMenu));
            Components::Menu* pauseMenuComp = new Components::Menu(*pauseMenu, Vector(0,-1), false, unfocusedScale, focusedScale);
            pauseMenu->AddComponent(pauseMenuComp);
            pauseMenuComp->AddItem(new MenuItems::ResumeGame("ButtonResume.png"));
            pauseMenuComp->AddItem(new MenuItems::ActivateAndDeactivate("ButtonHowTo.png", "HowToConfirm", "PauseMenu"));
            pauseMenuComp->AddItem(new MenuItems::ActivateAndDeactivate("ButtonQuit.png", "QuitConfirm", "PauseMenu"));

            Entity* quitConfirm = owner.AddEntity("QuitConfirm");
            quitConfirm->IncludeInSerialize = false;
            quitConfirm->AddComponent(new Components::Transform(*quitConfirm));
            Components::Menu* quitConfirmComp = new Components::Menu(*quitConfirm, Vector(0, -1), false, unfocusedScale, focusedScale);
            quitConfirm->AddComponent(quitConfirmComp);
            quitConfirmComp->AddItem(new MenuItems::Quit("ButtonQuit.png"));
            quitConfirmComp->AddItem(new MenuItems::ActivateAndDeactivate("ButtonCancel.png", "PauseMenu", "QuitConfirm"));
            

            Entity* howToConfirm = owner.AddEntity("HowToConfirm");
            howToConfirm->IncludeInSerialize = false;
            howToConfirm->AddComponent(new Components::Transform(*howToConfirm));
            Components::Menu* howToConfirmComp = new Components::Menu(*howToConfirm, Vector(0, -1), false, unfocusedScale, focusedScale);
            howToConfirm->AddComponent(howToConfirmComp);
            howToConfirmComp->AddItem(new MenuItems::LoadLevel("ButtonHowTo.png", "HowToPlay"));
            howToConfirmComp->AddItem(new MenuItems::ActivateAndDeactivate("ButtonCancel.png", "PauseMenu", "HowToConfirm"));
            
            
            ForLease->Dispatcher.Attach(NULL, this, "KeyDown", &Menu::OnKeyDown);
        }

        Menu::~Menu() {
            std::vector<Entity *> menus;

            menus.push_back(ForLease->GameStateManager().CurrentState().GetEntityByName("PauseMenu"));
            menus.push_back(ForLease->GameStateManager().CurrentState().GetEntityByName("QuitConfirm"));
            menus.push_back(ForLease->GameStateManager().CurrentState().GetEntityByName("HowToConfirm"));

            for (Entity* menu : menus) {
                if (menu)
                    menu->GetComponent<Components::Menu>(true)->Deactivate();
            }

            ForLease->Dispatcher.Detach(this, "KeyDown");
        }

        void Menu::Update(std::vector<Entity *>& entities) {
            std::vector<Entity *> menus;

            menus.push_back(ForLease->GameStateManager().CurrentState().GetEntityByName("PauseMenu"));
            menus.push_back(ForLease->GameStateManager().CurrentState().GetEntityByName("QuitConfirm"));
            menus.push_back(ForLease->GameStateManager().CurrentState().GetEntityByName("HowToConfirm"));
            
            unsigned long camID = ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Renderer>()->GetCameraID();
            Entity* camera = ForLease->GameStateManager().CurrentState().GetEntityByID(camID);
            if (!camera) return;
            Components::Transform* camTransform = camera->GetComponent<Components::Transform>();
            if (!camTransform) return;

            for (Entity* menu : menus) {
                Components::Transform* transform = menu->GetComponent<Components::Transform>();
                if (transform) transform->Position = camTransform->Position;
            }
        }

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