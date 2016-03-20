
/*!
    \file   SeanState.cpp
    \author Sean McGeer

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "SeanState.h"
#include "ComponentsInclude.h"
#include "ResourceManager.h"
#include "Vector.h"
#include "Face.h"
#include "Edge.h"
#include "Mesh.h"
#include "GameStateManager.h"
#include "Ray.h"
#include "ComponentsInclude.h"

#include <iostream>
#include <string>

namespace FLE = ForLeaseEngine;
using namespace ForLeaseEngine;

SeanState::SeanState() : State("Sean's State") {}

void SeanState::Load() {
    FLE::LevelComponents::Renderer* renderer = new FLE::LevelComponents::Renderer(*this);
    FLE::Entity* camera = AddEntity("Camera");
    camera->AddComponent(new FLE::Components::Transform(*camera, FLE::Point(0,0), 1, 1, 0));
    camera->AddComponent(new FLE::Components::Camera(*camera, 0, 1, 50));
    renderer->SetCamera(*camera);
    AddLevelComponent(renderer);
    AddLevelComponent(new LevelComponents::Physics(*this, Vector(0,-10)));
    AddLevelComponent(new LevelComponents::Collision(*this));
    AddLevelComponent(new LevelComponents::Menu(*this));

    //Entity* checkpoint = AddEntity("Checkpoint");
    //checkpoint->AddComponent(new Components::Transform(*checkpoint));
    //Components::Collision* checkpointCollide = new Components::Collision(*checkpoint);
    //checkpointCollide->ResolveCollisions = false;
    //checkpoint->AddComponent(checkpointCollide);
    //checkpoint->AddComponent(Components::Checkpoint::Create(*checkpoint));
    //checkpoint->AddComponent(new Components::Model(*checkpoint, true, false, false, "1-1Block.json"));

    //Entity* block = AddEntity("Block");
    //block->AddComponent(new Components::Transform(*block, Point(4,3), 1, 5));
    //block->AddComponent(new Components::Physics(*block));
    //block->AddComponent(new Components::Collision(*block, 2, 2));

    Entity* floor = AddEntity("Floor");
    floor->AddComponent(new Components::Transform(*floor, Point(0,-4), 20, 1, 0, 0));
    //floor->AddComponent(new Components::Physics(*floor, 1.0f, Vector(0, 0), Vector(0, 0), Vector(0, 0), false, true));
    //floor->AddComponent(new Components::EnemyPace(*floor, 3, 20, 0));
    floor->AddComponent(new Components::Collision(*floor, 2, 2, true, 0, 0, true));
    floor->AddComponent(new Components::Model(*floor, true, false, false, "1-1Block.json"));

    Entity* ceil = AddEntity("Ceiling");
    ceil->AddComponent(new Components::Transform(*ceil, Point(0, 12), 20, 1, 0, 0));
    //floor->AddComponent(new Components::Physics(*floor, 1.0f, Vector(0, 0), Vector(0, 0), Vector(0, 0), false, true));
    //floor->AddComponent(new Components::EnemyPace(*floor, 3, 20, 0));
    ceil->AddComponent(new Components::Collision(*ceil, 2, 2, true, 0, 0, true));
    ceil->AddComponent(new Components::Model(*ceil, true, false, false, "1-1Block.json"));

    //Entity* floor2 = AddEntity("Floor2");
    //floor2->AddComponent(new Components::Transform(*floor2, Point(0, -8), 20, 1, 0, 0));
    //floor2->AddComponent(new Components::Collision(*floor2, 2, 2));
    //floor2->AddComponent(new Components::EnemyPace(*floor2, 3, 20, 2, 1));
    //floor2->AddComponent(new Components::Model(*floor2, true, false, false, "1-1Block.json"));

    //Entity* floor3 = AddEntity("Floor3");
    //floor3->AddComponent(new Components::Transform(*floor3, Point(0, -12), 20, 1, 0, 0));
    //floor3->AddComponent(new Components::Collision(*floor3, 2, 2));
    //floor3->AddComponent(new Components::Model(*floor3, true, false, false, "1-1Block.json"));

    //Entity* floor4 = AddEntity("Floor4");
    //floor4->AddComponent(new Components::Transform(*floor4, Point(0, -16), 20, 1, 0, 0));
    //floor4->AddComponent(new Components::Collision(*floor4, 2, 2));
    //floor4->AddComponent(new Components::Model(*floor4, true, false, false, "1-1Block.json"));

    Entity* character = AddEntity("Character");
    character->AddComponent(new Components::Transform(*character, Point(0, 4), 5, 5));
    character->AddComponent(new Components::Physics(*character));
    character->AddComponent(new Components::Collision(*character, 2.0f, 2.0f, true/*, 20, 20*/));
    character->AddComponent(new Components::Model(*character, true, false, false, "1-1Block.json"));
    Components::CharacterController* charController = Components::CharacterController::Create(*character);
    charController->Acceleration = 60;
    charController->JumpSpeed = 20;
