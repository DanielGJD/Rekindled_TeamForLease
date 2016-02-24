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


    // BOX
    //FLE::Mesh* box = new FLE::Mesh(4, 4, 2);
    //box->SetVertex(Point(-1,-1), 0);
    //box->SetVertex(Point(-1,1), 1);
    //box->SetVertex(Point(1,1), 2);
    //box->SetVertex(Point(1,-1), 3);

    //box->SetEdge(IndexedEdge(0,1), 0);
    //box->SetEdge(IndexedEdge(1,2), 1);
    //box->SetEdge(IndexedEdge(2,3), 2);
    //box->SetEdge(IndexedEdge(3,0), 3);

    //box->SetFace(IndexedFace(0,1,2), 0);
    //box->SetFace(IndexedFace(0,2,3), 1);

    //box->SetFaceColor(Color(1,0,0), 0);
    //box->SetFaceColor(Color(1,0,0), 1);

    //Serializer serial;
    //box->Serialize(serial);
    //serial.WriteFile("BoxMesh.json");

    // TRIANGLE
    //FLE::Mesh* triangle = new FLE::Mesh(3, 3, 1);
    //triangle->SetVertex(Point(-1, -1), 0);
    //triangle->SetVertex(Point(1, -1), 1);
    //triangle->SetVertex(Point(0, 1), 2);

    //triangle->SetEdge(IndexedEdge(0, 1), 0);
    //triangle->SetEdge(IndexedEdge(1, 2), 1);
    //triangle->SetEdge(IndexedEdge(2, 0), 2);

    //triangle->SetFace(IndexedFace(0, 1, 2), 0);

    //Serializer triSerial;
    //triangle->Serialize(triSerial);
    //triSerial.WriteFile("TriMesh.json");



    //FLE::Entity* entityBox = AddEntity("Box");
    //entityBox->AddComponent(new Components::Transform(*entityBox, 0, 0, 1, 1, 0));
    //entityBox->AddComponent(new Components::Model(*entityBox, true, "TriMesh.json", "", Color(1,1,1,1)));
    //entityBox->AddComponent(new Components::Collision(*entityBox, 2, 2));
    //entityBox->GetComponent<Components::Collision>()->Initialize();
    //entityBox->AddComponent(new Components::Physics(*entityBox, 1, Vector(0,10)));
    //Components::CharacterController* controller = Components::CharacterController::Create(*entityBox);
    //controller->JumpSpeed = 10;
    //controller->MoveSpeed = 10;
    //entityBox->AddComponent(controller);

    //FLE::Entity* entityBox = AddEntity("Box");
    //entityBox->AddComponent(new Components::Transform(*entityBox, 0, 0, 1, 1, 0));
    //entityBox->AddComponent(new Components::Model(*entityBox, true, "BoxMesh.json", "", Color(1,1,1,1)));
    //entityBox->AddComponent(new Components::Collision(*entityBox, 2, 2));
    //entityBox->AddComponent(new Components::Physics(*entityBox, 1, Vector(0,10)));
    //Components::CharacterController* controller = Components::CharacterController::Create(*entityBox);
    //controller->JumpSpeed = 10;
    //controller->MoveSpeed = 10;
    //entityBox->AddComponent(controller);

    ////entityBox->CreateArchetype("Box.arch");

    ////SpawnArchetype("Box.arch", Point(-100, 0), "Box1");
    ////SpawnArchetype("Box.arch", Point(100, 100), "Box2");

    //Entity* entityFloor = AddEntity("Floor");
    //entityFloor->AddComponent(new Components::Transform(*entityFloor, 0, -10, 250, 1, 0));
    //entityFloor->AddComponent(new Components::Model(*entityFloor, true, "BoxMesh.json", "", Color(0,1,1,1)));
    //entityFloor->AddComponent(new Components::Collision(*entityFloor, 250, 2));

    //Entity* entityLight = AddEntity("Light");
    //entityLight->AddComponent(new Components::Transform(*entityLight, Point(-10,10), 1, 1, -1, 0));
    //entityLight->AddComponent(new Components::Light(*entityLight));

    Serializer serial2;
    Serialize(serial2);
    serial2.WriteFile("StateTest.json");


    //ForLease->Resources.LoadMesh("BoxMesh.json");

    //DeleteAllEntities();
    //DeleteAllLevelComponents();





