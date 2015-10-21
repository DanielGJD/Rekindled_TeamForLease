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
#include "FrameRatePrinter.h"
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
bool addObject = false;

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
        addObject = true;
    }

    void OnMouseUp(const ForLeaseEngine::Event* e) {
        const ForLeaseEngine::MouseButtonEvent* mouse_e = reinterpret_cast<const ForLeaseEngine::MouseButtonEvent*>(e);
        std::cout << mouse_e->EventName << ":" << std::endl
                  << mouse_e->Button << std::endl
                  << mouse_e->Clicks << std::endl
                  << "[" << mouse_e->ScreenLocation[0] << "," << mouse_e->ScreenLocation[1] << "]" << std::endl << std::endl;
    }
};

DTPrinter printer;
MouseListener mouse;
LevelComponents::Renderer* render;
Mesh* testMesh;
Entity* spriteTest;
Texture* testTexture;
Entity* camera;
Font* arial32;
int numToDraw = 1;
FrameRatePrinter fpsPrinter;

void DTPrinter::PrintDt(const ForLeaseEngine::Event* e) {
    const ForLeaseEngine::UpdateEvent* updateEvent = reinterpret_cast<const ForLeaseEngine::UpdateEvent*>(e);
    std::cout << "FPS: " << static_cast<int>(1 / updateEvent->dt) << std::endl;
}

