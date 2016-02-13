/*!
    \file   TestingState.cpp
    \author Christopher Hudson

    \brief
        A state, used for testing right?

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/
#include "TestingState.h"
#include "Face.h"
#include "Edge.h"
#include "Vector.h"
#include "KeyboardEvent.h"
#include "Event.h"
#include "Keys.h"
#include "Vector.h"
#include "Texture.h"
#include "TextureRegion.h"
#include "Mesh.h"
#include "BmFont.h"
#include "Font.h"
#include "UpdateEvent.h"
#include "GL/gl.h"
#include "ComponentsInclude.h"
#include "LevelComponentsInclude.h"
#include "Timer.h"
#include "Serialize.h"
#include "MouseButtonEvent.h"
#include "MouseMotionEvent.h"
//#include "Audio.h"
#include "SoundFS.h"
#include "MeshAnimation.h"
#include "ComponentTransformModeControls.h"
#include "ComponentScaleWithKeyboard.h"
#include "ComponentVisionCone.h"
#include "ComponentEnemyAI.h"
#include "ComponentFadeWithDistance.h"
#include "ComponentFollow.h"
#include "ComponentChangeLevelOnCollide.h"
#include "ComponentBackgroundMusic.h"
#include "Mouse.h"
#include "fmod.hpp"
#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace ForLeaseEngine;

class DTPrinter {
    public:
    void PrintDt(const ForLeaseEngine::Event* e);
};

Entity* camera;
LevelComponents::Renderer* render;
Components::SoundEmitter* soundEmitter;
float frameTime = 0;
LevelComponents::Collision* collisionSystem;
LevelComponents::Physics* physicsSystem;


class CameraController {
public:
    void OnKeyDown(const ForLeaseEngine::Event* e) {
        const ForLeaseEngine::KeyboardEvent* keyboard_e = reinterpret_cast<const ForLeaseEngine::KeyboardEvent*>(e);
        std::cout << "Got KeyDown Event" << std::endl;
        Components::Transform* trans = camera->GetComponent<Components::Transform>();
        switch(keyboard_e->Key) {
        case Keys::W:
            trans->Position[1] += 0.01f;
            break;
        case Keys::A:
            trans->Position[0] -= 0.01f;
            break;
        case Keys::S:
            trans->Position[1] -= 0.01f;
            break;
        case Keys::D:
            trans->Position[0] += 0.01f;
            break;
        case Keys::E:
            trans->Rotation -= 3.1415927 / 20;
            break;
        case Keys::Q:
            trans->Rotation += 3.1415927 / 20;
            break;
        case Keys::One:
            soundEmitter->SetVolume(1.0f, "BalloonDetection");
            soundEmitter->PlayEvent("BalloonDetection");
            soundEmitter->StopEvent("BalloonDetection");
            printf("sound on");
            //render->SetClearColor(1 ,0, 0, 1);
            break;
        case Keys::Two:
            soundEmitter->SetVolume(1.0f, "Jumping");
            soundEmitter->PlayEvent("Jumping");
            soundEmitter->PlayEvent("Jumping");
            soundEmitter->StopEvent("Jumping");
            printf("sound on");
            break;
        case Keys::Three:
            soundEmitter->SetVolume(1.0f, "Footstep");
            soundEmitter->PlayEvent("Footstep");
            soundEmitter->StopEvent("Footstep");
            printf("sound on");
            break;
        case Keys::Space:
            //soundEmitter->PlayEvent();
            break;
        }
    }

    void OnKeyUp(const ForLeaseEngine::Event* u)
    {
       const ForLeaseEngine::KeyboardEvent* keyboard_u = reinterpret_cast<const ForLeaseEngine::KeyboardEvent*>(u);
       std::cout<< "Get KeyUp event" << std::endl;
       (switch keyboard_u->Key)
       {
         case Keys::One:
             soundEmitter->SetPause(true, "BalloonDetection");
             break;
         case Keys::Two:
             soundEmitter->SetPause(true, "Jumping");
             break;
         case Keys::Three:
             soundEmitter->SetPause(true, "Footstep");
             break;
       }
    }

};


CameraController cameraController;


void TestingState::Load() {
    std::cout << "TestingState Load" << std::endl;
    Name = "TestingState";
}

void TestingState::Initialize() {
    ForLease->Dispatcher.Attach(NULL, &cameraController, "KeyDown", &CameraController::OnKeyDown);
    ForLease->Dispatcher.Attach(NULL, &cameraController, "KeyUp", &CameraController::OnKeyUp);
    render = new LevelComponents::Renderer(*this);

    camera = AddEntity();
    camera->AddComponent(new Components::Transform(*camera, 0, 0, 0.01f, 0.01f, 0, -2));
    camera->AddComponent(new Components::Camera(*camera, 0, 1, 10));

    soundEmitter = new Components::SoundEmitter(*camera);
    //soundEmitter->Looping = true;
    //soundEmitter->LoopCount = -1;

    render->SetCamera(camera->GetID());
    render->SetClearColor(0.3f, 0.3f, 0.5f, 1);

    render->SetCamera(camera->GetID());
    LevelComponents.push_back(render);
    collisionSystem = new LevelComponents::Collision(*this);
    LevelComponents.push_back(collisionSystem);
    physicsSystem = new LevelComponents::Physics(*this);
    physicsSystem->SetGravity(Vector(0, -1));
    LevelComponents.push_back(physicsSystem);
}

void TestingState::Update() {
    double dt = ForLease->FrameRateController().GetDt();
    frameTime += dt;
    ForLease->OSInput.ProcessAllInput();
    ForLease->sound->Update(dt);
    physicsSystem->Update(Entities);
    collisionSystem->Update(Entities);
    for(unsigned int i = 0; i < Entities.size(); ++i) {
        Entities[i]->Update();
    }


    render->Update(Entities);

    ForLease->GameWindow->UpdateGameWindow();
}

void TestingState::Deinitialize() {
    std::cout << "TestingState Deinitialize" << std::endl;
    DeleteAllEntities();
    DeleteAllLevelComponents();
}

void TestingState::Unload() {
    std::cout << "TestingState Unload" << std::endl;
}


