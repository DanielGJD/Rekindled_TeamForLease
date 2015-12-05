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
#include "Audio.h"
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

Point mousePos;
Mesh* testMesh;
Entity* camera;
bool addObject = false;
LevelComponents::Renderer* render;
Sound* sound;
Components::SoundEmitter* soundEmitter;
MeshAnimation* animation;
float frameTime = 0;
Components::TransformModeControls* transCont;
Components::ScaleWithKeyboard* keyScale;
Components::VisionCone* visionCone;
Components::EnemyAI* enemyAI;
LevelComponents::Collision* collisionSystem;
LevelComponents::Physics* physicsSystem;

class MouseListener {
    public:
    void OnMouseDown(const ForLeaseEngine::Event* e) {
        const ForLeaseEngine::MouseButtonEvent* mouse_e = reinterpret_cast<const ForLeaseEngine::MouseButtonEvent*>(e);
        std::cout << mouse_e->EventName << ":" << std::endl
                  << mouse_e->Button << std::endl
                  << mouse_e->Clicks << std::endl
                  << "[" << mouse_e->ScreenLocation[0] << "," << mouse_e->ScreenLocation[1] << "]" << std::endl << std::endl;
        mousePos[0] = mouse_e->ScreenLocation[0] - 720 / 2;
        mousePos[1] = -(mouse_e->ScreenLocation[1] - 720 / 2);
        //addObject = true;
        if(mouse_e->Button == MouseButton::Left) {
            //testMesh->DeleteVertex(0);
        }
        else if(mouse_e->Button == MouseButton::Right) {
            //testMesh->AddFace(0, 3, 5, 1, 1, 1, 1);
        }
    }

    void OnMouseUp(const ForLeaseEngine::Event* e) {
        const ForLeaseEngine::MouseButtonEvent* mouse_e = reinterpret_cast<const ForLeaseEngine::MouseButtonEvent*>(e);
        std::cout << mouse_e->EventName << ":" << std::endl
                  << mouse_e->Button << std::endl
                  << mouse_e->Clicks << std::endl
                  << "[" << mouse_e->ScreenLocation[0] << "," << mouse_e->ScreenLocation[1] << "]" << std::endl << std::endl;
    }

    void OnMouseMove(const ForLeaseEngine::Event* e) {
        const ForLeaseEngine::MouseMotionEvent* mouse_e = reinterpret_cast<const ForLeaseEngine::MouseMotionEvent*>(e);
        ForLeaseEngine::MouseMotionEvent temp = *mouse_e;
        //std::cout << temp << std::endl;
        Point p = render->ScreenToWorld(Point(mouse_e->X, mouse_e->Y));
        p = render->WorldToScreen(p);
        std::cout << "X: " << p[0] << "|Y: " << p[1] << std::endl;
    }
};

class CameraController {
public:
    void OnKeyDown(const ForLeaseEngine::Event* e) {
        const ForLeaseEngine::KeyboardEvent* keyboard_e = reinterpret_cast<const ForLeaseEngine::KeyboardEvent*>(e);
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
            soundEmitter->Play("fluteCheerful1.wav");
            break;
        case Keys::Two:
            soundEmitter->Play("harpgraceful1.wav");
            break;
        case Keys::Three:
            soundEmitter->Play("frenchhornupset.wav");
            break;
        case Keys::Space:
            soundEmitter->Play();
            break;
        }
    }
};

DTPrinter printer;
MouseListener mouse;
Entity* spriteTest;
Texture* testTexture;
CameraController cameraController;
Font* arial32;
int numToDraw = 1;
Components::Model* model;
Components::DragWithMouse* mouseDrag;

void DTPrinter::PrintDt(const ForLeaseEngine::Event* e) {
    const ForLeaseEngine::UpdateEvent* updateEvent = reinterpret_cast<const ForLeaseEngine::UpdateEvent*>(e);
    std::cout << "FPS: " << static_cast<int>(1 / updateEvent->dt) << std::endl;
}

void TestingState::Load() {
    std::cout << "TestingState Load" << std::endl;
    Name = "TestingState";
}

