#include "SeanState.h"
#include "ComponentsInclude.h"
#include "ResourceManager.h"
#include "Vector.h"
#include "Face.h"
#include "Edge.h"
#include "Mesh.h"
#include "GameStateManager.h"

#include <iostream>

namespace FLE = ForLeaseEngine;
using namespace ForLeaseEngine;

SeanState::SeanState() : State("Sean's State") {}

void SeanState::Load() {
    FLE::LevelComponents::Renderer* renderer = new FLE::LevelComponents::Renderer(*this);
    FLE::Entity* camera = AddEntity("Camera");
    camera->AddComponent(new FLE::Components::Transform(*camera, FLE::Point(0,0), 1, 1, 0));
    camera->AddComponent(new FLE::Components::Camera(*camera, 0, 1, 720));
    renderer->SetCamera(*camera);
    AddLevelComponent(renderer);
    AddLevelComponent(new LevelComponents::Physics(*this, Vector(0,-10)));
//    AddLevelComponent(new LevelComponents::Collision(*this));

    FLE::Mesh* box = new FLE::Mesh(4, 4, 2);
    box->SetVertex(Point(-1,-1), 0);
    box->SetVertex(Point(-1,1), 1);
    box->SetVertex(Point(1,1), 2);
    box->SetVertex(Point(1,-1), 3);

    box->SetEdge(IndexedEdge(0,1), 0);
    box->SetEdge(IndexedEdge(1,2), 1);
    box->SetEdge(IndexedEdge(2,3), 2);
    box->SetEdge(IndexedEdge(3,0), 3);

    box->SetFace(IndexedFace(0,1,2), 0);
    box->SetFace(IndexedFace(0,2,3), 1);

    box->SetFaceColor(Color(1,0,0), 0);
    box->SetFaceColor(Color(1,0,0), 1);

    Serializer serial;
    box->Serialize(serial);
    serial.WriteFile("BoxMesh.json");


    FLE::Entity* entityBox = AddEntity("Box");
    entityBox->AddComponent(new Components::Transform(*entityBox, 0, 100, 100, 100, 0));
    entityBox->AddComponent(new Components::Model(*entityBox, true, "BoxMesh.json", "", Color(1,1,1,1)));
    entityBox->AddComponent(new Components::Collision(*entityBox, 100, 100));
    entityBox->AddComponent(new Components::Physics(*entityBox));

    Entity* entityFloor = AddEntity("Floor");
    entityFloor->AddComponent(new Components::Transform(*entityFloor, 0, -300, 500, 20, 0));
    entityFloor->AddComponent(new Components::Model(*entityFloor, true, "BoxMesh.json", "", Color(0,1,1,1)));
    entityFloor->AddComponent(new Components::Collision(*entityFloor, 500, 100));

    serial;
    Serialize(serial);
    serial.WriteFile("StateTest.json");


    ForLease->Resources.LoadMesh("BoxMesh.json");





//    Entity* entity = AddEntity();
//    entity->AddComponent(new Components::Transform(*entity, Point(0,0), 100, 100, 0));

}

void SeanState::Initialize() {
    Serializer serial;
    serial.ReadFile("StateTest.json");
    Deserialize(serial);

    Health = 300;
}

void SeanState::Update() {


    for (FLE::Entity* entity : Entities) {
        entity->Update();
    }

    for (FLE::LevelComponent* levelComponent : LevelComponents) {
        levelComponent->Update(Entities);
    }

    ForLease->GameWindow->UpdateGameWindow();

    --Health;
    if (Health <= 0) ForLease->GameStateManager().SetAction(Modules::StateAction::Restart);
}

void SeanState::Deinitialize() {
    DeleteAllEntities();
}

void SeanState::Unload() {}
