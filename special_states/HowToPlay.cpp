/*!
    \file   HowToPlay.cpp
    \author Sean McGeer
    \date   12/3/15

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "HowToPlay.h"
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

HowToPlay::HowToPlay() : State("HowToPlay") {}

void HowToPlay::Load() {
}

void HowToPlay::Initialize() {
    FLE::LevelComponents::Renderer* renderer = new FLE::LevelComponents::Renderer(*this);
    FLE::Entity* camera = AddEntity("Camera");
    camera->AddComponent(new FLE::Components::Transform(*camera, FLE::Point(0, 0), 1, 1, 0));
    camera->AddComponent(new FLE::Components::Camera(*camera, 0, 1, 50));
    renderer->SetCamera(*camera);
    AddLevelComponent(renderer);
    AddLevelComponent(new LevelComponents::Physics(*this, Vector(0, -10)));
    AddLevelComponent(new LevelComponents::Collision(*this));


    Entity* background = AddEntity("Background");
    background->AddComponent(new Components::Transform(*background, 0, 0, 50, 50));
    background->AddComponent(new Components::Sprite(*background));
    ForLease->Resources.LoadTexture("bg7.png");
    //Texture* texture = Texture::CreateTexture("bg7.png");
    //TextureRegion textureRegion(texture, 0, texture->GetWidth(), 0, texture->GetHeight());
    background->GetComponent<Components::Sprite>(true)->SetSpriteSource("bg7.png");
    background->GetComponent<Components::Sprite>(true)->AnimationActive = false;

    Entity* logo = AddEntity("Logo");
    logo->AddComponent(new Components::Transform(*logo, Point(0, 5), 30, 30));
    logo->AddComponent(new Components::Sprite(*logo));
    logo->GetComponent<Components::Sprite>(true)->SetSpriteSource("ControlPage.png");
    logo->GetComponent<Components::Sprite>(true)->AnimationActive = false;


    Entity* menu = AddEntity("Menu");
    menu->AddComponent(new Components::Transform(*menu, Point(0, -15)));
    menu->AddComponent(new Components::Menu(*menu));
    Components::Menu* menuComp = menu->GetComponent<Components::Menu>();
    menuComp->AddItem(new MenuItems::LoadLevel("ButtonMainMenu.png", "MainMenu"));
    menuComp->Activate();
}

void HowToPlay::Update() {

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

void HowToPlay::Deinitialize() {
    DeleteAllEntities();
    DeleteAllLevelComponents();
}

void HowToPlay::Unload() {}