void TestingState::Initialize() {
    std::cout << "TestingState Initialize" << std::endl;
    ForLease->FrameRateController().TimeScaling(1);
    ForLease->Resources.LoadTexture("bg7.png");
    render = new LevelComponents::Renderer(*this);
    //ForLease->Dispatcher.Attach(NULL, &mouse, "MouseButtonDown", &MouseListener::OnMouseDown, this);
    //MouseButtonEvent me = MouseButtonEvent("MouseButtonDown");
    //me.ScreenLocation = Point(99, 99);
    //ForLease->Dispatcher.DispatchToParent(&me, this);

    Components::Transform* transform = new Components::Transform(*spriteTest, 0, 0, 100, 100, 0);

    model = new Components::Model(*spriteTest, true, false, false, "MeshTest.json", "", Color(1, 1, 1, 1), BlendMode::NONE, false, false);
    testMesh = ForLease->Resources.GetMesh("MeshTest.json");

    model->AnimationActive = true;
    model->SetAnimation("MainCharWalkAni.json");
    model->FrameRate = 24;
    model->Looping = true;
    model->FlipY = true;
    ForLease->Resources.GetMeshAnimation("MainCharWalkAni.json");

    ForLease->Resources.LoadFont("Arial.fnt");

    Components::SpriteText* text = new Components::SpriteText(*spriteTest, "");
    Serializer spriteTextReader;
    spriteTextReader.ReadFile("SpriteText.json");
    text->Deserialize(spriteTextReader);


    Entity* entity = AddEntity();

    soundEmitter = new Components::SoundEmitter(*entity);

    Serializer soundEmitterReader;
    soundEmitterReader.ReadFile("SoundEmitter.json");
    soundEmitter->Deserialize(soundEmitterReader);


    mouseDrag = Components::DragWithMouse::Create(*entity);
    Serializer mouseDragReader;
    mouseDragReader.ReadFile("MouseDragTest.json");
    mouseDrag->Deserialize(mouseDragReader);
    mouseDrag->Active = false;

    //transCont = new Components::TransformModeControls(*entity);
    //transCont->SlowMotionSpeed = 0.1f;
    //transCont->Initialize();

    keyScale = Components::ScaleWithKeyboard::Create(*entity);

        entity->AddComponent(new Components::Transform(*entity));
        entity->AddComponent(model);
        entity->AddComponent(soundEmitter);
        entity->AddComponent(new Components::Collision(*entity));
        entity->AddComponent(mouseDrag);
        entity->AddComponent(keyScale);
        //entity->AddComponent(transCont);
        visionCone = new Components::VisionCone(*entity, true, true, true, Vector(-0.2, 0.35), 4, Vector(-1, 0), PI / 2, Color(0.5f, 0, 0.5f, 1));
        entity->AddComponent(visionCone);
        enemyAI = new Components::EnemyAI(*entity);
        enemyAI->HatedEntityName = "AnotherEntity";
        enemyAI->LikedEntityName = "LastOne";
        enemyAI->LikedSeenSound = "BalloonDetection.wav";
        enemyAI->HatedSeenSound = "Detected.wav";
        enemyAI->Initialize();
        entity->AddComponent(enemyAI);
        entity->AddComponent(new Components::SoundEmitter(*entity));

    ////////////////AI write////////////////////
//    Serializer enemyAIWriter;
//    enemyAI->Serialize(enemyAIWriter);
//    enemyAIWriter.WriteFile("EnemyAITest.json");

    //////////////AI read////////////////////////
    //Serializer enemyAIRead;
    //enemyAIRead.ReadFile("EnemyAITest.json");
    //enemyAI->Deserialize(enemyAIRead);


    ///////////////Vision cone read///////////////
    Serializer visionConeReader;
    visionConeReader.ReadFile("VisionConeTest.json");
    visionCone->Deserialize(visionConeReader);

    Serializer mouseDragWriter;
    mouseDrag->Serialize(mouseDragWriter);
    mouseDragWriter.WriteFile("MouseDragTest.json");
    Serializer keyScaleWriter;
    keyScale->Serialize(keyScaleWriter);
    keyScaleWriter.WriteFile("KeyScaleTest.json");
    //Serializer transContWriter;
    //transCont->Serialize(transContWriter);
    //transContWriter.WriteFile("TransContTest.json");

    Entity* anotherEntity = AddEntity("AnotherEntity");
    anotherEntity->AddComponent(new Components::Transform(*anotherEntity, 0, 1.1, 1, 1, 0, -1));
    Components::Model* mainModel = new Components::Model(*anotherEntity, true, false, false, "MeshTest.json");
    mainModel->FrameRate = 24;
    mainModel->AnimationActive = true;
    mainModel->Looping = true;
    anotherEntity->AddComponent(mainModel);
    anotherEntity->AddComponent(new Components::Collision(*anotherEntity, 0.5, 1));
    anotherEntity->AddComponent(new Components::Physics(*anotherEntity));
    anotherEntity->AddComponent(Components::DragWithMouse::Create(*anotherEntity));
    Components::ScaleWithKeyboard* scaler = Components::ScaleWithKeyboard::Create(*anotherEntity);
    scaler->ScaleSound = "Scaling.wav";
    anotherEntity->AddComponent(scaler);
    Components::CharacterController* controller = Components::CharacterController::Create(*anotherEntity);
    controller->MoveSpeed = 2;
    controller->JumpSpeed = 1;
    controller->WalkAnimation = "MainCharWalkAni.json";
    controller->WalkSound = "Footstep.wav";
    controller->JumpSound = "Jumping.wav";
    controller->LandSound = "Landing.wav";
    anotherEntity->AddComponent(controller);
    Components::SoundEmitter* emitter = new Components::SoundEmitter(*anotherEntity);
    emitter->Volume = 0.1;
    anotherEntity->AddComponent(emitter);

    Entity* oneMoreEntity = AddEntity("OneMoreEntity");
    oneMoreEntity->AddComponent(new Components::Transform(*oneMoreEntity, -2, -3));
    oneMoreEntity->AddComponent(new Components::Model(*oneMoreEntity, true, false, false, "Building1Mesh.json"));
    oneMoreEntity->AddComponent(new Components::Collision(*oneMoreEntity, 1.2, 2));
    oneMoreEntity->AddComponent(Components::DragWithMouse::Create(*oneMoreEntity));
    oneMoreEntity->AddComponent(Components::ScaleWithKeyboard::Create(*oneMoreEntity));
    oneMoreEntity->AddComponent(new Components::SoundEmitter(*oneMoreEntity));
    Components::ChangeLevelOnCollide* levelSwitch = new Components::ChangeLevelOnCollide(*oneMoreEntity, true, "TestingState", "AnotherEntity", "", 0.5,4);
    //////////// switch test ///////////
    Serializer lsio;
    //write//
//    levelSwitch->Serialize(lsio);
//    lsio.WriteFile("ChangeLevelOnCollide.json");
    //read//
    lsio.ReadFile("ChangeLevelOnCollide.json");
    levelSwitch->Deserialize(lsio);

    oneMoreEntity->AddComponent(levelSwitch);

    Entity* lastOne = AddEntity("LastOne");
    lastOne->AddComponent(new Components::Transform(*lastOne, -2, 3));
    lastOne->AddComponent(new Components::Model(*lastOne, true, false, false, "Building1Mesh.json"));
    lastOne->AddComponent(new Components::Collision(*lastOne, 1.2, 2));
    lastOne->AddComponent(Components::DragWithMouse::Create(*lastOne));
    //lastOne->AddComponent(Components::ScaleWithKeyboard::Create(*lastOne));

    Entity* tehmuziks = AddEntity("TEHMUZIKHSBRUH");
    tehmuziks->AddComponent(new Components::Transform(*tehmuziks));
    tehmuziks->AddComponent(new Components::SoundEmitter(*tehmuziks));
    Components::BackgroundMusic* bgm = new Components::BackgroundMusic(*tehmuziks, "sneaky spook in the dark.wav");
    tehmuziks->AddComponent(bgm);
    Serializer gbmwrite;
    bgm->Serialize(gbmwrite);
    gbmwrite.WriteFile("bgm.json");

    camera = AddEntity();
    camera->AddComponent(new Components::Transform(*camera, 0, 0, 0.01f, 0.01f, 0, -2));
    camera->AddComponent(new Components::Camera(*camera, 0, 1, 10));
    Components::Sprite* newSprite = new Components::Sprite(*camera);
    newSprite->SetSpriteSource("bg7.png");
    newSprite->BlendingMode = BlendMode::ALPHA;

    //// sprite write ////
    //Serializer spriteWrite;
    //newSprite->Serialize(spriteWrite);
    //spriteWrite.WriteFile("SpriteTest.json");

    //// sprite read ////
//    Serializer spriteRead;
//    spriteRead.ReadFile("SpriteTest.json");
//    newSprite->Deserialize(spriteRead);
    camera->AddComponent(newSprite);
    Components::FadeWithDistance* fade = new Components::FadeWithDistance(*camera, true, true, true, 2, 4, anotherEntity->GetID());
    camera->AddComponent(fade);
    Components::SoundEmitter* camEmitter = new Components::SoundEmitter(*camera);
    camEmitter->Looping = true;
    camEmitter->LoopCount = -1;
    camera->AddComponent(camEmitter);
    transCont = new Components::TransformModeControls(*camera);
    transCont->SlowMotionSpeed = 0.5;
    transCont->Initialize();
    //transCont->TransformModeSound = "SlowMotion.wav";
    camera->AddComponent(transCont);

    //// fade write ////
//    Serializer fadeWithDistanceWriter;
//    fade->Serialize(fadeWithDistanceWriter);
//    fadeWithDistanceWriter.WriteFile("FadeWithDistanceTest.json");

    //// fade read ////
    Serializer fadeWithDistanceReader;
    fadeWithDistanceReader.ReadFile("FadeWithDistanceTest.json");
    fade->Deserialize(fadeWithDistanceReader);


    render->SetCamera(camera->GetID());
    render->SetClearColor(0.3f, 0.3f, 0.5f, 1);

    spriteTest = AddEntity();
    spriteTest->AddComponent(transform);
    //spriteTest->AddComponent(text);
    /////////////////////Entity Serialize Test///////////////////////
    Serializer entityWriter;
    spriteTest->Serialize(entityWriter);
    entityWriter.WriteFile("Entity.json");


    ///////////////////Renderer Deserialize Test//////////////////////
    Serializer rendererReader;
    rendererReader.ReadFile("RendererTest.json");
    render->Deserialize(rendererReader);

    render->SetClearColor(0.3f, 0.3f, 0.5f, 1);

    render->SetCamera(camera->GetID());
    LevelComponents.push_back(render);
    collisionSystem = new LevelComponents::Collision(*this);
    LevelComponents.push_back(collisionSystem);
    physicsSystem = new LevelComponents::Physics(*this);
    physicsSystem->SetGravity(Vector(0, -1));
    LevelComponents.push_back(physicsSystem);

    Serializer test;
    model->Serialize(test);
    test.WriteFile("Model.json");
}

