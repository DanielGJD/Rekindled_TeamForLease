/*!
    \file   SecondState.cpp
    \author Sean McGeer

    \copyright ęCopyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "SecondState.h"
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

SecondState::SecondState() : State("SecondState") {}

void SecondState::Load() {
    FLE::LevelComponents::Renderer* renderer = new FLE::LevelComponents::Renderer(*this);
    FLE::Entity* camera = AddEntity("Camera");
    camera->AddComponent(new FLE::Components::Transform(*camera, FLE::Point(0, 0), 1, 1, 0));
    camera->AddComponent(new FLE::Components::Camera(*camera, 0, 1, 50));
    renderer->SetCamera(*camera);
    AddLevelComponent(renderer);
    AddLevelComponent(new LevelComponents::Physics(*this, Vector(0, -10)));
    AddLevelComponent(new LevelComponents::Collision(*this));
    AddLevelComponent(new LevelComponents::Menu(*this));
    AddLevelComponent(new LevelComponents::Light(*this));

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

    FLE::Entity* background = AddEntity("Background");
    background->AddComponent(new Components::Transform(*background, Point(0,30), 0.020, 0.020, 0, -50));
    background->AddComponent(new Components::Sprite(*background));
    background->GetComponent<Components::Sprite>(true)->SetSpriteSource("BG_Composite.png");


    FLE::Entity* entityBox = AddEntity("Box");
    entityBox->AddComponent(new Components::Transform(*entityBox, 0, 0, 1, 1, 0));
    entityBox->AddComponent(new Components::Model(*entityBox, true, false, false, "BoxMesh.json", "", Color(1,1,1,1)));
    entityBox->AddComponent(new Components::Collision(*entityBox, 2, 2/*, true, 0, 2*/));
    entityBox->AddComponent(new Components::Physics(*entityBox, 1));
    Components::CharacterController* controller = Components::CharacterController::Create(*entityBox);
    controller->JumpSpeed = 10;
    controller->MoveSpeed = 10;
    entityBox->AddComponent(controller);

    //entityBox->CreateArchetype("Box.arch");

    //SpawnArchetype("Box.arch", Point(-100, 0), "Box1");
    //SpawnArchetype("Box.arch", Point(100, 100), "Box2");

    //Entity* shitfuck = AddEntity();
    //shitfuck->AddComponent(new Components::Transform(*shitfuck, Point(3, 0)));
    //shitfuck->AddComponent(new Components::Model(*shitfuck, true, false, false, "BoxMesh.json", "", Color(1, 1, 1, 1)));
    //shitfuck->AddComponent(new Components::Collision(*shitfuck, 2, 2));
    //shitfuck->AddComponent(new Components::Physics(*shitfuck, 1/*, Vector(20, 0)*/));

    Entity* entityFloor = AddEntity("Floor");
    entityFloor->AddComponent(new Components::Transform(*entityFloor, 0, -10, 250, 1, 0));
    entityFloor->AddComponent(new Components::Model(*entityFloor, true, false, false, "BoxMesh.json", "", Color(0,1,1,1)));
    entityFloor->AddComponent(new Components::Collision(*entityFloor, 250, 2));

    Entity* entityLight = AddEntity("Light");
    entityLight->AddComponent(new Components::Transform(*entityLight, Point(-10,10), 1, 1, -1, 500));
    entityLight->AddComponent(new Components::Light(*entityLight, Vector(1,-1), Color(0,1,0,0.5), 0.5, 50));

    Entity* entityWall = AddEntity("Wall");
    entityWall->AddComponent(new Components::Transform(*entityWall, 10, 0, 1, 10, 0, 1));
    entityWall->AddComponent(new Components::Model(*entityWall, true, false, false, "BoxMesh.json", "", Color(0, 1, 1, 1)));
    entityWall->AddComponent(new Components::Collision(*entityWall, 2, 20));

    Serializer serial2;
    Serialize(serial2);
    serial2.WriteFile("SecondStateTest.json");


    ForLease->Resources.LoadMesh("BoxMesh.json");

    DeleteAllEntities();
    DeleteAllLevelComponents();



    //    Entity* entity = AddEntity();
    //    entity->AddComponent(new Components::Transform(*entity, Point(0,0), 100, 100, 0));

}

