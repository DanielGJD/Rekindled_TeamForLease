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
    AddLevelComponent(new LevelComponents::Collision(*this));

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
    entityBox->AddComponent(new Components::Transform(*entityBox, 0, 0, 25, 25, 0));
    entityBox->AddComponent(new Components::Model(*entityBox, true, "BoxMesh.json", "", Color(1,1,1,1)));
    entityBox->AddComponent(new Components::Collision(*entityBox, 50, 50));
    entityBox->AddComponent(new Components::Physics(*entityBox));

    entityBox->CreateArchetype("Box.arch");

    SpawnArchetype("Box.arch", Point(-100, 0), "Box1");
    SpawnArchetype("Box.arch", Point(100, 100), "Box2");
    SpawnArchetype("Box.arch");

    Entity* entityFloor = AddEntity("Floor");
    entityFloor->AddComponent(new Components::Transform(*entityFloor, 0, -250, 250, 10, 0));
    entityFloor->AddComponent(new Components::Model(*entityFloor, true, "BoxMesh.json", "", Color(0,1,1,1)));
    entityFloor->AddComponent(new Components::Collision(*entityFloor, 500, 20));

    Entity* entityLight = AddEntity("Light");
    entityLight->AddComponent(new Components::Transform(*entityLight, 0, 200, 0, 0, 0));
    entityLight->AddComponent(new Components::Light(*entityLight, Vector(-1,-1), Vector(1,-1), 1000, 700));

    Serializer serial2;
    Serialize(serial2);
    serial2.WriteFile("StateTest.json");


    ForLease->Resources.LoadMesh("BoxMesh.json");

    DeleteAllEntities();
    DeleteAllLevelComponents();





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

    LevelComponents::Renderer* renderer = ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Renderer>();

    Ray ray(Point(0, 100), Vector(0,-1), 700);

    ray.IsColliding(GetEntityByName("Box"));

//    renderer->DrawLine(ray.GetStart(), ray.GetScaledVector());

    ForLease->GameWindow->UpdateGameWindow();

    --Health;
//    if (Health <= 0) ForLease->GameStateManager().SetAction(Modules::StateAction::Restart);
}

void SeanState::Deinitialize() {
    DeleteAllEntities();
    DeleteAllLevelComponents();
}

void SeanState::Unload() {}