void TestingState::Update() {
    double dt = ForLease->FrameRateController().GetDt();
    frameTime += dt;
    //std::cout << static_cast<int>(1 / dt) << std::endl;
    //ForLeaseEngine::UpdateEvent e = ForLeaseEngine::UpdateEvent(dt);
    //ForLease->Dispatcher.Dispatch(&e, NULL);
    //ForLease->Dispatcher.DispatchTo(&e, &printer);
    ForLease->OSInput.ProcessAllInput();
    ForLease->AudioSystem->Update();
    physicsSystem->Update(Entities);
    collisionSystem->Update(Entities);
    //Components::Sprite* sprite = reinterpret_cast<Components::Sprite*>(spriteTest->GetComponent(ComponentType::Sprite));
    //Components::Transform* transform = reinterpret_cast<Components::Transform*>(spriteTest->GetComponent(ComponentType::Transform));
    for(unsigned int i = 0; i < Entities.size(); ++i) {
        /*Components::Sprite* sprite = Entities[i]->GetComponent<Components::Sprite>();
        if(sprite)
            sprite->Update();
        Components::ScaleWithKeyboard* scale = Entities[i]->GetComponent<Components::ScaleWithKeyboard>();
        if(scale)
            scale->Update();
        if(Entities[i]->HasComponent(ComponentType::FadeWithDistance))
        {
            //std::cout << "Found it" << std::endl;
            Entities[i]->GetComponent<Components::FadeWithDistance>()->Update();
        }
        if(Entities[i]->HasComponent(ComponentType::Follow)){
            Entities[i]->GetComponent<Components::Follow>()->Update();
        }
        if(Entities[i]->HasComponent(ComponentType::ChangeLevelOnCollide)) {
            Entities[i]->GetComponent<Components::ChangeLevelOnCollide>()->Update();
        }*/
        Entities[i]->Update();
    }
    //enemyAI->Update();
    //model->Update();
    //if(mouseDrag)
        //mouseDrag->Update();

    render->Update(Entities);
    //render->SetDrawingColor(Color(1, 1, 1, 1));
    //render->SetDebugPointSize(4);

    /*float animationTime = model->GetFrameTime();
    float frameRate = model->FrameRate;
    unsigned int currentFrame = model->GetFrame();
    float t = animationTime * frameRate - currentFrame;
    for(unsigned int i = 0; i < animation->GetFrameVertexCount(); ++i) {
        render->DrawPoint(animation->InterpolateVertex(currentFrame, i, t));
    }*/
    //transCont->Update();
    //visionCone->Update();
    //Timer t;
    ForLease->GameWindow->UpdateGameWindow();
    //std::cout << t.GetTime() << std::endl;
}

void TestingState::Deinitialize() {
    std::cout << "TestingState Deinitialize" << std::endl;
    DeleteAllEntities();
    DeleteAllLevelComponents();
}

void TestingState::Unload() {
    std::cout << "TestingState Unload" << std::endl;
}