void TestingState::Load() {
    render = new LevelComponents::Renderer(*this);

    ForLease->Dispatcher.Attach(NULL, &printer, "UpdateEvent", &DTPrinter::PrintDt);
    ForLease->Dispatcher.Attach(NULL, &mouse, "MouseButtonDown", &MouseListener::OnMouseDown);
    ForLease->Dispatcher.Attach(NULL, &mouse, "MouseButtonUp", &MouseListener::OnMouseUp);
//    testMesh = new Mesh(15, 17, 7);
//
//    testMesh->SetVertex(Point(-3.0f / 11,  0.5f / 11),  0);
//    testMesh->SetVertex(Point( 3.0f / 11,  0.5f / 11),  1);
//    testMesh->SetVertex(Point( 3.0f / 11,  5.5f / 11),  2);
//    testMesh->SetVertex(Point(-3.0f / 11,  5.5f / 11),  3);
//    testMesh->SetVertex(Point( 3.0f / 11,  2.5f / 11),  4);
//    testMesh->SetVertex(Point( 4.0f / 11,  2.5f / 11),  5);
//    testMesh->SetVertex(Point( 3.0f / 11,  3.5f / 11),  6);
//    testMesh->SetVertex(Point(-3.0f / 11, -4.5f / 11),  7);
//    testMesh->SetVertex(Point( 3.0f / 11, -4.5f / 11),  8);
//    testMesh->SetVertex(Point(-1.0f / 11, -17.0f / 11 / 6),  9);
//    testMesh->SetVertex(Point( 1.0f / 11, -17.0f / 11 / 6), 10);
//    testMesh->SetVertex(Point( 1.0f / 11, -7.0f / 11 / 6), 11);
//    testMesh->SetVertex(Point(-1.0f / 11, -5.5f / 11), 12);
//    testMesh->SetVertex(Point( 1.0f / 11, -5.5f / 11), 13);
//    testMesh->SetVertex(Point(-1.0f / 11, -4.5f / 11), 14);
//
//
//    testMesh->SetEdge(IndexedEdge(0, 1), 0);
//    testMesh->SetEdge(IndexedEdge(1, 2), 1);
//    testMesh->SetEdge(IndexedEdge(2, 0), 2);
//    testMesh->SetEdge(IndexedEdge(2, 3), 3);
//    testMesh->SetEdge(IndexedEdge(3, 0), 4);
//    testMesh->SetEdge(IndexedEdge(4, 5), 5);
//    testMesh->SetEdge(IndexedEdge(5, 6), 6);
//    testMesh->SetEdge(IndexedEdge(0, 7), 7);
//    testMesh->SetEdge(IndexedEdge(7, 8), 8);
//    testMesh->SetEdge(IndexedEdge(8, 1), 9);
//    testMesh->SetEdge(IndexedEdge(7, 1), 10);
//    testMesh->SetEdge(IndexedEdge(9, 10), 11);
//    testMesh->SetEdge(IndexedEdge(10, 11), 12);
//    testMesh->SetEdge(IndexedEdge(11, 9), 13);
//    testMesh->SetEdge(IndexedEdge(12, 13), 14);
//    testMesh->SetEdge(IndexedEdge(13, 14), 15);
//    testMesh->SetEdge(IndexedEdge(14, 12), 16);
//
//    testMesh->SetFace(IndexedFace(0, 1, 2), 0);
//    testMesh->SetFace(IndexedFace(0, 2, 3), 1);
//    testMesh->SetFace(IndexedFace(4, 5, 6), 2);
//    testMesh->SetFace(IndexedFace(7, 1, 0), 3);
//    testMesh->SetFace(IndexedFace(7, 8, 1), 4);
//    testMesh->SetFace(IndexedFace(9, 10, 11), 5);
//    testMesh->SetFace(IndexedFace(12, 13, 14), 6);
//
//    Color clothes = Color( 52.0f / 255,  77.0f / 255, 103.0f / 255);
//    Color skin    = Color(218.0f / 255, 200.0f / 255, 183.0f / 255);
//
//    testMesh->SetFaceColor(skin, 0);
//    testMesh->SetFaceColor(clothes, 1);
//    testMesh->SetFaceColor(skin, 2);
//    testMesh->SetFaceColor(clothes, 3);
//    testMesh->SetFaceColor(clothes, 4);
//    testMesh->SetFaceColor(skin, 5);
//    testMesh->SetFaceColor(clothes, 6);

    ////////////////Serialize test for mesh//////////////////
//    Serializer serializer;
//    testMesh->Serialize(serializer);
//    serializer.WriteFile("MeshTest.json");

    ///////////////Deserialize test for mesh//////////////////
//    Serializer serializer;
//    serializer.ReadFile("MeshTest.json");
//    testMesh = new Mesh();
//    testMesh->Deserialize(serializer);

    //for(int i = 0; i < 5000; ++i) {
        ForLease->Resources.LoadMesh("MeshTest.json");
    //}
    testMesh = ForLease->Resources.GetMesh("MeshTest.json");
    //testMesh->ClearData();

    /*Serializer s2;
    testMesh->Serialize(s2);
    s2.WriteFile("MeshTest2.json");*/

    //testTexture = Texture::CreateTexture("BlobCharacter.png");
    ForLease->Resources.LoadTexture("BlobCharacter.png");
//    testTexture = ForLease->Resources.GetTexture("BlobCharacter.png");
//    TextureRegion frame1 = TextureRegion(testTexture, 0, 32, 0, 32);
//    TextureRegion frame2 = TextureRegion(testTexture, 32, 64, 0, 32);
//    TextureRegion frame3 = TextureRegion(testTexture, 64, 96, 0, 32);
//    TextureRegion frame4 = TextureRegion(testTexture, 96, 128, 0, 32);
//    TextureRegion frame5 = TextureRegion(testTexture, 128, 160, 0, 32);
//    TextureRegion frame6 = TextureRegion(testTexture, 160, 192, 0, 32);
//    TextureRegion frame7 = TextureRegion(testTexture, 192, 224, 0, 32);
//    TextureRegion frame8 = TextureRegion(testTexture, 224, 256, 0, 32);

    /////////////////Texture Region Serialize Test////////////////////////
//    Serializer root;
//    frame1.Serialize(root);
//    root.WriteFile("TextureRegion.json");

    ////////////////Texture Region Deserialize Test////////////////////////
    /*Serializer root;
    root.ReadFile("TextureRegion.json");
    frame1.Deserialize(root);*/



    Components::Transform* transform = new Components::Transform(*spriteTest, 0, 0, 100, 100, 0);
    Components::Sprite* sprite = new Components::Sprite(*spriteTest);
//    sprite->SpriteColor = Color(0, 1, 0);
//    sprite->AnimationActive = true;
//    sprite->SpriteSource.push_back(frame1);
//    sprite->SpriteSource.push_back(frame2);
//    sprite->SpriteSource.push_back(frame3);
//    sprite->SpriteSource.push_back(frame4);
//    sprite->SpriteSource.push_back(frame5);
//    sprite->SpriteSource.push_back(frame6);
//    sprite->SpriteSource.push_back(frame7);
//    sprite->SpriteSource.push_back(frame8);
//    sprite->FrameRate = 12;

    /////////////////////Sprite Serialize Test///////////////////////
//    Serializer root;
//    sprite->Serialize(root);
//    root.WriteFile("Sprite.json");

    /////////////////////Sprite Deserialize Test/////////////////////////
    Serializer root;
    root.ReadFile("Sprite.json");
    sprite->Deserialize(root);
    //std::cout << *sprite << std::endl;

    //spriteTest->AddComponent(sprite);
    Components::Model* model = new Components::Model(*spriteTest, true, "MeshTest.json", "", Color(1, 1, 1, 1));
    ////////////////Model Serialize Test//////////////////////////////////
//    Serializer modelWriter;
//    model->Serialize(modelWriter);
//    modelWriter.WriteFile("Model.json");

    ///////////////Model Deserialize Test/////////////////////////////////
    Serializer modelReader;
    modelReader.ReadFile("Model.json");
    model->Deserialize(modelReader);


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

    for(int i = 0; i < 1; ++i) {
        Entity* entity = AddEntity();
        entity->AddComponent(new Components::Transform(*entity, 200 * cos(2 * 3.14 * i / 500), 200 * sin(2 * 3.14 * i / 500), 500, 500, i));
        entity->AddComponent(model);
    }


    camera = new Entity();
    camera->AddComponent(new Components::Transform(*camera, 0, 0, 1, 1, 0));
    camera->AddComponent(new Components::Camera(*camera, 0, 1, 720));

    render->SetCamera(&camera);

    spriteTest = AddEntity();
    spriteTest->AddComponent(transform);
    spriteTest->AddComponent(text);
    /////////////////////Entity Serialize Test///////////////////////
    Serializer entityWriter;
    spriteTest->Serialize(entityWriter);
    entityWriter.WriteFile("Entity.json");
}

