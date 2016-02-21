/*!
    \file   Loading.cpp
    \author Sean McGeer
    \date   2/15/16

    \copyright ©Copyright 2016 DigiPen Institute of Technology, All Rights Reserved
*/

#include "Loading.h"
#include "ComponentsInclude.h"
#include "ResourceManager.h"
#include "Vector.h"
#include "Face.h"
#include "Edge.h"
#include "Mesh.h"
#include "GameStateManager.h"
#include "Ray.h"

#include "Utilities.h"

#include <iostream>
#include <string>

namespace FLE = ForLeaseEngine;
using namespace ForLeaseEngine;

Loading::Loading(std::string loadfile) : State("Loading"), LoadFile(loadfile) {}

void Loading::Load() {
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
    logo->AddComponent(new Components::Transform(*logo, Point(0, 15), 30, 30));
    logo->AddComponent(new Components::Sprite(*logo));
    ForLease->Resources.LoadTexture("Title.png");
    logo->GetComponent<Components::Sprite>(true)->SetSpriteSource("Title.png");
    logo->GetComponent<Components::Sprite>(true)->AnimationActive = false;

    //Entity* menu = AddEntity("Menu");
    //menu->AddComponent(new Components::Transform(*menu, Point(0,-15)));
    //menu->AddComponent(new Components::Menu(*menu));
    //Components::Menu* menuComp = menu->GetComponent<Components::Menu>();
    //ForLease->Resources.LoadTexture("ButtonMainMenu.png");
    //menuComp->AddItem(new MenuItems::LoadLevel("ButtonMainMenu.png", "MainMenu"));
    //menuComp->Activate();

    Serializer serializer;
    Serialize(serializer);
    serializer.WriteFile("Loading.json");

    DeleteAllEntities();
    DeleteAllLevelComponents();
}

void Loading::Initialize() {
    Serializer serializer;
    serializer.ReadFile("Loading.json");
    Deserialize(serializer);

    //ForLease->Filesystem.LoadAllAssets();

    LoadPaths = ForLease->Filesystem.GetAllAssetDirectoryListings();

    if (LoadPaths.size() != 0)
        RotationPerFile = 2 * PI / LoadPaths.size();
    NextToLoad = 0;

    //std::vector<Preload::AssetPath> assets = Preload::AllAssets(LoadFile);
}

void Loading::Update() {

    ForLease->OSInput.ProcessAllInput();

    for (FLE::Entity* entity : Entities) {
        entity->Update();
    }

    for (FLE::LevelComponent* levelComponent : LevelComponents) {
        levelComponent->Update(Entities);
    }

    LevelComponents::Renderer* renderer = ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Renderer>();

    if (NextToLoad < LoadPaths.size()) {
        Entity* logo = GetEntityByName("Logo");
        logo->GetComponent<FLE::Components::Transform>()->Rotation += RotationPerFile;
        ForLease->Filesystem.LoadAsset(LoadPaths[NextToLoad]);
        std::cout << "Loaded " << LoadPaths[NextToLoad].second << std::endl;
        ++NextToLoad;
    }
    else {
        ForLease->GameStateManager().SetAction(FLE::Modules::StateAction::Next);
    }

    ForLease->GameWindow->UpdateGameWindow();
}

void Loading::Deinitialize() {
    DeleteAllEntities();
    DeleteAllLevelComponents();
}

void Loading::Unload() {}