void SecondState::Initialize() {
    Serializer serial;
    serial.ReadFile("SecondStateTest.json");
    Deserialize(serial);

    //Entity* menu = AddEntity("Menu");
    //menu->AddComponent(new Components::Transform(*menu));
    //menu->AddComponent(new Components::Menu(*menu));
    //Components::Menu* menuComp = menu->GetComponent<Components::Menu>();
    //menuComp->AddLoadLevel("HI", "SecondState");
    //menuComp->AddLoadLevel("YO", "SecondState");
    //menuComp->Activate();

    //AddLevelComponent(new LevelComponents::Menu(*this));

    Health = 300;
    //ForLease->FrameRateController().TimeScaling(1);
}

void SecondState::Update() {

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

    Entity* camera = GetEntityByName("Camera", true);
    Entity* player = GetEntityByName("Box", true);
    camera->GetComponent<Components::Transform>(true)->Position = player->GetComponent<Components::Transform>(true)->Position;

    //std::cout << player->GetComponent<Components::Physics>(true)->Acceleration << std::endl;
    //std::cout << player->GetComponent<Components::Physics>(true)->Velocity << std::endl;


    //std::cout << "=========================================================" << std::endl;

    LevelComponents::Renderer* renderer = ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Renderer>();

    //std::vector<Ray> rays;
    //rays.push_back(Ray(Point(1, 1), Vector(0,-1), 15));
    //rays.push_back(Ray(Point(-1, -1), Vector(0, 1), 15));
    //rays.push_back(Ray(Point(0, 0), Vector(-1, -1), 15));
    ////rays.push_back(Ray(Point(-2, -2), Vector(1, 0.5), 15));
    //rays.push_back(Ray(Point(5, 5), Vector(1, 1), 15));

    //renderer->DrawRectangle(Point(0, 0), 10, 10, 0);

    //for (Ray ray : rays) {
    //    for (Entity* entity : Entities) {
    //        ray.IsColliding(entity);
    //    }
    //    renderer->SetDrawingColor(Color(1, 1, 1));
    //    renderer->DrawArrow(ray.GetStart(), ray.GetScaledVector());
    //    renderer->DrawRectangle(ray.GetStart(),0.5,0.5);
    //}

    //for (Ray ray : rays) {
    //    ray.IsColliding(GetEntityByName("Box"));
    //    renderer->SetDrawingColor(Color(1, 1, 1));
    //    renderer->DrawArrow(ray.GetStart(), ray.GetScaledVector());
    //    renderer->DrawRectangle(ray.GetStart(),0.5,0.5);
    //}

    //Ray ray(Point(0, 0), Vector(0, -1));

    //Entity* collidedWith = Ray::CheckCollisions(ray, Entities);

    //if (collidedWith) std::cout << collidedWith->GetName() << std::endl;

    //renderer->DrawArrow(ray.GetStart(), ray.GetScaledVector());

    //renderer->DrawRectangleFilled(Point(0, -1), 0.2, 0.2);
    //Entity* entity = GetEntityAtPosition(Point(0,-1));
    //if (entity)
    //    std::cout << entity->GetName() << std::endl;
    //else
    //    std::cout << "No entity found." << std::endl;

    ForLease->GameWindow->UpdateGameWindow();

    --Health;
    //if (Health == 150) ForLease->FrameRateController().TimeScaling(.25);
    //if (Health == -150) ForLease->FrameRateController().TimeScaling(1);
    //    if (Health <= 0) ForLease->GameStateManager().SetAction(Modules::StateAction::Restart);
}

void SecondState::Deinitialize() {
    DeleteAllEntities();
    DeleteAllLevelComponents();
}

void SecondState::Unload() {}
