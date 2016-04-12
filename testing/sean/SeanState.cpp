
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
    Serializer serial;
    serial.ReadFile(ForLease->Filesystem.AssetDirectory(FLE::Modules::Filesystem::AssetType::Level) + "Collision_Test");
    Deserialize(serial);
    //FLE::LevelComponents::Renderer* renderer = new FLE::LevelComponents::Renderer(*this);
    //FLE::Entity* camera = AddEntity("Camera");
    //camera->AddComponent(new FLE::Components::Transform(*camera, FLE::Point(0,0), 1, 1, 0));
    //camera->AddComponent(new FLE::Components::Camera(*camera, 0, 1, 50));
    //renderer->SetCamera(*camera);
    //AddLevelComponent(renderer);
    //AddLevelComponent(new LevelComponents::Physics(*this, Vector(0,-100)));
    //AddLevelComponent(new LevelComponents::Collision(*this));
    //AddLevelComponent(new LevelComponents::Menu(*this));
    AddLevelComponent(new LevelComponents::UsefulObject(*this, "balloon", "distraction"));
    GetLevelComponent<LevelComponents::UsefulObject>()->BalloonMesh = "Balloon.json";
    GetLevelComponent<LevelComponents::UsefulObject>()->DistractionMesh = "Color.json";

    ////Entity* checkpoint = AddEntity("Checkpoint");
    ////checkpoint->AddComponent(new Components::Transform(*checkpoint));
    ////Components::Collision* checkpointCollide = new Components::Collision(*checkpoint);
    ////checkpointCollide->ResolveCollisions = false;
    ////checkpoint->AddComponent(checkpointCollide);
    ////checkpoint->AddComponent(Components::Checkpoint::Create(*checkpoint));
    ////checkpoint->AddComponent(new Components::Model(*checkpoint, true, false, false, "1-1Block.json"));

    ////Entity* block = AddEntity("Block");
    ////block->AddComponent(new Components::Transform(*block, Point(4,3), 1, 5));
    ////block->AddComponent(new Components::Physics(*block));
    ////block->AddComponent(new Components::Collision(*block, 2, 2));

    ////Entity* floor = AddEntity("Floor");
    ////floor->AddComponent(new Components::Transform(*floor, Point(0,-4), 20, 1, 0, 0));
    //////floor->AddComponent(new Components::Physics(*floor, 1.0f, Vector(0, 0), Vector(0, 0), Vector(0, 0), false, true));
    ////floor->AddComponent(new Components::EnemyPace(*floor, 3, 20, 0, 1));
    ////floor->AddComponent(new Components::Collision(*floor, 2, 2, true, 0, 0, true));
    ////floor->AddComponent(new Components::Model(*floor, true, false, false, "1-1Block.json"));
    //Entity* bg = AddEntity("Backround");
    //bg->AddComponent(new Components::Transform(*bg, Point(0,0), 300, 300));
    //bg->AddComponent(new Components::Model(*bg, true, false, false, "1-1Block.json", "", Color(0,0,0)));

    //Entity* floor2 = AddEntity("Floor2");
    //floor2->AddComponent(new Components::Transform(*floor2, Point(0, -20), 300, 1, 0, 0));
    ////floor2->AddComponent(new Components::Physics(*floor2, 1.0f, Vector(0, 0), Vector(0, 0), Vector(0, 0), false, true));
    ////floor2->AddComponent(new Components::EnemyPace(*floor2, 3, 20, 0));
    //floor2->AddComponent(new Components::Collision(*floor2, 2, 2, true, 0, 0, true));
    //floor2->AddComponent(new Components::Model(*floor2, true, false, false, "1-1Block.json"));

    //Entity* character = AddEntity("Character");
    //character->AddComponent(new Components::Transform(*character, Point(0, 0), 5, 5));
    //character->AddComponent(new Components::Physics(*character));
    //character->AddComponent(new Components::Collision(*character, 2.0f, 2.0f, true/*, 20, 20*/));
    //character->AddComponent(new Components::Model(*character, true, false, false, "1-1Block.json"));
    //character->AddComponent(new Components::UsefulObjectInventory(*character));
    //Components::CharacterController* charController = Components::CharacterController::Create(*character);
    //charController->Acceleration = 60;
    //charController->JumpSpeed = 40;
    ////    charController->JumpSpeed = 50;
    ////    charController->MoveSpeed = 50;
    ////    charController->maxSpeed = 200;
    //character->AddComponent(charController);
    //character->GetComponent<Components::UsefulObjectInventory>()->ThrowVector = Vector(20, 20);
    //character->GetComponent<Components::UsefulObjectInventory>()->FollowName = "Follow";
    //character->GetComponent<Components::UsefulObjectInventory>()->BalloonJumpSpeed = 100.0f;
    //character->GetComponent<Components::UsefulObjectInventory>()->NormalJumpSpeed = charController->JumpSpeed;

    Entity* follow = AddEntity("Follow");
    follow->AddComponent(new Components::Transform(*follow, Point(0, 10), 5, 5));
    //follow->AddComponent(new Components::Physics(*follow));
    //follow->AddComponent(new Components::Collision(*follow, 2.0f, 2.0f, false/*, 20, 20*/));
    follow->AddComponent(new Components::Model(*follow, true, false, false));
    follow->AddComponent(new Components::Follow(*follow, true, 0.0f, 1.0f, 5716, Vector(0,2)));

    GetEntityByID(5716)->GetComponent<Components::UsefulObjectInventory>()->BalloonJumpSpeed = 100.0f;
    GetEntityByID(5716)->GetComponent<Components::UsefulObjectInventory>()->NormalJumpSpeed = 50.0f;


    Entity* object = AddEntity("Object");
    object->AddComponent(new Components::Transform(*object, Point(0, 30), 1, 1));
    object->AddComponent(new Components::Physics(*object));
    object->AddComponent(new Components::Collision(*object, 2.0f, 2.0f, false/*, 20, 20*/));
    object->AddComponent(new Components::Model(*object, true, false, false, "1-1Block.json", "", Color(1,0,0)));
    object->AddComponent(new Components::UsefulObject(*object, UsefulObjectCategory::Balloon));
    object->CreateArchetype(ForLease->Filesystem.AssetDirectory(Modules::Filesystem::AssetType::Blueprint) + "balloon");

    //Entity* object2 = AddEntity("Object2");
    //object2->AddComponent(new Components::Transform(*object2, Point(-10, 4), 1, 1));
    //object2->AddComponent(new Components::Physics(*object2));
    //object2->AddComponent(new Components::Collision(*object2, 2.0f, 2.0f, false/*, 20, 20*/));
    //object2->AddComponent(new Components::Model(*object2, true, false, false, "1-1Block.json", "", Color(0, 1, 0)));
    //object2->AddComponent(new Components::UsefulObject(*object2, UsefulObjectCategory::Distraction));
    //object2->CreateArchetype(ForLease->Filesystem.AssetDirectory(Modules::Filesystem::AssetType::Blueprint) + "distraction");

    //Entity* smallPlatform = AddEntity("SmallPlatform");
    //smallPlatform->AddComponent(new Components::Transform(*smallPlatform, Point(20,-15), 5, 1, 0, 0));
    //smallPlatform->AddComponent(new Components::Physics(*smallPlatform, 1.0f, Vector(0, 0), Vector(0, 0), Vector(0, 0), false, true));
    //smallPlatform->AddComponent(new Components::Collision(*smallPlatform, 2, 2, true, 0, 0, true));
    //smallPlatform->AddComponent(new Components::MovingPlatform(*smallPlatform, 15.0f, 15.0f, 0.25f, 5, 1.0f, Components::MovingPlatform::Axis::Horizontal));
    //smallPlatform->AddComponent(new Components::Model(*smallPlatform, true, false, false, "1-1Block.json"));

    //Entity* smallPlatform2 = AddEntity("SmallPlatform2");
    //smallPlatform2->AddComponent(new Components::Transform(*smallPlatform2, Point(-20, -4), 5, 1, 0, 0));
    //smallPlatform2->AddComponent(new Components::Physics(*smallPlatform2, 1.0f, Vector(0, 0), Vector(0, 0), Vector(0, 0), false, true));
    //smallPlatform2->AddComponent(new Components::Collision(*smallPlatform2, 2, 2, true, 0, 0, true));
    //smallPlatform2->AddComponent(new Components::MovingPlatform(*smallPlatform2, 15.0f, 15.0f, 0.25f, 5.0f, 1.0f, Components::MovingPlatform::Axis::Vertical));
    //smallPlatform2->AddComponent(new Components::Model(*smallPlatform2, true, false, false, "1-1Block.json"));

    Entity* ceil = AddEntity("Ceiling");
    ceil->AddComponent(new Components::Transform(*ceil, Point(0, 15), 20, 1, 0, 0));
    //floor->AddComponent(new Components::Physics(*floor, 1.0f, Vector(0, 0), Vector(0, 0), Vector(0, 0), false, true));
    //floor->AddComponent(new Components::EnemyPace(*floor, 3, 20, 0));
    ceil->AddComponent(new Components::Collision(*ceil, 2, 2, true, 0, 0, true));
    ceil->AddComponent(new Components::Model(*ceil, true, false, false, "1-1Block.json"));

    Entity* lWall = AddEntity("LeftWall");
    lWall->AddComponent(new Components::Transform(*lWall, Point(-10, 0), 1, 20, 0, 0));
    //floor->AddComponent(new Components::Physics(*floor, 1.0f, Vector(0, 0), Vector(0, 0), Vector(0, 0), false, true));
    //floor->AddComponent(new Components::EnemyPace(*floor, 3, 20, 0));
    lWall->AddComponent(new Components::Collision(*lWall, 2, 2, true, 0, 0, true));
    lWall->AddComponent(new Components::Model(*lWall, true, false, false, "1-1Block.json"));

    Entity* rWall = AddEntity("RightWall");
    rWall->AddComponent(new Components::Transform(*rWall, Point(10, 0), 1, 20, 0, 0));
    //floor->AddComponent(new Components::Physics(*floor, 1.0f, Vector(0, 0), Vector(0, 0), Vector(0, 0), false, true));
    //floor->AddComponent(new Components::EnemyPace(*floor, 3, 20, 0));
    rWall->AddComponent(new Components::Collision(*rWall, 2, 2, false, 0, 0, true));
    rWall->AddComponent(new Components::Model(*rWall, true, false, false, "1-1Block.json"));

    ////Entity* floor2 = AddEntity("Floor2");
    ////floor2->AddComponent(new Components::Transform(*floor2, Point(0, -8), 20, 1, 0, 0));
    ////floor2->AddComponent(new Components::Collision(*floor2, 2, 2));
    ////floor2->AddComponent(new Components::EnemyPace(*floor2, 3, 20, 2, 1));
    ////floor2->AddComponent(new Components::Model(*floor2, true, false, false, "1-1Block.json"));

    ////Entity* floor3 = AddEntity("Floor3");
    ////floor3->AddComponent(new Components::Transform(*floor3, Point(0, -12), 20, 1, 0, 0));
    ////floor3->AddComponent(new Components::Collision(*floor3, 2, 2));
    ////floor3->AddComponent(new Components::Model(*floor3, true, false, false, "1-1Block.json"));

    ////Entity* floor4 = AddEntity("Floor4");
    ////floor4->AddComponent(new Components::Transform(*floor4, Point(0, -16), 20, 1, 0, 0));
    ////floor4->AddComponent(new Components::Collision(*floor4, 2, 2));
    ////floor4->AddComponent(new Components::Model(*floor4, true, false, false, "1-1Block.json"));

    ////Entity* moving = AddEntity("Moving");
    ////moving->AddComponent(new Components::Transform(*moving, Point(), 20));
    ////moving->AddComponent(new Components::Physics(*moving, 1.0f, Vector(5,0), Vector(0,0), Vector(0,0), false, true));
    ////moving->AddComponent(new Components::Collision(*moving, 2, 2));
    ////moving->AddComponent(new Components::Model(*moving, true, false, false, "1-1Block.json"));

    ////Entity* wall = AddEntity("Wall");
    ////wall->AddComponent(new Components::Transform(*wall, Point(7, -4), 2, 3));
    ////wall->AddComponent(new Components::Collision(*wall, 2.0f, 2.0f, true, 0, 0));
    ////wall->AddComponent(new Components::Model(*wall, true, false, false, "1-1Block.json"));

    ////Entity* platform = AddEntity("Platform");
    ////platform->AddComponent(new Components::Transform(*platform, Point(12, 5), 1, 1));
    ////platform->AddComponent(new Components::Collision(*platform, 2.0f, 2.0f, true/*, -2, -2*/));
    ////platform->AddComponent(new Components::Model(*platform, true, false, false, "1-1Block.json"));

    //Serializer serial;
    //LevelComponents::Checkpoint* lcCheckpoint = new LevelComponents::Checkpoint(*this, serial);
    //lcCheckpoint->TriggerEntityID = character->GetID();
    //AddLevelComponent(lcCheckpoint);

    //Serializer serial2;
    //Serialize(serial2);
    //lcCheckpoint->LastCheckpointState = serial2;

    //serial2.WriteFile("StateTest.json");
}

