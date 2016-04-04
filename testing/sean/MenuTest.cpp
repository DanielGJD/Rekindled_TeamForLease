/*!
    \file   MenuTest.cpp
    \author Sean McGeer
    \date   4/03/16

    \see MenuTest.h

    \copyright ©Copyright 2016 DigiPen Institute of Technology, All Rights Reserved
*/

#include "State.h"
#include "ComponentsInclude.h"
#include "ResourceManager.h"
#include "Vector.h"
#include "Face.h"
#include "Edge.h"
#include "Mesh.h"
#include "GameStateManager.h"
#include "Ray.h"
#include "MenuTest.h"

#include <iostream>
#include <string>

namespace FLE = ForLeaseEngine;
using namespace ForLeaseEngine;

MenuTest::MenuTest() : State("MenuTest") {}

void MenuTest::Load() {
}

void MenuTest::Initialize() {
    FLE::LevelComponents::Renderer* renderer = new FLE::LevelComponents::Renderer(*this);
    FLE::Entity* camera = AddEntity("Camera");
    camera->AddComponent(new FLE::Components::Transform(*camera, FLE::Point(0, 0), 1, 1, 0));
    camera->AddComponent(new FLE::Components::Camera(*camera, 0, 1, 50));
    renderer->SetCamera(*camera);
    AddLevelComponent(renderer);
    //AddLevelComponent(new LevelComponents::Physics(*this, Vector(0, -10)));
    //AddLevelComponent(new LevelComponents::Collision(*this));


    Entity* background = AddEntity("Background");
    background->AddComponent(new Components::Transform(*background, 0, 0, 50, 50));
    background->AddComponent(new Components::Sprite(*background));
    //ForLease->Resources.LoadTexture("bg7.png");
    //Texture* texture = Texture::CreateTexture("bg7.png");
    //TextureRegion textureRegion(texture, 0, texture->GetWidth(), 0, texture->GetHeight());
    background->GetComponent<Components::Sprite>(true)->SetSpriteSource("bg7.png");
    background->GetComponent<Components::Sprite>(true)->AnimationActive = false;

    Entity* backgroundMask = AddEntity("BackgroundMask");
    backgroundMask->AddComponent(new Components::Transform(*backgroundMask, 0, 0, 50, 50));
    background->AddComponent(new Components::Model(*backgroundMask, true, false, false, "1-1Block.json", "", Color(0,1,0,0.5f), ForLeaseEngine::MULTIPLY));

    Entity* logo = AddEntity("Logo");
    logo->AddComponent(new Components::Transform(*logo, Point(0, 15), 30, 30));
    logo->AddComponent(new Components::Sprite(*logo));
    logo->GetComponent<Components::Sprite>(true)->SetSpriteSource("Title.png");
    logo->GetComponent<Components::Sprite>(true)->AnimationActive = false;

    Entity* follow = AddEntity("MenuFollow");
    follow->AddComponent(new Components::Transform(*follow, Point(0, 15), 1, 1));
    follow->AddComponent(new Components::Sprite(*follow));
    follow->GetComponent<Components::Sprite>(true)->SetSpriteSource("Title.png");
    follow->GetComponent<Components::Sprite>(true)->AnimationActive = false;
    follow->AddComponent(new Components::Follow(*follow, true, 0.0f, 1.0f, 0UL, Vector(-0.5f,-1.0f)));
//    follow->GetComponent<Components::Follow>()->InterpolationScale = 7.0f;


    Entity* menu = AddEntity("Menu");
    menu->AddComponent(new Components::Transform(*menu));
    menu->AddComponent(new Components::Menu(*menu, Vector(0, -1), false, 2.0f, 2.5f, "Liberation_Serif.fnt", "MenuFollow", Color(1, 1, 1), Color(1, 1, 0)));
    Components::Menu* menuComp = menu->GetComponent<Components::Menu>();
    //menuComp->AddItem(new MenuItems::ResumeGame("ButtonResume.png"));
    //menuComp->AddItem(new MenuItems::LoadLevel("ButtonHowTo.png", "HowToPlay"));
    //menuComp->AddItem(new MenuItems::ActivateAndDeactivate("ButtonMainMenu.png", "MainMenuConfirm", "Menu"));
    menuComp->AddItem(new OptionMenuItems::Resolution());
    menuComp->AddItem(new OptionMenuItems::Fullscreen());
    menuComp->AddItem(new OptionMenuItems::Volume());
    menuComp->AddItem(new OptionMenuItems::FinalAccept("Menu"));
    menuComp->AddItem(new MenuItems::ActivateAndDeactivate("Quit", "QuitConfirm", "Menu"));

    menuComp->Activate();

    Entity* quitConfirm = AddEntity("QuitConfirm");
    quitConfirm->AddComponent(new Components::Transform(*quitConfirm));
    quitConfirm->AddComponent(new Components::Menu(*quitConfirm, Vector(0, -1), false, 2.0f, 2.5f, "Liberation_Serif.fnt", "MenuFollow", Color(1, 1, 1), Color(1, 1, 0)));
    Components::Menu* quitConfirmComp = quitConfirm->GetComponent<Components::Menu>();
    quitConfirmComp->AddItem(new MenuItems::Quit("ButtonQuit.png"));
    quitConfirmComp->AddItem(new MenuItems::ActivateAndDeactivate("ButtonCancel.png", "Menu", "QuitConfirm"));

    Entity* mainMenuConfirm = AddEntity("MainMenuConfirm");
    mainMenuConfirm->AddComponent(new Components::Transform(*mainMenuConfirm));
    mainMenuConfirm->AddComponent(new Components::Menu(*mainMenuConfirm, Vector(0, -1), false, 2.0f, 2.5f, "Liberation_Serif.fnt", "MenuFollow", Color(1, 1, 1), Color(1, 1, 0)));
    Components::Menu* mainMenuConfirmComp = mainMenuConfirm->GetComponent<Components::Menu>();
    mainMenuConfirmComp->AddItem(new MenuItems::LoadLevel("ButtonMainMenu.png", "MainMenu"));
    mainMenuConfirmComp->AddItem(new MenuItems::ActivateAndDeactivate("ButtonCancel.png", "Menu", "MainMenuConfirm"));

//    ForLease->Dispatcher.Attach(NULL, this, "KeyDown", &MenuTest::OnKeyDown);
}

void MenuTest::Update() {

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

void MenuTest::Deinitialize() {
    DeleteAllEntities();
    DeleteAllLevelComponents();
//    ForLease->Dispatcher.Detach(this, "KeyDown");
}

void MenuTest::Unload() {}

//void MenuTest::OnKeyDown(const Event* e) {
//    const KeyboardEvent* key_e = static_cast<const KeyboardEvent*>(e);
//    if (key_e->Key == Keys::Escape)
//        ForLease->GameStateManager().SetAction(Modules::StateAction::Continue);
//}
