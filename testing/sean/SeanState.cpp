#include "SeanState.h"
#include "ComponentsInclude.h"
#include "ResourceManager.h"
#include "Vector.h"
#include "Face.h"
#include "Edge.h"
#include "Mesh.h"

#include <iostream>

namespace FLE = ForLeaseEngine;
using namespace ForLeaseEngine;

void SeanState::Load() {
    FLE::LevelComponents::Renderer* renderer = new FLE::LevelComponents::Renderer(*this);
    FLE::Entity* camera = AddEntity();
    camera->AddComponent(new FLE::Components::Transform(*camera, FLE::Point(0,0), 1, 1, 0));
    camera->AddComponent(new FLE::Components::Camera(*camera, 0, 1, 720));
    renderer->SetCamera(*camera);

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
    box->SetFace(IndexedFace(1,2,3), 1);

    box->SetFaceColor(Color(1,0,0), 0);
    box->SetFaceColor(Color(1,0,0), 1);

    Entity* entity = AddEntity();
    entity->AddComponent(new Components::Transform(*entity, Point(0,0), 100, 100, 0));
    entity->AddComponent

}

void SeanState::Initialize() {

}

void SeanState::Update() {
    for (FLE::Entity* entity : Entities) {
        entity->Update();
    }

    for (FLE::LevelComponent* levelComponent : LevelComponents) {
        levelComponent->Update(Entities);
    }

    ForLease->GameWindow->UpdateGameWindow();

    std::cout << ForLease->FrameRateController().GetDt() << std::endl;
}

void SeanState::Deinitialize() {}

void SeanState::Unload() {}
