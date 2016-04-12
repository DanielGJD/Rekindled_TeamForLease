/*!
    \file   PauseMenu.cpp
    \author Sean McGeer
    \date   1/24/16

    \see PauseMenu.h

    \copyright ©Copyright 2016 DigiPen Institute of Technology, All Rights Reserved
*/

#include "State.h"
#include "PauseMenu.h"
#include "ComponentsInclude.h"
#include "ResourceManager.h"
#include "Vector.h"
#include "Face.h"
#include "Edge.h"
#include "Mesh.h"
#include "GameStateManager.h"
#include "Ray.h"
#include "SoundEmitter.h"

#include <iostream>
#include <string>

namespace FLE = ForLeaseEngine;
using namespace ForLeaseEngine;

PauseMenu::PauseMenu() : State("PauseMenu") {}

void PauseMenu::Load() {
}

void PauseMenu::Initialize() {
    FLE::LevelComponents::Renderer* renderer = new FLE::LevelComponents::Renderer(*this);
    FLE::Entity* camera = AddEntity("Camera");
    camera->AddComponent(new FLE::Components::Transform(*camera, FLE::Point(0, 0), 1, 1, 0));
    camera->AddComponent(new FLE::Components::Camera(*camera, 0, 1, 50));
    renderer->SetCamera(*camera);
    AddLevelComponent(renderer);
    AddLevelComponent(new LevelComponents::Light(*this, Color(0.5f, 0.5f, 0.5f)));
    //AddLevelComponent(new LevelComponents::Physics(*this, Vector(0, -10)));
    //AddLevelComponent(new LevelComponents::Collision(*this));


    Entity* background = AddEntity("Background");
    background->AddComponent(new Components::Transform(*background, 0, 0, 45, 25));
    background->AddComponent(new Components::Sprite(*background));
    //ForLease->Resources.LoadTexture("bg7.png");
    //Texture* texture = Texture::CreateTexture("bg7.png");
    //TextureRegion textureRegion(texture, 0, texture->GetWidth(), 0, texture->GetHeight());
    background->GetComponent<Components::Sprite>(true)->SetSpriteSource("bg7_test2.png");
    background->GetComponent<Components::Sprite>(true)->AnimationActive = false;

    Entity* backgroundMask = AddEntity("BackgroundMask");
    backgroundMask->AddComponent(new Components::Transform(*backgroundMask, 0, 0, 50, 50));
    backgroundMask->AddComponent(new Components::Model(*backgroundMask, true, false, false, "1-1Block.json", "", Color(0.75f, 0.75f, 0.75f), FLE::MULTIPLY));

    //Entity* logo = AddEntity("Logo");
    //logo->AddComponent(new Components::Transform(*logo, Point(0, 15), 30, 30));
    //logo->AddComponent(new Components::Sprite(*logo));
    //logo->GetComponent<Components::Sprite>(true)->SetSpriteSource("Title.png");
    //logo->GetComponent<Components::Sprite>(true)->AnimationActive = false;

    Entity* player = AddEntity("Player");
    player->AddComponent(new Components::Transform(*player, Point(15, -15), 15, 15));
    Components::Model* model = new Components::Model(*player, true, false, true, "fox.json");
    model->SetAnimation("a");
    model->AnimationActive = true;
    model->Looping = true;
    model->FrameRate = 4;
    player->AddComponent(model);

    Entity* follow = SpawnArchetype(ForLease->Filesystem.AssetDirectory(Modules::Filesystem::AssetType::Blueprint) + "Wisp", Point(-40.0f, -10.0f), "MenuFollow");
    follow->GetComponent<Components::Follow>()->Offset = Vector(-1.5f, -1.0f);
    follow->GetComponent<Components::Follow>()->Speed = 4;
    follow->GetComponent<Components::Light>()->Radius = 5.0f;

    Entity* creditsPic = AddEntity("CreditsPic");
    creditsPic->AddComponent(new Components::Transform(*creditsPic, Point(0, 0), 30.0f, 30.0f, 0.0f));
    creditsPic->AddComponent(new Components::Sprite(*creditsPic, "Credits.png", false));

    Entity* htpPic = AddEntity("HowToPlayPic");
    htpPic->AddComponent(new Components::Transform(*htpPic, Point(0, 0), 30.0f, 30.0f, 0.0f));
    htpPic->AddComponent(new Components::Sprite(*htpPic, "HowToPlay.png", false));

    Entity* menu = AddEntity("Menu");
    menu->AddComponent(new Components::Transform(*menu, Point(-35.0f, -6.0f)));
    menu->AddComponent(new Components::Menu(*menu, Vector(0, -1), true, 2.0f, 2.5f, "Liberation_Serif.fnt", "MenuFollow", Color(1, 1, 1), Color(1, 1, 0)));
    Components::Menu* menuComp = menu->GetComponent<Components::Menu>();
    menuComp->AddItem(new MenuItems::ResumeGame("Resume Game"));
    //menuComp->AddItem(new MenuItems::LoadLevel("How to Play", "HowToPlay"));
    //menuComp->AddItem(new MenuItems::LoadLevel("How to Play", "HowToPlay"));  // THIS NEEDS TO BE FIXED
    menuComp->AddItem(new MenuItems::ActivateAndDeactivateAndMakeVisible("How To Play", "HowToPlayMenu", "Menu", "HowToPlayPic"));
    menuComp->AddItem(new MenuItems::ActivateAndDeactivate("Options", "OptionsMenu", "Menu"));
    menuComp->AddItem(new MenuItems::ActivateAndDeactivateAndMakeVisible("Credits", "CreditsMenu", "Menu", "CreditsPic"));
    menuComp->AddItem(new MenuItems::ActivateAndDeactivate("Main Menu", "MainMenuConfirm", "Menu"));
    menuComp->AddItem(new MenuItems::ActivateAndDeactivate("Quit Game", "QuitConfirm", "Menu"));
    menuComp->Activate();

    Entity* opMenu = AddEntity("OptionsMenu");
    opMenu->AddComponent(new Components::Transform(*opMenu, Point(-35.0f, -6.0f)));
    opMenu->AddComponent(new Components::Menu(*opMenu, Vector(0, -1), false, 2.0f, 2.5f, "Liberation_Serif.fnt", "MenuFollow", Color(1, 1, 1), Color(1, 1, 0)));
    Components::Menu* opMenuComp = opMenu->GetComponent<Components::Menu>();
    opMenuComp->AddItem(new OptionMenuItems::Resolution());
    opMenuComp->AddItem(new OptionMenuItems::Fullscreen());
    opMenuComp->AddItem(new OptionMenuItems::Volume());
    opMenuComp->AddItem(new OptionMenuItems::FinalAccept("OptionsMenu"));
    opMenuComp->AddItem(new MenuItems::ActivateAndDeactivate("Back", "Menu", "OptionsMenu"));

    Entity* quitConfirm = AddEntity("QuitConfirm");
    quitConfirm->AddComponent(new Components::Transform(*quitConfirm, Point(-35.0f, -13.0f)));
    quitConfirm->AddComponent(new Components::Menu(*quitConfirm, Vector(0, -1), false, 2.0f, 2.5f, "Liberation_Serif.fnt", "MenuFollow", Color(1, 1, 1), Color(1, 1, 0)));
    Components::Menu* quitConfirmComp = quitConfirm->GetComponent<Components::Menu>();
    quitConfirmComp->AddItem(new MenuItems::Quit("Quit Game"));
    quitConfirmComp->AddItem(new MenuItems::ActivateAndDeactivate("Cancel", "Menu", "QuitConfirm"));

    Entity* creditsMenu = AddEntity("CreditsMenu");
    creditsMenu->AddComponent(new Components::Transform(*creditsMenu, Point(-35.0f, -13.0f)));
    creditsMenu->AddComponent(new Components::Menu(*creditsMenu, Vector(0, -1), false, 2.0f, 2.5f, "Liberation_Serif.fnt", "MenuFollow", Color(1, 1, 1), Color(1, 1, 0)));
    Components::Menu* creditsMenuComp = creditsMenu->GetComponent<Components::Menu>();
    //creditsMenuComp->AddItem(new MenuItems::Quit("Quit"));
    creditsMenuComp->AddItem(new MenuItems::ActivateAndDeactivateAndMakeInvisible("Back", "Menu", "CreditsMenu", "CreditsPic"));

    Entity* htpMenu = AddEntity("HowToPlayMenu");
    htpMenu->AddComponent(new Components::Transform(*htpMenu, Point(-35.0f, -13.0f)));
    htpMenu->AddComponent(new Components::Menu(*htpMenu, Vector(0, -1), false, 2.0f, 2.5f, "Liberation_Serif.fnt", "MenuFollow", Color(1, 1, 1), Color(1, 1, 0)));
    Components::Menu* htpMenuComp = htpMenu->GetComponent<Components::Menu>();
    //creditsMenuComp->AddItem(new MenuItems::Quit("Quit"));
    htpMenuComp->AddItem(new MenuItems::ActivateAndDeactivateAndMakeInvisible("Back", "Menu", "HowToPlayMenu", "HowToPlayPic"));

    Entity* mainMenuConfirm = AddEntity("MainMenuConfirm");
    mainMenuConfirm->AddComponent(new Components::Transform(*mainMenuConfirm, Point(-35.0f, -13.0f)));
    mainMenuConfirm->AddComponent(new Components::Menu(*mainMenuConfirm, Vector(0, -1), false, 2.0f, 2.5f, "Liberation_Serif.fnt", "MenuFollow", Color(1, 1, 1), Color(1, 1, 0)));
    Components::Menu* mainMenuConfirmComp = mainMenuConfirm->GetComponent<Components::Menu>();
    mainMenuConfirmComp->AddItem(new MenuItems::LoadLevel("Main Menu", "MainMenu"));
    mainMenuConfirmComp->AddItem(new MenuItems::ActivateAndDeactivate("Cancel", "Menu", "MainMenuConfirm"));

    ForLease->Dispatcher.Attach(NULL, this, "KeyDown", &PauseMenu::OnKeyDown);
}

