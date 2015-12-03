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

#include <iostream>
#include <string>

namespace FLE = ForLeaseEngine;
using namespace ForLeaseEngine;

MainMenu::MainMenu() : State("MainMenu") {}

void MainMenu::Load() {
    FLE::LevelComponents::Renderer* renderer = new FLE::LevelComponents::Renderer(*this);
    FLE::Entity* camera = AddEntity("Camera");
    camera->AddComponent(new FLE::Components::Transform(*camera, FLE::Point(0, 0), 1, 1, 0));
    camera->AddComponent(new FLE::Components::Camera(*camera, 0, 1, 50));
    renderer->SetCamera(*camera);
    AddLevelComponent(renderer);
    AddLevelComponent(new LevelComponents::Physics(*this, Vector(0, -10)));
    AddLevelComponent(new LevelComponents::Collision(*this));


    Entity* background = AddEntity("Background");
    background->AddComponent(new Components::Transform(*background));
    background->AddComponent(new Components::Sprite(*background));
    ForLease->Resources.LoadTexture("bg7.png");
    Texture* texture = Texture::CreateTexture("bg7.png");
    //TextureRegion textureRegion(texture, 0, texture->GetWidth(), 0, texture->GetHeight());
    background->GetComponent<Components::Sprite>(true)->SetSpriteSource("bg7.png");
    background->GetComponent<Components::Sprite>(true)->AnimationActive = false;
    background->GetComponent<Components::Transform>(true)->ScaleX = 0.05;
    background->GetComponent<Components::Transform>(true)->ScaleY = 0.05;

    Entity* menu = AddEntity("Menu");
    menu->AddComponent(new Components::Transform(*menu));
    menu->AddComponent(new Components::Menu(*menu));
    Components::Menu* menuComp = menu->GetComponent<Components::Menu>();
    menuComp->AddItem(new MenuItems::NextLevel("ButtonPlay.png"));
    menuComp->AddItem(new MenuItems::ActivateAndDeactivate("ButtonQuit.png", "QuitConfirm", "Menu"));
    menuComp->Activate();

    Entity* quitConfirm = AddEntity("QuitConfirm");
    quitConfirm->AddComponent(new Components::Transform(*quitConfirm));
    quitConfirm->AddComponent(new Components::Menu(*quitConfirm));
    Components::Menu* quitConfirmComp = quitConfirm->GetComponent<Components::Menu>();
    quitConfirmComp->AddItem(new MenuItems::Quit("ButtonQuit.png"));
    quitConfirmComp->AddItem(new MenuItems::ActivateAndDeactivate("ButtonCancel.png", "Menu", "QuitConfirm"));

    Serializer serializer;
    Serialize(serializer);
    serializer.WriteFile("MainMenu.json");

    DeleteAllEntities();
    DeleteAllLevelComponents();
}

void MainMenu::Initialize() {
    Serializer serializer;
    serializer.ReadFile("MainMenu.json");
    Deserialize(serializer);
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
