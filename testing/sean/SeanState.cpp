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

    Entity* floor = AddEntity("Floor");
    floor->AddComponent(new Components::Transform(*floor, Point(0,-4), 20, 1, 0, 0));
    floor->AddComponent(new Components::Collision(*floor, 2, 2));
    floor->AddComponent(new Components::Model(*floor, true, false, false, "1-1Block.json"));

    Entity* character = AddEntity("Character");
    character->AddComponent(new Components::Transform(*character, Point(0, 4)));
    character->AddComponent(new Components::Physics(*character));
    character->AddComponent(new Components::Collision(*character, 2.0f, 2.0f, true, 0,0,true));
    character->AddComponent(new Components::Model(*character, true, false, false, "1-1Block.json"));
    Components::CharacterController* charController = Components::CharacterController::Create(*character);
    charController->JumpSpeed = 50;
    charController->MoveSpeed = 50;
    charController->maxSpeed = 200;
    character->AddComponent(charController);

    Entity* moving = AddEntity("Moving");
    moving->AddComponent(new Components::Transform(*moving, Point(), 20));
    moving->AddComponent(new Components::Physics(*moving, 1.0f, Vector(5,0), Vector(0,0), Vector(0,0), false, true));
    moving->AddComponent(new Components::Collision(*moving, 2, 2));
    moving->AddComponent(new Components::Model(*moving, true, false, false, "1-1Block.json"));

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

    for (FLE::Entity* entity : Entities) {
        if (entity->HasComponent(ComponentType::Collision))
            entity->GetComponent<Components::Collision>()->DebugDraw();
    }

    ForLease->GameWindow->UpdateGameWindow();
}

void SeanState::Deinitialize() {
    DeleteAllEntities();
    DeleteAllLevelComponents();
}

void SeanState::Unload() {}