void PauseMenu::Update() {

    ForLease->OSInput.ProcessAllInput();

    for (FLE::Entity* entity : Entities) {
        entity->Update();
    }

    for (FLE::LevelComponent* levelComponent : LevelComponents) {
        levelComponent->Update(Entities);
    }

    LevelComponents::Renderer* renderer = ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Renderer>();

    ForLease->GameWindow->UpdateGameWindow();
}

void PauseMenu::Deinitialize() {
    DeleteAllEntities();
    DeleteAllLevelComponents();
    ForLease->Dispatcher.Detach(this, "KeyDown");
}

void PauseMenu::Unload() {}

void PauseMenu::OnKeyDown(const Event* e) {
    const KeyboardEvent* key_e = static_cast<const KeyboardEvent*>(e);
    /*Entity * pause = AddEntity("Pause");
    Components::SoundEmitter * emitter = pause->GetComponent<Components::SoundEmitter>();*/
    if (key_e->Key == Keys::Escape)
        ForLease->GameStateManager().SetAction(Modules::StateAction::Continue);
    //if(ForLease->GameStateManager().GetCurrentAction() ==  Modules::StateAction::Freeze)
    //    emitter->BeQuiet();
    //    continueFromFreeze = true;
    //    //continueFromPause = false;
    //if(ForLease->GameStateManager().GetCurrentAction() == Modules::StateAction::Pause)
    //{
    //    //continueFromFreeze = false;
    //    continueFromPause = true;
    //    State &levelState = ForLease->GameStateManager().CurrentState();
    //    Entity * bgm = levelState.GetEntityByName("backgroundMusic", false);
    //    std::string soundname = bgm->GetComponent<Components::BackgroundMusic>()->MusicName;
    //    emitter->SetPause(true, soundname);
    //}
    //else if(ForLease->GameStateManager().GetCurrentAction() == Modules::StateAction::Continue)
    //{

    //    if(continueFromPause)
    //    {
    //        State &levelState = ForLease->GameStateManager().CurrentState();
    //        Entity * bgm = levelState.GetEntityByName("backgroundMusic", false);
    //        std::string soundname = bgm->GetComponent<Components::BackgroundMusic>()->MusicName;
    //        emitter->SetPause(false, soundname);
    //        continueFromPause = false;
    //    }
    //    if(continueFromFreeze)
    //    {
    //        emitter->Rock();
    //        continueFromFreeze = false;
    //    }
    //}


}
