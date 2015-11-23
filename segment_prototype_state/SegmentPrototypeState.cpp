/*!
    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "SegmentPrototypeState.h"
#include "Vector.h"
#include "ComponentsInclude.h"

//#define DEBUG_DRAW

namespace ForLeaseEngine {
    static LevelComponents::Renderer* render;

    void SegmentPrototypeState::Load() {
        /*Components::Collision* collider;
        Name = "Segment Prototype";

        render = new LevelComponents::Renderer(*this);
        render->SetClearColor(0.2, 0.2, 0.8, 1);
        AddLevelComponent(render);

        AddLevelComponent(new LevelComponents::Physics(*this, Vector(0, -9.8)));

        AddLevelComponent(new LevelComponents::Collision(*this));

        Entity* camera = AddEntity("Camera");
        camera->AddComponent(new Components::Transform(*camera, Point(0, 6), 1, 1, 0));
        camera->AddComponent(new Components::Camera(*camera, 0, 1, 20));

        render->SetCamera(*camera);

        ///////////////Behind Player////////////

        Entity* ground = AddEntity("Ground");
        ground->AddComponent(new Components::Transform(*ground, Point(0, 0), 30, 5, 0));
        ground->AddComponent(new Components::Model(*ground, true, "GroundMesh.json", "", Color(1, 1, 1, 1), BlendMode::NONE, false, false));

        Entity* building = AddEntity("Building");
        building->AddComponent(new Components::Transform(*building, Point(0, 3), 3, 3, 0));
        building->AddComponent(new Components::Model(*building, true, "Building1Mesh.json", "", Color(1, 1, 1, 1), BlendMode::NONE, true));

        ///////////////Player///////////////////////
        Entity* player = AddEntity("Player");
        player->AddComponent(new Components::Transform(*player, Point(-15, 10), 2, 2, 0));
        player->AddComponent(new Components::Model(*player, true, "MainCharMesh.json", "", Color(1, 1, 1, 1), BlendMode::NONE, true));
        collider = new Components::Collision(*player);
        collider->Width = 1.4;
        collider->Height = 2;
        player->AddComponent(collider);
        player->AddComponent(new Components::Physics(*player));
        Components::CharacterController* controller = Components::CharacterController::Create(*player);
        controller->JumpSpeed = 10;
        controller->MoveSpeed = 10;
        player->AddComponent(controller);

        Entity* block = AddEntity("Block");
        block->AddComponent(new Components::Transform(*block, Point(-13.75, -1.5), 4, 4, 0));
        block->AddComponent(new Components::Model(*block, true, "Platform1Mesh.json", "", Color(1, 1, 1, 1), BlendMode::NONE, true));
        collider = new Components::Collision(*block);
        collider->Width = 8;
        collider->Height = 1.1;
        block->AddComponent(collider);

        block = AddEntity("Block");
        block->AddComponent(new Components::Transform(*block, Point(-5.75, -1.5), 4, 4, 0));
        block->AddComponent(new Components::Model(*block, true, "Platform1Mesh.json", "", Color(1, 1, 1, 1), BlendMode::NONE, true));
        collider = new Components::Collision(*block);
        collider->Width = 8;
        collider->Height = 1.1;
        block->AddComponent(collider);

        block = AddEntity("Block");
        block->AddComponent(new Components::Transform(*block, Point(2.25, -1.5), 4, 4, 0));
        block->AddComponent(new Components::Model(*block, true, "Platform1Mesh.json", "", Color(1, 1, 1, 1), BlendMode::NONE, true));
        collider = new Components::Collision(*block);
        collider->Width = 8;
        collider->Height = 1.1;
        block->AddComponent(collider);

        block = AddEntity("Block");
        block->AddComponent(new Components::Transform(*block, Point(10.25, -1.5), 4, 4, 0));
        block->AddComponent(new Components::Model(*block, true, "Platform1Mesh.json", "", Color(1, 1, 1, 1), BlendMode::NONE, true));
        collider = new Components::Collision(*block);
        collider->Width = 8;
        collider->Height = 1.1;
        block->AddComponent(collider);

        block = AddEntity("Block");
        block->AddComponent(new Components::Transform(*block, Point(6.25, 1.5), 2, 2, 0));
        block->AddComponent(new Components::Model(*block, true, "Platform2Mesh.json", "", Color(1, 1, 1, 1), BlendMode::NONE, true));
        collider = new Components::Collision(*block);
        collider->Width = 4;
        collider->Height = 1.1;
        block->AddComponent(collider);

        block = AddEntity("Block");
        block->AddComponent(new Components::Transform(*block, Point(8.25, 3.5), 2, 2, 0));
        block->AddComponent(new Components::Model(*block, true, "Platform2Mesh.json", "", Color(1, 1, 1, 1), BlendMode::NONE, true));
        collider = new Components::Collision(*block);
        collider->Width = 4;
        collider->Height = 1.1;
        block->AddComponent(collider);

        ////////////In front of player//////////////
        building = AddEntity("Building");
        building->AddComponent(new Components::Transform(*building, Point(10, 3), 4, 4, 0));
        building->AddComponent(new Components::Model(*building, true, "Building1Mesh.json", "", Color(1, 1, 1, 1), BlendMode::NONE, true));
        building->AddComponent(new Components::Collision(*building, 4.8, 8));

        building = AddEntity("Building");
        building->AddComponent(new Components::Transform(*building, Point(-8, 2), 5, 5, 0));
        building->AddComponent(new Components::Model(*building, true, "Building1Mesh.json", "", Color(1, 1, 1, 1), BlendMode::NONE, true));


        //////////ADDED BY SEAN, SORRY CHRIS, WANTED TO GIVE YOU A LIGHTING DEMO//////////
        Entity* light = AddEntity("Light");
        light->AddComponent(new Components::Transform(*light, Point(-5, 10), 0, 0, 0));
        light->AddComponent(new Components::Light(*light, Vector(-1,-1), Vector(1,-1), 1000, 700));



        Serializer levelWriter;
        Serialize(levelWriter);
        levelWriter.WriteFile("SegmentPrototype.json");*/

        /////////////////READ IT IN!!!!!/////////////
        Serializer levelReader;
        levelReader.ReadFile("SegmentPrototype.json");
        Deserialize(levelReader);
        for(unsigned int i = 0; i < LevelComponents.size(); ++i) {
            render = dynamic_cast<LevelComponents::Renderer*>(LevelComponents[i]);
            if(render)
                break;
        }
    }

    void SegmentPrototypeState::Initialize() {

    }

    void SegmentPrototypeState::Update() {
        ForLease->OSInput.ProcessAllInput();

        //////////ADDED BY SEAN, SORRY CHRIS, WANTED TO GIVE YOU A LIGHTING DEMO//////////
        for (Entity* entity : Entities) entity->Update();


        for(unsigned int i = 0; i < LevelComponents.size(); ++i) {
            LevelComponents[i]->Update(Entities);
        }

        #ifdef DEBUG_DRAW
        render->SetDrawingColor(0, 1, 0, 1);
        for(unsigned int i = 0; i < Entities.size(); ++i) {
            Components::Transform* trans = Entities[i]->GetComponent<Components::Transform>();
            if(Entities[i]->HasComponent(ComponentType::Collision)) {
                Components::Collision* collider = Entities[i]->GetComponent<Components::Collision>();
                render->DrawRectangle(trans->Position, collider->Width, collider->Height, trans->Rotation);
            }
        }

        render->SetDrawingColor(1, 0, 0, 1);
        for(unsigned int i = 0; i < Entities.size(); ++i) {
            Components::Transform* trans = Entities[i]->GetComponent<Components::Transform>();
            if(Entities[i]->HasComponent(ComponentType::Physics)) {
                Components::Physics* rbody = Entities[i]->GetComponent<Components::Physics>();
                render->DrawArrow(trans->Position, rbody->Velocity);
            }
        }
        #endif

        ForLease->GameWindow->UpdateGameWindow();
    }

    void SegmentPrototypeState::Deinitialize() {

    }

    void SegmentPrototypeState::Unload() {

    }
}
