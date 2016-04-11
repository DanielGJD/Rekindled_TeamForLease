/*!
    \file   MainMenu.cpp
    \author Sean McGeer
    \date   11/23/15

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "MainMenu.h"
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

MainMenu::MainMenu() : State("MainMenu") , MainMenuBGM("woody title 7.5") {}

void MainMenu::Load() {
}

void MainMenu::Initialize() {
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


    //SoundEmitter* emitter = background.GetComponent<SoundEmitter>();
//    if(emitter)
//    {
//        emitter->SetVolume(1.0f, MainMenuBGM);
//        emitter->StopEvent(MainMenuBGM);
//        emitter->PlayEvent(MainMenuBGM);
//        std::cout<< "MENU2 EMITTER HERE" << std::endl;
//    }
//    if(!emitter)
//    {
//        std::cout << "no menu sound" <<std::endl;
//    }
   // background->GetComponent<Components::SoundEmitter>(true)->SetVolume(1.0f, "Menu2");
    //background->GetComponent<Components::SoundEmitter>(true)->StopEvent("Menu2");
    //background->GetComponent<Components::SoundEmitter>(true)->PlayEvent("Menu2");
    //ForLease->Resources.LoadTexture("bg7.png");
    //Texture* texture = Texture::CreateTexture("bg7.png");
    //TextureRegion textureRegion(texture, 0, texture->GetWidth(), 0, texture->GetHeight());
    //background->GetComponent<Components::Sprite>(true)->SetSpriteSource("bg7.png");
    //background->GetComponent<Components::Sprite>(true)->AnimationActive = false;

    Entity* player = AddEntity("Player");
    player->AddComponent(new Components::Transform(*player, Point(15, -15), 15, 15));
    Components::Model* model = new Components::Model(*player, true, false, true, "fox.json");
    model->SetAnimation("a");
    model->AnimationActive = true;
    model->Looping = true;
    model->FrameRate = 4;
    player->AddComponent(model);

    Entity* logo = AddEntity("Logo");
    logo->AddComponent(new Components::Transform(*logo, Point(0, 15), 30, 30));
    logo->AddComponent(new Components::Sprite(*logo));
    logo->GetComponent<Components::Sprite>(true)->SetSpriteSource("Title.png");
    logo->GetComponent<Components::Sprite>(true)->AnimationActive = false;

    Entity* follow = SpawnArchetype(ForLease->Filesystem.AssetDirectory(Modules::Filesystem::AssetType::Blueprint) + "Wisp", Point(-40.0f, -10.0f), "MenuFollow");
    follow->GetComponent<Components::Follow>()->Offset = Vector(-1.5f, -1.0f);
    follow->GetComponent<Components::Follow>()->Speed = 4;
    follow->GetComponent<Components::Light>()->Radius = 5.0f;

    Entity* menu = AddEntity("Menu");
    menu->AddComponent(new Components::Transform(*menu, Point(-40.0f, -10.0f)));
    menu->AddComponent(new Components::Menu(*menu, Vector(0, -1), true, 2.0f, 2.5f, "Liberation_Serif.fnt", "MenuFollow", Color(1, 1, 1), Color(1, 1, 0)));
    Components::Menu* menuComp = menu->GetComponent<Components::Menu>();
    menuComp->AddItem(new MenuItems::NextLevel("Play"));
    menuComp->AddItem(new MenuItems::ActivateAndDeactivate("Options", "OptionsMenu", "Menu"));
    menuComp->AddItem(new MenuItems::LoadLevel("How to Play", "HowToPlay"));
    menuComp->AddItem(new MenuItems::LoadLevel("Credits", "Credits"));
    menuComp->AddItem(new MenuItems::ActivateAndDeactivate("Quit", "QuitConfirm", "Menu"));
    menuComp->Activate();

    Entity* opMenu = AddEntity("OptionsMenu");
    opMenu->AddComponent(new Components::Transform(*opMenu, Point(-40.0f, -10.0f)));
    opMenu->AddComponent(new Components::Menu(*opMenu, Vector(0, -1), false, 2.0f, 2.5f, "Liberation_Serif.fnt", "MenuFollow", Color(1, 1, 1), Color(1, 1, 0)));
    Components::Menu* opMenuComp = opMenu->GetComponent<Components::Menu>();
    opMenuComp->AddItem(new OptionMenuItems::Resolution());
    opMenuComp->AddItem(new OptionMenuItems::Fullscreen());
    opMenuComp->AddItem(new OptionMenuItems::Volume());
    opMenuComp->AddItem(new OptionMenuItems::FinalAccept("OptionsMenu"));
    opMenuComp->AddItem(new MenuItems::ActivateAndDeactivate("Back", "Menu", "OptionsMenu"));

    Entity* quitConfirm = AddEntity("QuitConfirm");
    quitConfirm->AddComponent(new Components::Transform(*quitConfirm, Point(-40.0f, -10.0f)));
    quitConfirm->AddComponent(new Components::Menu(*quitConfirm, Vector(0, -1), false, 2.0f, 2.5f, "Liberation_Serif.fnt", "MenuFollow", Color(1, 1, 1), Color(1, 1, 0)));
    Components::Menu* quitConfirmComp = quitConfirm->GetComponent<Components::Menu>();
    quitConfirmComp->AddItem(new MenuItems::Quit("Quit"));
    quitConfirmComp->AddItem(new MenuItems::ActivateAndDeactivate("Cancel", "Menu", "QuitConfirm"));

    Entity * title = AddEntity("Title");
    //Components::SoundEmitter* emitter = title->GetComponent<Components::SoundEmitter>();
    title->AddComponent(new Components::Transform(*title, 0,0, 45, 25));
    title->AddComponent(new Components::SoundEmitter(*title));
    title->GetComponent<Components::SoundEmitter>(title)->SetVolume(1.0f, "Menu2");
    title->GetComponent<Components::SoundEmitter>(title)->StopEvent("Menu2");
    title->GetComponent<Components::SoundEmitter>(title)->PlayEvent("Menu2");
    std::cout<< "BGMMMMMMMMMMMMMM" <<std::endl;
}

void MainMenu::Update() {

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

void MainMenu::Deinitialize() {
    DeleteAllEntities();
    DeleteAllLevelComponents();
}

void MainMenu::Unload() {}
