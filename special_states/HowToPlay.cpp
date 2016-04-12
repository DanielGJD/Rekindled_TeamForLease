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
    AddLevelComponent(new LevelComponents::Light(*this, Color(0.5f, 0.5f, 0.5f)));
    AddLevelComponent(new LevelComponents::Physics(*this, Vector(0, -100)));
    AddLevelComponent(new LevelComponents::Collision(*this));


    Entity* background = AddEntity("Background");
    background->AddComponent(new Components::Transform(*background, Point(), 45, 25, 0, -10));
    background->AddComponent(new Components::Sprite(*background));
    //ForLease->Resources.LoadTexture("bg7.png");
    //Texture* texture = Texture::CreateTexture("bg7.png");
    //TextureRegion textureRegion(texture, 0, texture->GetWidth(), 0, texture->GetHeight());
    background->GetComponent<Components::Sprite>(true)->SetSpriteSource("bg7_test2.png");
    background->GetComponent<Components::Sprite>(true)->AnimationActive = false;

    Entity* backgroundMask = AddEntity("BackgroundMask");
    backgroundMask->AddComponent(new Components::Transform(*backgroundMask, Point(), 50, 50, 0, -9));
    backgroundMask->AddComponent(new Components::Model(*backgroundMask, true, false, false, "1-1Block.json", "", Color(0.75f, 0.75f, 0.75f), FLE::MULTIPLY));

//    Entity* logo = AddEntity("Logo");
//    logo->AddComponent(new Components::Transform(*logo, Point(0, 5), 30, 30));
//    logo->AddComponent(new Components::Sprite(*logo));
//    logo->GetComponent<Components::Sprite>(true)->SetSpriteSource("ControlPage.png");
//    logo->GetComponent<Components::Sprite>(true)->AnimationActive = false;

    Entity* player = SpawnArchetype(ForLease->Filesystem.AssetDirectory(Modules::Filesystem::AssetType::Blueprint) + "Player");

    Entity* floor = AddEntity("Floor");
    floor->AddComponent(new Components::Transform(*floor, Point(0.0f, -10.0f), 9999999.0f, 1.0f, 0.0f, -7));
    floor->AddComponent(new Components::Collision(*floor, 2.0f, 2.0f));
    floor->AddComponent(new Components::Model(*floor, false, false, false, "1-1Block.json"));

    Entity* left = AddEntity("Left");
    left->AddComponent(new Components::Transform(*left, Point(-40.0f, 0.0f), 1.0f, 9999999.0f));
    left->AddComponent(new Components::Collision(*left, 2.0f, 2.0f));

    Entity* right = AddEntity("Right");
    right->AddComponent(new Components::Transform(*right, Point(40.0f, 0.0f), 1.0f, 9999999.0f));
    right->AddComponent(new Components::Collision(*right, 2.0f, 2.0f));

    Entity* follow = SpawnArchetype(ForLease->Filesystem.AssetDirectory(Modules::Filesystem::AssetType::Blueprint) + "Wisp", Point(-35.0f, -13.0f), "MenuFollow");
    follow->GetComponent<Components::Transform>()->ZOrder = 10000;
    follow->GetComponent<Components::Follow>()->Offset = Vector(-1.5f, -1.0f);
    follow->GetComponent<Components::Follow>()->Speed = 4;
    follow->GetComponent<Components::Light>()->Radius = 5.0f;

    Entity* menu = AddEntity("Menu");
    menu->AddComponent(new Components::Transform(*menu, Point(-35.0f, -13.0f)));
    menu->AddComponent(new Components::Menu(*menu, Vector(0, -1), true, 2.0f, 2.5f, "Liberation_Serif.fnt", "MenuFollow", Color(1, 1, 1), Color(1, 1, 0)));
    Components::Menu* menuComp = menu->GetComponent<Components::Menu>();
    menuComp->AddItem(new MenuItems::LoadLevel("Main Menu", "MainMenu"));
    menuComp->Activate();

    Entity* htpPic = AddEntity("HowToPlay");
    htpPic->AddComponent(new Components::Transform(*htpPic, Point(0, 0), 30.0f, 30.0f, 0.0f, -8));
    htpPic->AddComponent(new Components::Sprite(*htpPic, "HowToPlay.png"));

    //Entity* header = AddEntity("Header");
    //header->AddComponent(new Components::Transform(*header, Point(-40.0f, 20.0f), 3.0f, 3.0f));
    //header->AddComponent(new Components::SpriteText(*header, "Liberation_Serif.fnt"));
    //header->GetComponent<Components::SpriteText>()->Text = "CONTROLS";

    //Entity* underline = AddEntity("Underline");
    //underline->AddComponent(new Components::Transform(*underline, Point(-32.5f, 17.0f), 7.5f, 0.1f));
    //underline->AddComponent(new Components::Model(*underline, true, false, false, "1-1Block.json"));

    //Entity* text = AddEntity("Text");
    //text->AddComponent(new Components::Transform(*text, Point(-40.0f, 16.5f), 2.0f, 2.0f));
    //text->AddComponent(new Components::SpriteText(*text, "Liberation_Serif.fnt"));
    ////text->GetComponent<Components::SpriteText>()->
    //std::stringstream ss;
    //ss  << "A/D         Left/Right\n"
    //    << "Space       Jump";
    //text->GetComponent<Components::SpriteText>()->Text = ss.str();
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
