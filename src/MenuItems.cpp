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

        ActivateAndDeactivateAndMakeVisibleAndMakeInvisible::ActivateAndDeactivateAndMakeVisibleAndMakeInvisible(std::string text, std::string toActivate, std::string toDeactivate, std::string toMakeVisible, std::string toMakeInvisible)
            : MenuItem(MenuItemType::ActDeactVisInvis, text),
            ToDeactivate(toDeactivate),
            ToActivate(toActivate),
            ToMakeVisible(toMakeVisible),
            ToMakeInvisible(toMakeInvisible) {}

        void ActivateAndDeactivateAndMakeVisibleAndMakeInvisible::Action() {
            Entity* actEnt = ForLease->GameStateManager().CurrentState().GetEntityByName(ToActivate, true);
            Components::Menu* activate = actEnt->GetComponent<Components::Menu>(true);

            Entity* deactEnt = ForLease->GameStateManager().CurrentState().GetEntityByName(ToDeactivate, true);
            Components::Menu* deactivate = deactEnt->GetComponent<Components::Menu>(true);

            deactivate->Deactivate();
            activate->Activate();

            Entity* visEnt = ForLease->GameStateManager().CurrentState().GetEntityByName(ToMakeVisible, true);
            Components::Sprite* vis = visEnt->GetComponent<Components::Sprite>(true);
            Entity* invisEnt = ForLease->GameStateManager().CurrentState().GetEntityByName(ToMakeInvisible, true);
            Components::Sprite* invis = invisEnt->GetComponent<Components::Sprite>(true);

            vis->Visible = true;
            invis->Visible = false;
        }

        RestartLevel::RestartLevel(std::string text) : MenuItem(MenuItemType::RestartLevel, text) {}

        void RestartLevel::Action() {
            ForLease->GameStateManager().SetAction(Modules::StateAction::Restart);
        }

    } // MenuItems

    OptionMenuItem::OptionMenuItem(MenuItemType type, std::string text, bool affectsWindow)
        : MenuItem(type, text, true), FirstText(text), AffectsWindow(affectsWindow) {}

    namespace OptionMenuItems {
        FinalAccept::FinalAccept(std::string parentMenuName, std::string text)
            : OptionMenuItem(MenuItemType::OptionAccept, text),
              ParentMenuName(parentMenuName) {}

        void FinalAccept::Action() {
            Entity* parentMenu = ForLease->GameStateManager().CurrentState().GetEntityByName(ParentMenuName);
            if (parentMenu) {
                Components::Menu* menu = parentMenu->GetComponent<Components::Menu>();
                Systems::WindowProperties windowProperties = ForLease->GameWindow->GetProperties();
                bool windowChanged = false;
                if (menu) {
                    for (MenuItem* item : menu->Items) {
                        if (item->Option) {
                            OptionMenuItem* option = reinterpret_cast<OptionMenuItem*>(item);
                            if (option->AffectsWindow && option->Dirty) windowChanged = true;
                            option->Accept(windowProperties);
                            option->Dirty = false;
                        }
                    }
                }
                if (windowChanged) {
                    ForLease->GameWindow->SetProperties(windowProperties);
                    ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Renderer>()->Reload();
                }
            }
        }

        void FinalAccept::Accept(Systems::WindowProperties& windowProperties) {}

        Resolution::ItemizedResolution::ItemizedResolution() : Index(0) {
            Resolutions.push_back(Res(1920, 1080));
//            Resolutions.push_back(Res(1366, 768));
//            Resolutions.push_back(Res(1280, 1024));
//            Resolutions.push_back(Res(1280, 800));
            Resolutions.push_back(Res(1280, 720));
//            Resolutions.push_back(Res(1024, 768));
//            Resolutions.push_back(Res(800, 600));
        }

        void Resolution::ItemizedResolution::SetIndex(int x, int y) {
            for (unsigned i = 0; i < Resolutions.size(); ++i) {
                if (Resolutions[i].X == x && Resolutions[i].Y == y) {
                    Index = i;
                    OriginalIndex = i;
                    return;
                }
            }

            Index = 0;
            OriginalIndex = 0;
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

        bool Resolution::ItemizedResolution::IsDirty() {
            return !(Index == OriginalIndex);
        }

        void Resolution::ItemizedResolution::ClearDirty() {
            OriginalIndex = Index;
        }

        Resolution::Resolution(std::string text)
            : OptionMenuItem(MenuItemType::OptionResolution, text, true),
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

            Dirty = Current.IsDirty();
        }

        void Resolution::Accept(Systems::WindowProperties& windowProperties) {
            ItemizedResolution::Res currentRes = Current.GetResolution();
            //ForLease->GameWindow->SetResolution(currentRes.X, currentRes.Y);
            //ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Renderer>()->Reload();
            windowProperties.xResolution = currentRes.X;
            windowProperties.yResolution = currentRes.Y;
            Current.ClearDirty();
            Dirty = Current.IsDirty();
        }

        Fullscreen::Fullscreen(std::string text)
            : OptionMenuItem(MenuItemType::OptionFullscreen, text, true)
        {
            IsFullscreen = ForLease->GameWindow->GetFullscreen();
            OriginalFullscreen = IsFullscreen;
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

            if (IsFullscreen != OriginalFullscreen)
                Dirty = true;
            else
                Dirty = false;
        }

        void Fullscreen::Accept(Systems::WindowProperties& windowProperties) {
            //ForLease->GameWindow->SetFullscreen(IsFullscreen);
            OriginalFullscreen = IsFullscreen;
            Dirty = false;
            windowProperties.fullscreen = IsFullscreen;
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

        void Volume::Accept(Systems::WindowProperties& windowProperties) {
            std::cout << CurrentVolume << std::endl;
        }

        ToggleAudio::ToggleAudio(std::string text) : OptionMenuItem(MenuItemType::OptionToggleAudio, text) {
            SetLocalMuting();
            OriginalMuting = CurrentMuting;
            SetText();
        }

        void ToggleAudio::SetLocalMuting() {
            Systems::SoundManager::CurrentMuting currentMuting = ForLease->sound->GetMuting();

            if (currentMuting.BackgroundMuted && currentMuting.EffectsMuted)
                CurrentMuting = Muting::All;
            else if (currentMuting.BackgroundMuted && !currentMuting.EffectsMuted)
                CurrentMuting = Muting::Music;
            else
                CurrentMuting = Muting::None;
        }

        void ToggleAudio::IncrementLocalMuting() {
            switch (CurrentMuting) {
                case Muting::None:
                    CurrentMuting = Muting::Music;
                    break;
                case Muting::Music:
                    CurrentMuting = Muting::All;
                    break;
                case Muting::All:
                    CurrentMuting = Muting::None;
                    break;
            }
        }

        void ToggleAudio::SetText() {            
            std::stringstream ss;
            ss << FirstText;

            if (CurrentMuting == Muting::All) {
                ss << "All Audio";
            } else if (CurrentMuting == Muting::Music) {
                ss << "Music/Ambience";
            } else if (CurrentMuting == Muting::None) {
                ss << "None";
            }

            Text = ss.str();
        }

        void ToggleAudio::Action() {
            IncrementLocalMuting();
            SetText();
            if (OriginalMuting != CurrentMuting)
                Dirty = true;
            else
                Dirty = false;
        }

        void ToggleAudio::Accept(Systems::WindowProperties& windowProperties) {
            ForLease->sound->UnmuteGlobal();

            if (CurrentMuting == Muting::None) {
                ForLease->sound->UnmuteGlobal();
            } else if (CurrentMuting == Muting::All) {
                ForLease->sound->MuteGlobal();
            } else if (CurrentMuting == Muting::Music) {
                ForLease->sound->MuteBackground();
            }

            OriginalMuting = CurrentMuting;

            Dirty = false;
        }

    } // OptionMenuItems

} // ForLeaseEngine