void TestingState::Initialize() {
    //render.SetProjection(Point(0, 0), 720, 720, 0, 1, 0);
    fpsPrinter.Initialize();
}

void TestingState::Update() {
    //double dt = ForLease->FrameRateController().GetDt();
    //std::cout << static_cast<int>(1 / dt) << std::endl;
    //ForLeaseEngine::UpdateEvent e = ForLeaseEngine::UpdateEvent(dt);
    //ForLease->Dispatcher.Dispatch(&e, NULL);
    //ForLease->Dispatcher.DispatchTo(&e, &printer);
    ForLease->OSInput.ProcessAllInput();
    //Components::Sprite* sprite = reinterpret_cast<Components::Sprite*>(spriteTest->GetComponent(ComponentType::Sprite));
    //Components::Transform* transform = reinterpret_cast<Components::Transform*>(spriteTest->GetComponent(ComponentType::Transform));
    for(unsigned int i = 0; i < Entities.size(); ++i) {
        Components::Sprite* sprite = Entities[i]->GetComponent<Components::Sprite*>(ComponentType::Sprite);
        if(sprite)
            sprite->Update();
    }
    //ForLease->Resources.LoadTexture("BlobCharacter.png");
    Components::SpriteText* spriteText = spriteTest->GetComponent<Components::SpriteText*>(ComponentType::SpriteText);
    std::stringstream ss;
    ss << "FPS: " << static_cast<int>(1 / ForLease->FrameRateController().GetDt()) << std::endl
       << "Tris: " << render->GetTriCount() << std::endl
       << "Lines: " << render->GetLineCount() << std::endl
       << "Blend Mode Swaps: " << render->GetBlendModeSwapCount() << std::endl
       << "Texture Swaps: " << render->GetTextureSwapCount() << std::endl
       << "Render Time: " << render->GetRenderTime() << std::endl;
    //ss << ForLease->Resources;

    spriteText->Text = ss.str();
    //Components::Model* model = reinterpret_cast<Components::Model*>(spriteTest->GetComponent(ComponentType::Model));
    //sprite->Update();
    //render->SetDrawingColor(0.5, 0.5, 0.5);
    //render->DrawRectangleFilled(Point(0, 0), 720, 720);

//    for(int i = 0; i < model->ModelMesh->GetFaceCount(); ++i) {
//        Face face = model->ModelMesh->GetFace(i);
//        Color color = model->ModelMesh->GetFaceColor(i);
//        render.SetDrawingColor(color.GetR(), color.GetG(), color.GetB());
//        glBegin(GL_TRIANGLES);
//            for(int j = 0; j < 3; ++j) {
//                glVertex2f(face.Vertices[j][0], face.Vertices[j][1]);
//            }
//        glEnd();
//    }
    //Point o;
    //glLineWidth(4.0f);
    //glColor3f(0, 0, 0);
    //render.DrawWireframeMesh(*model->ModelMesh, transform->Position, 1, 1, transform->Rotation);
    //render.SetDrawingColor(1, 0, 0, 0.5f);
    //render.SetBlendMode(BlendMode::NONE);
    //render.DrawTextureRegion(transform->Position + Vector(-80, 0), sprite->GetCurrentRegion(), transform->ScaleX, transform->ScaleY, transform->Rotation);
    //render->DrawTextureRegion(transform->Position + Vector(80, 0), sprite->GetCurrentRegion(), transform->ScaleX, transform->ScaleY, transform->Rotation, numToDraw);
    //render->SetModelView(transform);
    //render->DrawModel(model);
    //render->DrawSprite(sprite);
    //for(int i = 0; i < 500; ++i) {
        //render.DrawTexture(Point(0, 0), testTexture);
    //}

    if(addObject) {
        Entity* entity = AddEntity();
        entity->AddComponent(new Components::Transform(*entity, mousePos[0], mousePos[1], 100, 100, 0));
        Components::Model* model = new Components::Model(*entity, true, "MeshTest.json", "", Color(), BlendMode::NONE);
        entity->AddComponent(model);
        addObject = false;
    }

    render->Update(Entities);

    //ForLease->Resources.LoadMesh("MeshTest.json");
    //std::cout << ForLease->Resources << std::endl;
    Timer t;
    ForLease->GameWindow->UpdateGameWindow();
    //std::cout << t.GetTime() << std::endl;
}

void TestingState::Deinitialize() {
}

void TestingState::Unload() {
}


