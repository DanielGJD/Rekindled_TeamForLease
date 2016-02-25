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
    AddLevelComponent(new LevelComponents::Checkpoint(*this));


    Entity* checkpoint = AddEntity("Checkpoint");
    checkpoint->AddComponent(new Components::Transform(checkpoint));
    checkpoint->AddComponent(new Components::Collision(checkpoint));
    checkpoint->AddComponent(new Components::Checkpoint(checkpoint));

    Serializer serial2;
    Serialize(serial2);
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

    ForLease->GameWindow->UpdateGameWindow();
}

void SeanState::Deinitialize() {
    DeleteAllEntities();
    DeleteAllLevelComponents();
}

void SeanState::Unload() {}
