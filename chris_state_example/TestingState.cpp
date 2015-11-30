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
#include "FrameRatePrinter.h"
#include "Audio.h"
#include "MeshAnimation.h"
#include "ComponentTransformModeControls.h"
#include "ComponentScaleWithKeyboard.h"
#include "ComponentVisionCone.h"
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
FrameRatePrinter fpsPrinter;
Components::Model* model;
Components::DragWithMouse* mouseDrag;

void DTPrinter::PrintDt(const ForLeaseEngine::Event* e) {
    const ForLeaseEngine::UpdateEvent* updateEvent = reinterpret_cast<const ForLeaseEngine::UpdateEvent*>(e);
    std::cout << "FPS: " << static_cast<int>(1 / updateEvent->dt) << std::endl;
}

void TestingState::Load() {
    ForLease->Resources.LoadSound("harpgraceful1.wav");
    ForLease->Resources.LoadSound("frenchhornupset.wav");
    ForLease->Resources.LoadSound("flutecheerful1.wav");
    ForLease->Resources.LoadTexture("bg7.png");
    render = new LevelComponents::Renderer(*this);
    ForLease->Dispatcher.Attach(NULL, &mouse, "MouseButtonDown", &MouseListener::OnMouseDown, this);
    MouseButtonEvent me = MouseButtonEvent("MouseButtonDown");
    me.ScreenLocation = Point(99, 99);
    ForLease->Dispatcher.DispatchToParent(&me, this);

    Components::Transform* transform = new Components::Transform(*spriteTest, 0, 0, 100, 100, 0);

    //spriteTest->AddComponent(sprite);
    model = new Components::Model(*spriteTest, true, false, false, "MeshTest.json", "", Color(1, 1, 1, 1), BlendMode::NONE, true, true);
    testMesh = ForLease->Resources.GetMesh("MeshTest.json");
//    testMesh->CreateAnimation("TestAnimation");
//    animation = testMesh->GetAnimation("TestAnimation");
//    animation->AddFrame();
//    animation->SetVertex(1, 12, Point(-1.0f / 11, -4.5f / 11));
//    animation->SetVertex(1, 13, Point(1.0f / 11, -4.5f / 11));
//    animation->SetVertex(1, 14, Point(-1.0f / 11, -3.5f / 11));
    animation = new MeshAnimation();

    /////////////////////Animation Serialization test///////////////////
//    Serializer animationWriter;
//    animation->Serialize(animationWriter);
//    animationWriter.WriteFile("AnimationTest.json");

    /////////////////////Animation Deserialize test///////////////////
    //Serializer animationReader;
    //animationReader.ReadFile("AnimationTest.json");
    //animation->Deserialize(animationReader);

    //testMesh->AddAnimation(animation);

    model->AnimationActive = true;
    model->SetAnimation("AnimationTest.json");
    model->FrameRate = 3;
    model->Looping = true;
    model->FlipY = true;
    ForLease->Resources.GetMeshAnimation("AnimationTest.json");
    //std::cout << ForLease->Resources.GetLoadedMeshAnimationNames()[0] << std::endl;

    //    testMesh->SetVertex(Point(-1.0f / 11, -5.5f / 11), 12);
//    testMesh->SetVertex(Point( 1.0f / 11, -5.5f / 11), 13);
//    testMesh->SetVertex(Point(-1.0f / 11, -4.5f / 11), 14);
    ////////////////Model Serialize Test//////////////////////////////////
//    Serializer modelWriter;
//    model->Serialize(modelWriter);
//    modelWriter.WriteFile("Model.json");

    ///////////////Model Deserialize Test/////////////////////////////////
    //Serializer modelReader;
    //modelReader.ReadFile("Model.json");
    //model->Deserialize(modelReader);

    //spriteTest->AddComponent(model);
//    std::ifstream fontFile("Arial.fnt", std::ios_base::in | std::ios_base::binary);
//    BmFont font;
//    fontFile >> font;
//    arial32 = new Font(font);
    //for(int i = 0; i < 2000; ++i) {
        ForLease->Resources.LoadFont("Arial.fnt");
    //}
    Components::SpriteText* text = new Components::SpriteText(*spriteTest, "");
    ///////////////////SpriteText Serialize Test////////////////////////////
//    Serializer spriteTextWriter;
//    text->Serialize(spriteTextWriter);
//    spriteTextWriter.WriteFile("SpriteText.json");

    //////////////////SpriteText Deserialize Test///////////////////////////
    Serializer spriteTextReader;
    spriteTextReader.ReadFile("SpriteText.json");
    text->Deserialize(spriteTextReader);

    //////////////////Virtual serialize test/////////////////////////////
//    Serializer vSerializer;
//    Component* cp = text;
//    cp->Serialize(vSerializer);
//    vSerializer.WriteFile("VirtualText.json");

    Entity* entity = AddEntity();

    soundEmitter = new Components::SoundEmitter(*entity);

    //////////////////Sound Emitter serialize///////////////////////
//    Serializer soundEmitterWriter;
//    soundEmitter->Serialize(soundEmitterWriter);
//    soundEmitterWriter.WriteFile("SoundEmitter.json");

    /////////////////Sound Emitter deserialize/////////////////////
    Serializer soundEmitterReader;
    soundEmitterReader.ReadFile("SoundEmitter.json");
    soundEmitter->Deserialize(soundEmitterReader);


    mouseDrag = Components::DragWithMouse::Create(*entity);
    //mouseDrag->Active = true;

    //////////////Mouse Drag serialize///////////////////
    //Serializer mouseDragWriter;
    //mouseDrag->Serialize(mouseDragWriter);
    //mouseDragWriter.WriteFile("MouseDragTest.json");

    /////////////Mouse Drag deserialize///////////////////
    Serializer mouseDragReader;
    mouseDragReader.ReadFile("MouseDragTest.json");
    mouseDrag->Deserialize(mouseDragReader);
    mouseDrag->Active = false;

    transCont = new Components::TransformModeControls(*entity);
    transCont->SlowMotionSpeed = 0.1f;
    transCont->Initialize();

    keyScale = ScaleWithKeyboard::Create(*entity);
    //keyScale->Active = true;

        //entity->AddComponent(new Components::Transform(*entity, 200 * cos(2 * 3.14 * i / 500), 200 * sin(2 * 3.14 * i / 500), 500, 500, i));
        entity->AddComponent(new Components::Transform(*entity));
        entity->AddComponent(model);
        //Components::Sprite* sprite = new Components::Sprite(*entity);
        //sprite->SpriteSource.push_back(TextureRegion(ForLease->Resources.GetTexture("bg11.jpg"), 0, 512, 0, 512));
        //entity->AddComponent(sprite);
        entity->AddComponent(soundEmitter);
        entity->AddComponent(new Components::Collision(*entity));
        entity->AddComponent(mouseDrag);
        entity->AddComponent(keyScale);
        visionCone = new Components::VisionCone(*entity, true, true, true, Vector(-0.2, 0.35), 4, Vector(-1, 0), PI / 4, Color(0.5f, 0, 0.5f, 1));
        entity->AddComponent(visionCone);

    Serializer mouseDragWriter;
    mouseDrag->Serialize(mouseDragWriter);
    mouseDragWriter.WriteFile("MouseDragTest.json");
    Serializer keyScaleWriter;
    keyScale->Serialize(keyScaleWriter);
    keyScaleWriter.WriteFile("KeyScaleTest.json");
    Serializer transContWriter;
    transCont->Serialize(transContWriter);
    transContWriter.WriteFile("TransContTest.json");

    Entity* anotherEntity = AddEntity("AnotherEntity");
    anotherEntity->AddComponent(new Components::Transform(*anotherEntity, -1, 1));
    anotherEntity->AddComponent(new Components::Model(*anotherEntity, true, false, false, "MainCharMesh.json"));
    anotherEntity->AddComponent(new Components::Collision(*anotherEntity, 0.5, 1));
    anotherEntity->AddComponent(Components::DragWithMouse::Create(*anotherEntity));
    anotherEntity->AddComponent(Components::ScaleWithKeyboard::Create(*anotherEntity));

    Entity* oneMoreEntity = AddEntity("OneMoreEntity");
    oneMoreEntity->AddComponent(new Components::Transform(*oneMoreEntity, -2, -3));
    oneMoreEntity->AddComponent(new Components::Model(*oneMoreEntity, true, false, false, "Building1Mesh.json"));
    oneMoreEntity->AddComponent(new Components::Collision(*oneMoreEntity, 1.2, 2));
    oneMoreEntity->AddComponent(Components::DragWithMouse::Create(*oneMoreEntity));
    oneMoreEntity->AddComponent(Components::ScaleWithKeyboard::Create(*oneMoreEntity));

    camera = AddEntity();
    camera->AddComponent(new Components::Transform(*camera, 0, 0, 1, 1, 0));
    camera->AddComponent(new Components::Camera(*camera, 0, 1, 8));

    render->SetCamera(camera->GetID());
    render->SetClearColor(0.3f, 0.3f, 0.5f, 1);

    spriteTest = AddEntity();
    spriteTest->AddComponent(transform);
    spriteTest->AddComponent(text);
    /////////////////////Entity Serialize Test///////////////////////
    Serializer entityWriter;
    spriteTest->Serialize(entityWriter);
    entityWriter.WriteFile("Entity.json");

    ///////////////////Renderer Serialize Test///////////////////////
//    Serializer rendererWriter;
//    render->Serialize(rendererWriter);
//    rendererWriter.WriteFile("RendererTest.json");

    ///////////////////Renderer Deserialize Test//////////////////////
    Serializer rendererReader;
    rendererReader.ReadFile("RendererTest.json");
    render->Deserialize(rendererReader);

    render->SetClearColor(0.3f, 0.3f, 0.5f, 1);

    render->SetCamera(camera->GetID());
    LevelComponents.push_back(render);
    LevelComponents.push_back(new LevelComponents::Collision(*this));

    Serializer test;
    model->Serialize(test);
    test.WriteFile("Model.json");
}