void SeanState::Initialize() {
}

void SeanState::Update() {
    //std::cout << ForLease->FrameRateController().FrameNumber << " " << ForLease->FrameRateController().GetDt() << std::endl;

    ForLease->OSInput.ProcessAllInput();

    //Components::Transform* smallPlatform = GetEntityByName("SmallPlatform")->GetComponent<Components::Transform>();
    //Components::MovingPlatform* spmp = GetEntityByName("SmallPlatform")->GetComponent<Components::MovingPlatform>();
    //smallPlatform->Position += spmp->LastMovement();

    //Components::Transform* floor = GetEntityByName("Floor2")->GetComponent<Components::Transform>();
    //floor->Position += spmp->LastMovement();
    ////std::cout << floor->Position.y << std::endl;

    for (FLE::Entity* entity : Entities) {
        entity->Update();
    }

    for (FLE::LevelComponent* levelComponent : LevelComponents) {
        levelComponent->Update(Entities);
    }

    FLE::Point testPos = FLE::Point(-5, 0);
    FLE::Ray   testRay = FLE::Ray(testPos, Vector(1, -1), 20, FLE::Ray::Unlimited);

    Entity* player = GetEntityByName("Character");
    //std::cout << player->GetComponent<Components::Transform>()->Position << std::endl;
    //std::cout << ForLease->FrameRateController().GetDt() << std::endl;

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

    PostFrameCleanup();
}

void SeanState::Deinitialize() {
    DeleteAllEntities();
    DeleteAllLevelComponents();
}

void SeanState::Unload() {}