//    charController->JumpSpeed = 50;
//    charController->MoveSpeed = 50;
//    charController->maxSpeed = 200;
    character->AddComponent(charController);

    //Entity* moving = AddEntity("Moving");
    //moving->AddComponent(new Components::Transform(*moving, Point(), 20));
    //moving->AddComponent(new Components::Physics(*moving, 1.0f, Vector(5,0), Vector(0,0), Vector(0,0), false, true));
    //moving->AddComponent(new Components::Collision(*moving, 2, 2));
    //moving->AddComponent(new Components::Model(*moving, true, false, false, "1-1Block.json"));

    //Entity* wall = AddEntity("Wall");
    //wall->AddComponent(new Components::Transform(*wall, Point(7, -4), 2, 3));
    //wall->AddComponent(new Components::Collision(*wall, 2.0f, 2.0f, true, 0, 0));
    //wall->AddComponent(new Components::Model(*wall, true, false, false, "1-1Block.json"));

    //Entity* platform = AddEntity("Platform");
    //platform->AddComponent(new Components::Transform(*platform, Point(12, 5), 1, 1));
    //platform->AddComponent(new Components::Collision(*platform, 2.0f, 2.0f, true/*, -2, -2*/));
    //platform->AddComponent(new Components::Model(*platform, true, false, false, "1-1Block.json"));

    Serializer serial;
    LevelComponents::Checkpoint* lcCheckpoint = new LevelComponents::Checkpoint(*this, serial);
    lcCheckpoint->TriggerEntityID = character->GetID();
    AddLevelComponent(lcCheckpoint);

    Serializer serial2;
    Serialize(serial2);
    lcCheckpoint->LastCheckpointState = serial2;

    serial2.WriteFile("StateTest.json");
}

void SeanState::Initialize() {
    Serializer serial;
    serial.ReadFile("StateTest.json");
    Deserialize(serial);
}

void SeanState::Update() {

    ForLease->OSInput.ProcessAllInput();

    for (FLE::Entity* entity : Entities) {
        entity->Update();
    }

    for (FLE::LevelComponent* levelComponent : LevelComponents) {
        levelComponent->Update(Entities);
    }

    FLE::Point testPos = FLE::Point(-5, 0);
    FLE::Ray   testRay = FLE::Ray(testPos, Vector(1, -1), 20, FLE::Ray::Unlimited);

    Entity* player = GetEntityByName("Character");
    std::cout << player->GetComponent<Components::Transform>()->Position << std::endl;

    std::vector<FLE::Ray::Collision> collisions = FLE::Ray::CheckCollisionsMultipleEntities(testRay, Entities);

    FLE::LevelComponents::Renderer* renderer = GetLevelComponent<FLE::LevelComponents::Renderer>();

    renderer->SetDrawingColor(Color(1, 0, 0));

    //renderer->DrawRectangleFilled(testPos, 1, 1, 0);
    renderer->DrawArrow(testPos, testRay.GetScaledVector());

    //std::cout << "============" << std::endl;

    for (FLE::Ray::Collision collision : collisions) {
        renderer->DrawRectangleFilled(collision.Point, 1, 1, 0);
    }

    //std::cout << "============" << std::endl;


    UpdateDebug();

    ForLease->GameWindow->UpdateGameWindow();
}

void SeanState::Deinitialize() {
    DeleteAllEntities();
    DeleteAllLevelComponents();
}

void SeanState::Unload() {}