void TestingState::Initialize() {
    //render.SetProjection(Point(0, 0), 720, 720, 0, 1, 0);
    fpsPrinter.Initialize();
    Serializer stringTest;
    stringTest.WriteString("Test", "This is a newline test:\nDidja get the newline?");
    stringTest.WriteFile("NewlineTest.json");
    /*std::cout << "Entity IDs: ";
    for(unsigned int i = 0; i < Entities.size(); ++i) {
        std::cout << Entities[i]->GetID() << ", ";
    }
    std::cout << std::endl;

    std::cout << "Loaded Textures:" << std::endl;
    std::vector<std::string> textures = ForLease->Resources.GetLoadedTextureNames();
    for(int i = 0; i < textures.size(); ++i) {
        std::cout << textures[i] << std::endl;
    }

    std::cout << "Loaded Fonts:" << std::endl;
    std::vector<std::string> fonts = ForLease->Resources.GetLoadedFontNames();
    for(int i = 0; i < fonts.size(); ++i) {
        std::cout << fonts[i] << std::endl;
    }

    std::cout << "Loaded Meshes:" << std::endl;
    std::vector<std::string> meshes = ForLease->Resources.GetLoadedMeshNames();
    for(int i = 0; i < meshes.size(); ++i) {
        std::cout << meshes[i] << std::endl;
    }*/
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
    //Components::Sprite* sprite = reinterpret_cast<Components::Sprite*>(spriteTest->GetComponent(ComponentType::Sprite));
    //Components::Transform* transform = reinterpret_cast<Components::Transform*>(spriteTest->GetComponent(ComponentType::Transform));
    for(unsigned int i = 0; i < Entities.size(); ++i) {
        Components::Sprite* sprite = Entities[i]->GetComponent<Components::Sprite>();
        if(sprite)
            sprite->Update();
        Components::ScaleWithKeyboard* scale = Entities[i]->GetComponent<Components::ScaleWithKeyboard>();
        if(scale)
            scale->Update();
    }
    model->Update();
    if(mouseDrag)
        mouseDrag->Update();

    render->Update(Entities);
    render->SetDrawingColor(Color(1, 1, 1, 1));
    render->SetDebugPointSize(4);

    float animationTime = model->GetFrameTime();
    float frameRate = model->FrameRate;
    unsigned int currentFrame = model->GetFrame();
    float t = animationTime * frameRate - currentFrame;
    for(unsigned int i = 0; i < animation->GetFrameVertexCount(); ++i) {
        render->DrawPoint(animation->InterpolateVertex(currentFrame, i, t));
    }
    transCont->Update();
    visionCone->Update();
    //Timer t;
    ForLease->GameWindow->UpdateGameWindow();
    //std::cout << t.GetTime() << std::endl;
}

void TestingState::Deinitialize() {
}

void TestingState::Unload() {
}


