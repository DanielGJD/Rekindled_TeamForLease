/*!
\file   RaycastTest.cpp
\author Sean McGeer

\copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "RaycastTest.h"
#include "ComponentsInclude.h"
#include "ResourceManager.h"
#include "Vector.h"
#include "Face.h"
#include "Edge.h"
#include "Mesh.h"
#include "GameStateManager.h"
#include "Ray.h"
#include "Engine.h"

#include <iostream>

namespace FLE = ForLeaseEngine;
using namespace ForLeaseEngine;

RaycastTest::RaycastTest() : State("RaycastTest") {}

void RaycastTest::Load() {
    FLE::LevelComponents::Renderer* renderer = new FLE::LevelComponents::Renderer(*this);
    FLE::Entity* camera = AddEntity("Camera");
    camera->AddComponent(new FLE::Components::Transform(*camera, FLE::Point(0, 0), 1, 1, 0));
    camera->AddComponent(new FLE::Components::Camera(*camera, 0, 1, 50));
    renderer->SetCamera(*camera);
    AddLevelComponent(renderer);
    AddLevelComponent(new LevelComponents::Physics(*this, Vector(0, -10)));
    AddLevelComponent(new LevelComponents::Collision(*this));
    AddLevelComponent(new LevelComponents::Menu(*this));

    FLE::Mesh* box = new FLE::Mesh(4, 4, 2);
    box->SetVertex(Point(-1, -1), 0);
    box->SetVertex(Point(-1, 1), 1);
    box->SetVertex(Point(1, 1), 2);
    box->SetVertex(Point(1, -1), 3);

    box->SetEdge(IndexedEdge(0, 1), 0);
    box->SetEdge(IndexedEdge(1, 2), 1);
    box->SetEdge(IndexedEdge(2, 3), 2);
    box->SetEdge(IndexedEdge(3, 0), 3);

    box->SetFace(IndexedFace(0, 1, 2), 0);
    box->SetFace(IndexedFace(0, 2, 3), 1);

    box->SetFaceColor(Color(1, 0, 0), 0);
    box->SetFaceColor(Color(1, 0, 0), 1);

    Serializer serial;
    box->Serialize(serial);
    serial.WriteFile("BoxMesh.json");

    delete box;


    FLE::Entity* entityBox = AddEntity("Box");
    entityBox->AddComponent(new Components::Transform(*entityBox, 0, 0, 1, 1, 0));
    entityBox->AddComponent(new Components::Model(*entityBox, true, false, false, "BoxMesh.json", "", Color(1, 1, 1, 1)));
    entityBox->AddComponent(new Components::Collision(*entityBox, 2, 2));

    FLE::Entity* entityFloor = AddEntity("Floor");
    entityFloor->AddComponent(new Components::Transform(*entityFloor, 0, -5, 25, 1, 0));
    entityFloor->AddComponent(new Components::Model(*entityFloor, true, false, false, "BoxMesh.json", "", Color(1, 1, 1, 1)));
    entityFloor->AddComponent(new Components::Collision(*entityFloor, 50, 2));

    FLE::Entity* entityShort = AddEntity("Short");
    entityShort->AddComponent(new Components::Transform(*entityShort, 5, 0, 1, 5, 0));
    entityShort->AddComponent(new Components::Model(*entityShort, true, false, false, "BoxMesh.json", "", Color(1, 1, 1, 1)));
    entityShort->AddComponent(new Components::Collision(*entityShort, 2, 10));

    FLE::Entity* entityMedium = AddEntity("Medium");
    entityMedium->AddComponent(new Components::Transform(*entityMedium, 15, 5, 1, 10, 0));
    entityMedium->AddComponent(new Components::Model(*entityMedium, true, false, false, "BoxMesh.json", "", Color(1, 1, 1, 1)));
    entityMedium->AddComponent(new Components::Collision(*entityMedium, 2, 20));

    FLE::Entity* entityTall = AddEntity("Tall");
    entityTall->AddComponent(new Components::Transform(*entityTall, 10, 10, 1, 15, 0));
    entityTall->AddComponent(new Components::Model(*entityTall, true, false, false, "BoxMesh.json", "", Color(1, 1, 1, 1)));
    entityTall->AddComponent(new Components::Collision(*entityTall, 2, 30));

    Serializer serial2;
    Serialize(serial2);
    serial2.WriteFile("RaycastTest.json");


    ForLease->Resources.LoadMesh("BoxMesh.json");

    DeleteAllEntities();
    DeleteAllLevelComponents();

    for (Entity* entity : Entities)
        std::cout << "There's still an entity!" << std::endl;



    //    Entity* entity = AddEntity();
    //    entity->AddComponent(new Components::Transform(*entity, Point(0,0), 100, 100, 0));

}

void RaycastTest::Initialize() {
    Serializer serial;
    serial.ReadFile("RaycastTest.json");
    Deserialize(serial);
}

void RaycastTest::Update() {

    ForLease->OSInput.ProcessAllInput();

    for (FLE::Entity* entity : Entities) {
        entity->Update();
    }

    for (FLE::LevelComponent* levelComponent : LevelComponents) {
        levelComponent->Update(Entities);
    }

    for (Entity* entity : Entities) {
        if (entity->HasComponent(ComponentType::Collision))
            entity->GetComponent<Components::Collision>()->DebugDraw();
    }

    Ray ray(Point(-5, 0), Vector(1, -1), 20.0f, 1);
    Entity* collision = ray.CheckCollisions(ray, Entities);

    if (collision) std::cout << collision->GetName() << std::endl;
    else std::cout << "No entity found." << std::endl;

    //std::cout << ray.IsColliding(ForLease->GameStateManager().CurrentState().GetEntityByName("Box")) << std::endl;;

    ForLeaseEngine::LevelComponents::Renderer* renderer = ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Renderer>(true);
    renderer->DrawArrow(ray.GetStart(), ray.GetScaledVector());

    ForLease->GameWindow->UpdateGameWindow();
}

void RaycastTest::Deinitialize() {
    DeleteAllEntities();
    DeleteAllLevelComponents();
}

void RaycastTest::Unload() {}
