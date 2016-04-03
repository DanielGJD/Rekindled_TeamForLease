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
#include "State.h"

namespace ForLeaseEngine {

    MenuItem::MenuItem(MenuItemType type, std::string text, bool option)
        : Text(text), Type(type), Option(option) {}

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
            loadLevel.WriteString("Text", Text);
            loadLevel.WriteString("StateName", StateName);
            loadLevel.WriteUint("Type", static_cast<unsigned>(Type));
            root.Append(loadLevel, "LoadLevel");
        }

        void LoadLevel::Deserialize(Serializer& root) {
            Serializer loadLevel = root.GetChild("LoadLevel");
            loadLevel.ReadString("Text", Text);
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
            nextLevel.WriteString("Text", Text);
            nextLevel.WriteUint("Type", static_cast<unsigned>(Type));
            root.Append(nextLevel, "NextLevel");
        }

        void NextLevel::Deserialize(Serializer& root) {
            Serializer nextLevel = root.GetChild("NextLevel");
            nextLevel.ReadString("Text", Text);
        }

        // ==================================================================================

        Quit::Quit(std::string image) : MenuItem(MenuItemType::Quit, image) {}

        void Quit::Action() {
            ForLease->GameStateManager().SetAction(Modules::StateAction::Quit);
        }

        void Quit::Serialize(Serializer& root) {
            root.WriteUint("Type", static_cast<unsigned>(Type));
            Serializer quit = root.GetChild("Quit");
            quit.WriteString("Text", Text);
            quit.WriteUint("Type", static_cast<unsigned>(Type));
            root.Append(quit, "Quit");
        }

        void Quit::Deserialize(Serializer& root) {
            Serializer quit = root.GetChild("Quit");
            quit.ReadString("Text", Text);
        }

        // ==================================================================================

        ActivateOther::ActivateOther(std::string image, std::string otherMenu) 
            : MenuItem(MenuItemType::ActivateOther, image), OtherMenu(otherMenu) {}

        void ActivateOther::Action() {
            Entity* otherEnt = ForLease->GameStateManager().CurrentState().GetEntityByName(OtherMenu, true);
            Components::Menu* other = otherEnt->GetComponent<Components::Menu>(true);

            other->Activate();
        }

        void ActivateOther::Serialize(Serializer& root) {
            root.WriteUint("Type", static_cast<unsigned>(Type));
            Serializer activateOther = root.GetChild("ActivateOther");
            activateOther.WriteString("Text", Text);
            activateOther.WriteString("OtherMenu", OtherMenu);
            activateOther.WriteUint("Type", static_cast<unsigned>(Type));
            root.Append(activateOther, "ActivateOther");
        }

        void ActivateOther::Deserialize(Serializer& root) {
            Serializer activateOther = root.GetChild("ActivateOther");
            activateOther.ReadString("Text", Text);
            activateOther.ReadString("OtherMenu", OtherMenu);
        }

        // ==================================================================================

        ActivateAndDeactivate::ActivateAndDeactivate(std::string image, std::string toActivate, std::string toDeactivate)
            : MenuItem(MenuItemType::ActivateAndDeactivate, image), ToDeactivate(toDeactivate), ToActivate(toActivate) {}

        void ActivateAndDeactivate::Action() {
            Entity* actEnt = ForLease->GameStateManager().CurrentState().GetEntityByName(ToActivate, true);
            Components::Menu* activate = actEnt->GetComponent<Components::Menu>(true);

            Entity* deactEnt = ForLease->GameStateManager().CurrentState().GetEntityByName(ToDeactivate, true);
            Components::Menu* deactivate = deactEnt->GetComponent<Components::Menu>(true);

            deactivate->Deactivate();
            activate->Activate();
        }

        void ActivateAndDeactivate::Serialize(Serializer& root) {
            root.WriteUint("Type", static_cast<unsigned>(Type));
            Serializer actAndDeact = root.GetChild("ActivateAndDeactivate");
            actAndDeact.WriteString("Text", Text);
            actAndDeact.WriteString("ToDeactivate", ToDeactivate);
            actAndDeact.WriteString("ToActivate", ToActivate);
            actAndDeact.WriteUint("Type", static_cast<unsigned>(Type));
            root.Append(actAndDeact, "ActivateAndDeactivate");
        }

        void ActivateAndDeactivate::Deserialize(Serializer& root) {
            Serializer activateOther = root.GetChild("ActivateAndDeactivate");
            activateOther.ReadString("Text", Text);
            activateOther.ReadString("ToDeactivate", ToDeactivate);
            activateOther.ReadString("ToActivate", ToActivate);
        }

        // ==================================================================================

        ResumeGame::ResumeGame(std::string image) : MenuItem(MenuItemType::ResumeGame, image) {}

        void ResumeGame::Action() {
            //LevelComponents::Menu* pauseMenu = ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Menu>(true);
            //pauseMenu->Unpause();
            ForLease->GameStateManager().SetAction(Modules::StateAction::Continue);
        }

        void ResumeGame::Serialize(Serializer& root) {
            root.WriteUint("Type", static_cast<unsigned>(Type));
            Serializer resume = root.GetChild("ResumeGame");
            resume.WriteString("Text", Text);
            resume.WriteUint("Type", static_cast<unsigned>(Type));
            root.Append(resume, "ResumeGame");
        }

        void ResumeGame::Deserialize(Serializer& root) {
            Serializer resume = root.GetChild("ResumeGame");
            resume.ReadString("Text", Text);
        }

        // ==================================================================================

        ActivateAndDeactivateAndMakeInvisible::ActivateAndDeactivateAndMakeInvisible(std::string image, std::string toActivate, std::string toDeactivate, std::string toMakeInvisible)
            : MenuItem(MenuItemType::ActDeactInvisible, image), ToDeactivate(toDeactivate), ToActivate(toActivate), ToMakeInvisible(toMakeInvisible) {}

        void ActivateAndDeactivateAndMakeInvisible::Action() {
            Entity* actEnt = ForLease->GameStateManager().CurrentState().GetEntityByName(ToActivate, true);
            Components::Menu* activate = actEnt->GetComponent<Components::Menu>(true);

            Entity* deactEnt = ForLease->GameStateManager().CurrentState().GetEntityByName(ToDeactivate, true);
            Components::Menu* deactivate = deactEnt->GetComponent<Components::Menu>(true);

            deactivate->Deactivate();
            activate->Activate();

            Entity* invisEnt = ForLease->GameStateManager().CurrentState().GetEntityByName(ToMakeInvisible, true);
            Components::Sprite* invis = invisEnt->GetComponent<Components::Sprite>(true);

            invis->Visible = false;
        }

        void ActivateAndDeactivateAndMakeInvisible::Serialize(Serializer& root) {
            root.WriteUint("Type", static_cast<unsigned>(Type));
            Serializer actAndDeact = root.GetChild("ActivateAndDeactivateAndMakeInvisible");
            actAndDeact.WriteString("Text", Text);
            actAndDeact.WriteString("ToDeactivate", ToDeactivate);
            actAndDeact.WriteString("ToActivate", ToActivate);
            actAndDeact.WriteString("ToMakeInvisible", ToMakeInvisible);
            actAndDeact.WriteUint("Type", static_cast<unsigned>(Type));
            root.Append(actAndDeact, "ActivateAndDeactivateAndMakeInvisible");
        }

        void ActivateAndDeactivateAndMakeInvisible::Deserialize(Serializer& root) {
            Serializer activateOther = root.GetChild("ActivateAndDeactivateAndMakeInvisible");
            activateOther.ReadString("Text", Text);
            activateOther.ReadString("ToDeactivate", ToDeactivate);
            activateOther.ReadString("ToActivate", ToActivate);
            activateOther.ReadString("ToMakeInvisible", ToMakeInvisible);
        }

        // ==================================================================================

        ActivateAndDeactivateAndMakeVisible::ActivateAndDeactivateAndMakeVisible(std::string image, std::string toActivate, std::string toDeactivate, std::string toMakeVisible)
            : MenuItem(MenuItemType::ActDeactVisible, image), ToDeactivate(toDeactivate), ToActivate(toActivate), ToMakeVisible(toMakeVisible) {}

        void ActivateAndDeactivateAndMakeVisible::Action() {
            Entity* actEnt = ForLease->GameStateManager().CurrentState().GetEntityByName(ToActivate, true);
            Components::Menu* activate = actEnt->GetComponent<Components::Menu>(true);

            Entity* deactEnt = ForLease->GameStateManager().CurrentState().GetEntityByName(ToDeactivate, true);
            Components::Menu* deactivate = deactEnt->GetComponent<Components::Menu>(true);

            deactivate->Deactivate();
            activate->Activate();

            Entity* visEnt = ForLease->GameStateManager().CurrentState().GetEntityByName(ToMakeVisible, true);
            Components::Sprite* vis = visEnt->GetComponent<Components::Sprite>(true);

            vis->Visible = true;
        }

        void ActivateAndDeactivateAndMakeVisible::Serialize(Serializer& root) {
            root.WriteUint("Type", static_cast<unsigned>(Type));
            Serializer actAndDeact = root.GetChild("ActivateAndDeactivateAndMakeVisible");
            actAndDeact.WriteString("Text", Text);
            actAndDeact.WriteString("ToDeactivate", ToDeactivate);
            actAndDeact.WriteString("ToActivate", ToActivate);
            actAndDeact.WriteString("ToMakeVisible", ToMakeVisible);
            actAndDeact.WriteUint("Type", static_cast<unsigned>(Type));
            root.Append(actAndDeact, "ActivateAndDeactivateAndMakeVisible");
        }

        void ActivateAndDeactivateAndMakeVisible::Deserialize(Serializer& root) {
            Serializer activateOther = root.GetChild("ActivateAndDeactivateAndMakeVisible");
            activateOther.ReadString("Text", Text);
            activateOther.ReadString("ToDeactivate", ToDeactivate);
            activateOther.ReadString("ToActivate", ToActivate);
            activateOther.ReadString("ToMakeVisible", ToMakeVisible);
        }

    } // MenuItems

    OptionMenuItem::OptionMenuItem(MenuItemType type, std::string text)
        : MenuItem(type, text, true), FirstText(text) {}

    namespace OptionMenuItems {
        FinalAccept::FinalAccept(std::string parentMenuName, std::string text)
            : OptionMenuItem(MenuItemType::OptionAccept, text),
              ParentMenuName(parentMenuName) {}

        void FinalAccept::Action() {
            Entity* parentMenu = ForLease->GameStateManager().CurrentState().GetEntityByName(ParentMenuName);
            if (parentMenu) {
                Components::Menu* menu = parentMenu->GetComponent<Components::Menu>();
                if (menu) {
                    for (MenuItem* item : menu->Items) {
                        if (item->Option) {
                            OptionMenuItem* option = reinterpret_cast<OptionMenuItem*>(item);
                            if (option->Dirty) option->Accept();
                        }
                    }
                }
            }
        }

        void FinalAccept::Accept() {}

        Resolution::ItemizedResolution::ItemizedResolution() : Index(0) {
            Resolutions.push_back(Res(1920, 1080));
            Resolutions.push_back(Res(1280, 720));
            Resolutions.push_back(Res(1366, 768));
            Resolutions.push_back(Res(1280, 1024));
            Resolutions.push_back(Res(1280, 800));
            Resolutions.push_back(Res(1024, 768));
            Resolutions.push_back(Res(800, 600));
        }

        void Resolution::ItemizedResolution::SetIndex(int x, int y) {
            for (unsigned i = 0; i < Resolutions.size(); ++i) {
                if (Resolutions[i].X == x && Resolutions[i].Y == y) {
                    Index = i;
                    return;
                }
            }

            Index = 0;
            return;
        }

        void Resolution::ItemizedResolution::IncrementIndex() {
            ++Index;
            if (Index >= Resolutions.size()) {
                Index = 0;
            }
        }

        Resolution::ItemizedResolution::Res Resolution::ItemizedResolution::GetNextResolution() {
            IncrementIndex();
            return GetResolution();
        }

        Resolution::ItemizedResolution::Res Resolution::ItemizedResolution::GetResolution() {
            return Resolutions[Index];
        }

        Resolution::Resolution(std::string text)
            : OptionMenuItem(MenuItemType::OptionResolution, text),
              Current()
        {
            Current.SetIndex(ForLease->GameWindow->GetXResolution(), ForLease->GameWindow->GetYResolution());

            SetText();
        }

        void Resolution::SetText() {
            std::stringstream ss;
            ss << FirstText;
            ItemizedResolution::Res currentRes = Current.GetResolution();
            ss << currentRes.X << "x" << currentRes.Y;
            Text = ss.str();
        }

        void Resolution::Action() {
            Current.IncrementIndex();
            SetText();

            Dirty = true;
        }

        void Resolution::Accept() {
            ItemizedResolution::Res currentRes = Current.GetResolution();
            ForLease->GameWindow->SetResolution(currentRes.X, currentRes.Y);
        }

        Fullscreen::Fullscreen(std::string text)
            : OptionMenuItem(MenuItemType::OptionFullscreen, text)
        {
            IsFullscreen = ForLease->GameWindow->GetFullscreen();
            SetText();
        }

        void Fullscreen::SetText() {
            std::stringstream ss;
            ss << FirstText;
            if (IsFullscreen) {
                ss << "On";
            } else {
                ss << "Off";
            }
            Text = ss.str();
        }

        void Fullscreen::Action() {
            IsFullscreen = !IsFullscreen;
            SetText();

            Dirty = true;
        }

        void Fullscreen::Accept() {
            ForLease->GameWindow->SetFullscreen(IsFullscreen);
        }

        Volume::Volume(std::string text, unsigned volumeIncrement, unsigned maxVolume)
            : OptionMenuItem(MenuItemType::OptionVolume, text),
              CurrentVolume(100),
              VolumeIncrement(volumeIncrement),
              MaxVolume(maxVolume)
        {
            SetText();
        }

        void Volume::SetText() {
            std::stringstream ss;
            ss << FirstText;
            ss << CurrentVolume;
            Text = ss.str();
        }

        void Volume::Action() {
            CurrentVolume += VolumeIncrement;
            if (CurrentVolume > MaxVolume) {
                CurrentVolume = 0;
            }

            Dirty = true;

            SetText();
        }

        void Volume::Accept() {
            std::cout << CurrentVolume << std::endl;
        }

    } // OptionMenuItems

} // ForLeaseEngine