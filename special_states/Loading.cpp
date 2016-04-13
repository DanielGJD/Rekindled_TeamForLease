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
}

void Loading::Initialize() {
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

//    Entity* logo = AddEntity("Logo");
//    logo->AddComponent(new Components::Transform(*logo, Point(0, 15), 30, 30));
//    logo->AddComponent(new Components::Sprite(*logo));
//    ForLease->Resources.LoadTexture("Title.png");
//    logo->GetComponent<Components::Sprite>(true)->SetSpriteSource("Title.png");
//    logo->GetComponent<Components::Sprite>(true)->AnimationActive = false;

    Entity* player = AddEntity("Player");
    player->AddComponent(new Components::Transform(*player, Point(0,0), 5, 5));
    Components::Model* model = new Components::Model(*player, true, false, false, "fox.json");
    model->SetAnimation("a");
    model->AnimationActive = true;
    model->Looping = true;
    model->FrameRate = 4;
    player->AddComponent(model);

    Entity* follow = SpawnArchetype(ForLease->Filesystem.AssetDirectory(Modules::Filesystem::AssetType::Blueprint) + "Wisp", Point(-25.0f, 0.0f), "MenuFollow");
    follow->GetComponent<Components::Follow>()->Offset = Vector(-2.0f, 2.5f);
    follow->GetComponent<Components::Light>()->Radius = 5.0f;
    follow->GetComponent<Components::Follow>()->FollowEntityID = player->GetID();
    follow->GetComponent<Components::Transform>()->Position = player->GetComponent<Components::Transform>()->Position + follow->GetComponent<Components::Follow>()->Offset;

//    Entity* status = AddEntity("Status");
//    status->AddComponent(new Components::Transform(*status, Point(-40,-10), 2, 2));
//    status->AddComponent(new Components::SpriteText(*status, "Liberation_Serif.fnt"));

    Entity* backBar = AddEntity("BackBar");
    backBar->AddComponent(new Components::Transform(*backBar, Point(0, -5), EndScale, 1));
    backBar->AddComponent(new Components::Model(*backBar, true, false, false, "1-1Block.json", "", Color(0,0,0)));

    Entity* bar = AddEntity("Bar");
    bar->AddComponent(new Components::Transform(*bar, Point(0, -5), 0, 1));
    bar->AddComponent(new Components::Model(*bar, true, false, false, "1-1Block.json"));


    //ForLease->Filesystem.LoadAllAssets();

    LoadPaths = ForLease->Filesystem.GetAllAssetDirectoryListings();

    if (LoadPaths.size() != 0) {
        ScalePerFile = EndScale / LoadPaths.size();
        RotationPerFile = 2 * PI / LoadPaths.size();
    }
    NextToLoad = 0;
}

void Loading::Update() {
    ForLease->sound->Update();
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
        //logo->GetComponent<FLE::Components::Transform>()->Rotation += RotationPerFile;

        Entity* player = GetEntityByName("Player");
//        player->GetComponent<FLE::Components::Transform>()->Position += MovementPerFile;
        Entity* bar = GetEntityByName("Bar");
        bar->GetComponent<Components::Transform>()->ScaleX += ScalePerFile;

        ForLease->Filesystem.LoadAsset(LoadPaths[NextToLoad]);
        std::cout << "Loaded " << LoadPaths[NextToLoad].second << std::endl;
        ++NextToLoad;
//        if (NextToLoad < LoadPaths.size()) {
//            Entity* status = GetEntityByName("Status");
//            status->GetComponent<FLE::Components::SpriteText>()->Text = "Loading " + LoadPaths[NextToLoad].second + "...";
//        }
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