//    Entity* entity = AddEntity();
//    entity->AddComponent(new Components::Transform(*entity, Point(0,0), 100, 100, 0));

}

void SeanState::Initialize() {
    Serializer serial;
    serial.ReadFile("StateTest.json");
    Deserialize(serial);

    Entity* background = AddEntity("Background");
    background->AddComponent(new Components::Transform(*background));
    background->AddComponent(new Components::Sprite(*background));
    ForLease->Resources.LoadTexture("bg7.png");
    Texture* texture = Texture::CreateTexture("bg7.png");
    //TextureRegion textureRegion(texture, 0, texture->GetWidth(), 0, texture->GetHeight());
    //background->GetComponent<Components::Sprite>(true)->SpriteSource.push_back(textureRegion);
    background->GetComponent<Components::Sprite>(true)->AnimationActive = false;
    background->GetComponent<Components::Sprite>(true)->SetSpriteSource("bg7.png");
    background->GetComponent<Components::Transform>(true)->ScaleX = 0.05;
    background->GetComponent<Components::Transform>(true)->ScaleY = 0.05;

    //Entity* menu = AddEntity("MainMenu");
    //menu->AddComponent(new Components::Transform(*menu));
    //menu->AddComponent(new Components::Menu(*menu));
    //Components::Menu* menuComp = menu->GetComponent<Components::Menu>();
    //menuComp->AddItem(new MenuItems::NextLevel("ButtonPlay.png"));
    //menuComp->AddItem(new MenuItems::ActivateAndDeactivate("ButtonQuit.png", "QuitConfirm", "MainMenu"));
    //menuComp->Activate();

    //Entity* quitConfirm = AddEntity("QuitConfirm");
    //quitConfirm->AddComponent(new Components::Transform(*quitConfirm));
    //quitConfirm->AddComponent(new Components::Menu(*quitConfirm));
    //Components::Menu* quitConfirmComp = quitConfirm->GetComponent<Components::Menu>();
    //quitConfirmComp->AddItem(new MenuItems::Quit("ButtonQuit.png"));
    //quitConfirmComp->AddItem(new MenuItems::ActivateAndDeactivate("ButtonCancel.png", "MainMenu", "QuitConfirm"));

    //AddLevelComponent(new LevelComponents::Menu(*this));

    Health = 300;
    ForLease->FrameRateController().TimeScaling(1);
}

void SeanState::Update() {

    ForLease->OSInput.ProcessAllInput();

    for (FLE::Entity* entity : Entities) {
        entity->Update();
    }

    for (FLE::LevelComponent* levelComponent : LevelComponents) {
        levelComponent->Update(Entities);
    }

    //for (Entity* entity : Entities) {
    //    std::cout << entity->GetName() << std::endl;
    //}

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

    //renderer->DrawRectangleFilled(Point(0, -1), 0.2, 0.2);
    //Entity* entity = GetEntityAtPosition(Point(0,-1));
    //if (entity)
    //    std::cout << entity->GetName() << std::endl;
    //else
    //    std::cout << "No entity found." << std::endl;

    ForLease->GameWindow->UpdateGameWindow();

    --Health;
    //std::cout << Health << std::endl;
    //if (Health == 150) ForLease->FrameRateController().TimeScaling(.25);
    //if (Health == -150) ForLease->FrameRateController().TimeScaling(1);
//    if (Health <= 0) ForLease->GameStateManager().SetAction(Modules::StateAction::Restart);
}

void SeanState::Deinitialize() {
    DeleteAllEntities();
    DeleteAllLevelComponents();
}

void SeanState::Unload() {}
