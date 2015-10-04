#include "EngineProofState.h"
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
#include "SpriteText.h"
#include "BmFont.h"
#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <iomanip>

/*class EngineProofState : public ForLeaseEngine::State {
    public:
        EngineProofState(Engine* engine);
        void Load();
        void Initialize();
        void Update();
        void Deinitialize();
        void Unload();
    private:
        ForLeaseEngine::Renderer Render;
        Engine* MainEngine;
        TestObject* Object;
};

class TestObject {
    public:
        Mesh Model;
        Point Translation;
        float Rotation;
        float ScaleX;
        float ScaleY;
        Vector Speed;

        TestObject();

};*/

bool DEBUG = false;
float timer = 0;
ForLeaseEngine::Texture* Background;
ForLeaseEngine::Texture* FullTextureDemo;
ForLeaseEngine::Texture* TransparencyDemo;
ForLeaseEngine::TextureRegion TextureRegionDemo;
ForLeaseEngine::Mesh* ModelDemo;
ForLeaseEngine::SpriteText* TextDemo;
ForLeaseEngine::Font* FontDemo;
ForLeaseEngine::Font* FontDemo2;
ForLeaseEngine::SpriteText* FPSCounter;


TestObject::TestObject() : Model(5, 6, 2) {
    Model.SetCenter(ForLeaseEngine::Point(0, 0));

    Model.SetVertex(ForLeaseEngine::Point(0, 0), 0);
    Model.SetVertex(ForLeaseEngine::Point(1, -1), 1);
    Model.SetVertex(ForLeaseEngine::Point(1, 1), 2);
    Model.SetVertex(ForLeaseEngine::Point(-1, 1), 3);
    Model.SetVertex(ForLeaseEngine::Point(-1, -1), 4);

    Model.SetFace(ForLeaseEngine::IndexedFace(0, 1, 2), 0);
    Model.SetFace(ForLeaseEngine::IndexedFace(0, 3, 4), 1);

    Model.SetEdge(ForLeaseEngine::IndexedEdge(0, 1), 0);
    Model.SetEdge(ForLeaseEngine::IndexedEdge(1, 2), 1);
    Model.SetEdge(ForLeaseEngine::IndexedEdge(2, 0), 2);
    Model.SetEdge(ForLeaseEngine::IndexedEdge(0, 3), 3);
    Model.SetEdge(ForLeaseEngine::IndexedEdge(3, 4), 4);
    Model.SetEdge(ForLeaseEngine::IndexedEdge(4, 0), 5);

    ForLeaseEngine::AddComponentsToEntity(ForLeaseEngine::ComponentType::Transform
        | ForLeaseEngine::ComponentType::Physics | ForLeaseEngine::ComponentType::Collision, this);

    ForLeaseEngine::Components::Collision * collision = reinterpret_cast<ForLeaseEngine::Components::Collision *>(GetComponent(ForLeaseEngine::ComponentType::Collision));
    collision->ResolveCollisions = true;
    collision->Width = 20;
    collision->Height = 20;
}

void TestObject::OnKeyDown(const ForLeaseEngine::Event* e){
    const ForLeaseEngine::KeyboardEvent* keyboardE = reinterpret_cast<const ForLeaseEngine::KeyboardEvent*>(e);
    ForLeaseEngine::Components::Physics * ObjectPhysics = reinterpret_cast<ForLeaseEngine::Components::Physics *>(GetComponent(ForLeaseEngine::ComponentType::Physics));
    ForLeaseEngine::Components::Transform* ObjectTransform = reinterpret_cast<ForLeaseEngine::Components::Transform*>(GetComponent(ForLeaseEngine::ComponentType::Transform));

    ForLeaseEngine::Vector direction;
    if(keyboardE->Key == ForLeaseEngine::Keys::Up)
    {
        ObjectPhysics->Velocity += ForLeaseEngine::Vector(0, 40);
        ObjectTransform->Position += ForLeaseEngine::Vector(0, 5);
        //Velocity = ForLeaseEngine::Vector::Scale(Velocity, Speed);
    }
    if(keyboardE->Key == ForLeaseEngine::Keys::Down)
    {
        //ObjectPhysics->Velocity += ForLeaseEngine::Vector(0, -20);
        //Velocity = ForLeaseEngine::Vector::Scale(Velocity, Speed);
    }
    if(keyboardE->Key == ForLeaseEngine::Keys::Left)
    {
        ObjectPhysics->Velocity += ForLeaseEngine::Vector(-40, 0);
        //Velocity = ForLeaseEngine::Vector::Scale(Velocity, Speed);
    }
    if(keyboardE->Key == ForLeaseEngine::Keys::Right)
    {
        ObjectPhysics->Velocity += ForLeaseEngine::Vector(40, 0);
        //Velocity = ForLeaseEngine::Vector::Scale(Velocity, Speed);
    }
    if(keyboardE->Key == ForLeaseEngine::Keys::Q){
       ForLease->GameStateManager().SetAction(ForLeaseEngine::Modules::StateAction::Quit);
    }
    if(keyboardE->Key == ForLeaseEngine::Keys::D) {
        DEBUG = !DEBUG;
    }
}

void TestObject::OnKeyUp(const ForLeaseEngine::Event* e){
    const ForLeaseEngine::KeyboardEvent* keyboardE = reinterpret_cast<const ForLeaseEngine::KeyboardEvent*>(e);
    ForLeaseEngine::Components::Physics * ObjectPhysics = reinterpret_cast<ForLeaseEngine::Components::Physics *>(GetComponent(ForLeaseEngine::ComponentType::Physics));


    if(keyboardE->Key == ForLeaseEngine::Keys::Up)
    {
        ObjectPhysics->Velocity += ForLeaseEngine::Vector(0, -40);
        //Velocity = ForLeaseEngine::Vector::Scale(Velocity, Speed);
    }
    if(keyboardE->Key == ForLeaseEngine::Keys::Down)
    {
        //ObjectPhysics->Velocity += ForLeaseEngine::Vector(0, 20);
        //Velocity = ForLeaseEngine::Vector::Scale(Velocity, Speed);
    }
    if(keyboardE->Key == ForLeaseEngine::Keys::Left)
    {
        ObjectPhysics->Velocity += ForLeaseEngine::Vector(40, 0);
        //Velocity = ForLeaseEngine::Vector::Scale(Velocity, Speed);
    }
    if(keyboardE->Key == ForLeaseEngine::Keys::Right)
    {
        ObjectPhysics->Velocity += ForLeaseEngine::Vector(-40, 0);
        //Velocity = ForLeaseEngine::Vector::Scale(Velocity, Speed);
    }
}

TestCollision::TestCollision() : Model(5, 6, 2)
{
    Model.SetCenter(ForLeaseEngine::Point(0, 0));

    Model.SetVertex(ForLeaseEngine::Point(0, 0), 0);
    Model.SetVertex(ForLeaseEngine::Point(1, -1), 1);
    Model.SetVertex(ForLeaseEngine::Point(1, 1), 2);
    Model.SetVertex(ForLeaseEngine::Point(-1, 1), 3);
    Model.SetVertex(ForLeaseEngine::Point(-1, -1), 4);

    Model.SetFace(ForLeaseEngine::IndexedFace(0, 1, 2), 0);
    Model.SetFace(ForLeaseEngine::IndexedFace(0, 3, 4), 1);

    Model.SetEdge(ForLeaseEngine::IndexedEdge(0, 1), 0);
    Model.SetEdge(ForLeaseEngine::IndexedEdge(1, 2), 1);
    Model.SetEdge(ForLeaseEngine::IndexedEdge(2, 0), 2);
    Model.SetEdge(ForLeaseEngine::IndexedEdge(0, 3), 3);
    Model.SetEdge(ForLeaseEngine::IndexedEdge(3, 4), 4);
    Model.SetEdge(ForLeaseEngine::IndexedEdge(4, 0), 5);

    ForLeaseEngine::AddComponentsToEntity(ForLeaseEngine::ComponentType::Transform
        | ForLeaseEngine::ComponentType::Collision, this);

    ForLeaseEngine::Components::Collision * collision = reinterpret_cast<ForLeaseEngine::Components::Collision *>(GetComponent(ForLeaseEngine::ComponentType::Collision));
    collision->ResolveCollisions = false;
    collision->Width = 20;
    collision->Height = 20;
}

void TestCollision::Update()
{
    ForLeaseEngine::Components::Collision * collision = reinterpret_cast<ForLeaseEngine::Components::Collision *>(GetComponent(ForLeaseEngine::ComponentType::Collision));

    if (collision->CollidedLastFrame)
    {
      ForLease->GameStateManager().SetAction(ForLeaseEngine::Modules::StateAction::Quit);
    }
}

TestFloor::TestFloor() : Model(4, 5, 2) {
    Model.SetCenter(ForLeaseEngine::Point(0, 0));

    Model.SetVertex(ForLeaseEngine::Point(-1,1), 0);
    Model.SetVertex(ForLeaseEngine::Point(1,1), 1);
    Model.SetVertex(ForLeaseEngine::Point(1,-1), 2);
    Model.SetVertex(ForLeaseEngine::Point(-1,-1), 3);

    Model.SetEdge(ForLeaseEngine::IndexedEdge(0,1), 0);
    Model.SetEdge(ForLeaseEngine::IndexedEdge(1,2), 1);
    Model.SetEdge(ForLeaseEngine::IndexedEdge(2,3), 2);
    Model.SetEdge(ForLeaseEngine::IndexedEdge(3,0), 3);
    Model.SetEdge(ForLeaseEngine::IndexedEdge(0,2), 4);

    Model.SetFace(ForLeaseEngine::IndexedFace(0,1,2), 0);
    Model.SetFace(ForLeaseEngine::IndexedFace(0,2,3), 1);

    ForLeaseEngine::AddComponentsToEntity(ForLeaseEngine::ComponentType::Transform
        | ForLeaseEngine::ComponentType::Collision, this);

    ForLeaseEngine::Components::Collision * collision = reinterpret_cast<ForLeaseEngine::Components::Collision *>(GetComponent(ForLeaseEngine::ComponentType::Collision));
    collision->ResolveCollisions = true;
    collision->Width = 200;
    collision->Height = 20;
}

EngineProofState::EngineProofState() : Physics(*ForLease, ForLeaseEngine::Vector(0,-10)),
    Collision(*ForLease) {
    //MainEngine = ForLease;
}

void EngineProofState::Load() {
    Object = new TestObject();
    Entities.push_back(Object);
    Floor = new TestFloor();
    Entities.push_back(Floor);
    Collide = new TestCollision();
    Entities.push_back(Collide);

    TransparencyDemo = ForLeaseEngine::Texture::CreateTexture("MIDSPACE.png");
    FullTextureDemo = ForLeaseEngine::Texture::CreateTexture("STONE2.png");
    TextureRegionDemo = ForLeaseEngine::TextureRegion(FullTextureDemo, 64, 576 , 384, 640);

    std::ifstream fontFile("Arial.fnt", std::ios_base::in | std::ios_base::binary);
    ForLeaseEngine::BmFont bmFont;
    fontFile >> bmFont;
    FontDemo = new ForLeaseEngine::Font(bmFont);
//    fontFile.open("ComicSans.fnt", std::ios_base::in | std::ios_base::binary);
//    ForLeaseEngine::BmFont bmFont2;
//    try{
//        fontFile >> bmFont2;
//    } catch(std::exception e) {
//        std::cout << e.what() << std::endl;
//    }
//    FontDemo2 = new ForLeaseEngine::Font(bmFont2);
    TextDemo = new ForLeaseEngine::SpriteText(FontDemo, "It does text too!\nMulti-line in fact!");
    FPSCounter = new ForLeaseEngine::SpriteText(FontDemo);

    ModelDemo = new ForLeaseEngine::Mesh(5, 6, 2);
    ModelDemo->SetCenter(ForLeaseEngine::Point(0, 0));

    ModelDemo->SetVertex(ForLeaseEngine::Point(0, 0), 0);
    ModelDemo->SetVertex(ForLeaseEngine::Point(1, -1), 1);
    ModelDemo->SetVertex(ForLeaseEngine::Point(1, 1), 2);
    ModelDemo->SetVertex(ForLeaseEngine::Point(-1, 1), 3);
    ModelDemo->SetVertex(ForLeaseEngine::Point(-1, -1), 4);

    ModelDemo->SetFace(ForLeaseEngine::IndexedFace(0, 1, 2), 0);
    ModelDemo->SetFace(ForLeaseEngine::IndexedFace(0, 3, 4), 1);

    ModelDemo->SetEdge(ForLeaseEngine::IndexedEdge(0, 1), 0);
    ModelDemo->SetEdge(ForLeaseEngine::IndexedEdge(1, 2), 1);
    ModelDemo->SetEdge(ForLeaseEngine::IndexedEdge(2, 0), 2);
    ModelDemo->SetEdge(ForLeaseEngine::IndexedEdge(0, 3), 3);
    ModelDemo->SetEdge(ForLeaseEngine::IndexedEdge(3, 4), 4);
    ModelDemo->SetEdge(ForLeaseEngine::IndexedEdge(4, 0), 5);
}

void EngineProofState::Initialize() {
    ForLeaseEngine::Components::Transform * ObjectTransform = reinterpret_cast<ForLeaseEngine::Components::Transform *>(Object->GetComponent(ForLeaseEngine::ComponentType::Transform));
    ObjectTransform->Position = ForLeaseEngine::Point(-200, 0);
    ObjectTransform->Rotation = 0;
    ObjectTransform->ScaleX = 10;
    ObjectTransform->ScaleY = 10;

    ForLeaseEngine::Components::Transform * FloorTransform = reinterpret_cast<ForLeaseEngine::Components::Transform *>(Floor->GetComponent(ForLeaseEngine::ComponentType::Transform));
    FloorTransform->Position = ForLeaseEngine::Point(-200,-30);
    FloorTransform->Rotation = 0;
    FloorTransform->ScaleX = 100;
    FloorTransform->ScaleY = 10;

    ForLeaseEngine::Components::Transform * CollideTransform = reinterpret_cast<ForLeaseEngine::Components::Transform *>(Collide->GetComponent(ForLeaseEngine::ComponentType::Transform));
    CollideTransform->Position = ForLeaseEngine::Point(-100, 20);
    CollideTransform->Rotation = 0;
    CollideTransform->ScaleX = 10;
    CollideTransform->ScaleY = 10;

    ForLeaseEngine::Components::Physics * ObjectPhysics = reinterpret_cast<ForLeaseEngine::Components::Physics *>(Object->GetComponent(ForLeaseEngine::ComponentType::Physics));
    ObjectPhysics->Velocity = ForLeaseEngine::Vector(0, 0);
    Object->Speed = 100;
    ForLease->Dispatcher.Attach(&ForLease->OSInput, Object, "KeyDown", &TestObject::OnKeyDown);
    ForLease->Dispatcher.Attach(&ForLease->OSInput, Object, "KeyUp", &TestObject::OnKeyUp);


}

void EngineProofState::Update() {
    double dt = ForLease->FrameRateController().GetDt();
    std::ostringstream sstream;
    sstream << std::setprecision(2) << "FPS: " << 1 / dt;
    FPSCounter->SetText(sstream.str());
    timer += dt;
    // std::cout << dt << std::endl;
    Physics.Update(Entities);
    ForLeaseEngine::Components::Transform * ObjectTransform = reinterpret_cast<ForLeaseEngine::Components::Transform *>(Object->GetComponent(ForLeaseEngine::ComponentType::Transform));
    ForLeaseEngine::Components::Physics * ObjectPhysics = reinterpret_cast<ForLeaseEngine::Components::Physics *>(Object->GetComponent(ForLeaseEngine::ComponentType::Physics));
    ForLeaseEngine::Components::Collision* ObjectCollider = reinterpret_cast<ForLeaseEngine::Components::Collision*>(Object->GetComponent(ForLeaseEngine::ComponentType::Collision));
    ForLeaseEngine::Components::Transform * FloorTransform = reinterpret_cast<ForLeaseEngine::Components::Transform *>(Floor->GetComponent(ForLeaseEngine::ComponentType::Transform));
    ForLeaseEngine::Components::Collision* FloorCollider = reinterpret_cast<ForLeaseEngine::Components::Collision*>(Floor->GetComponent(ForLeaseEngine::ComponentType::Collision));
    ForLeaseEngine::Components::Transform * CollideTransform = reinterpret_cast<ForLeaseEngine::Components::Transform *>(Collide->GetComponent(ForLeaseEngine::ComponentType::Transform));
    ForLeaseEngine::Components::Collision* CollideCollider = reinterpret_cast<ForLeaseEngine::Components::Collision*>(Collide->GetComponent(ForLeaseEngine::ComponentType::Collision));
    std::cout << ObjectTransform->Position[0] << "," << ObjectTransform->Position[1] << std::endl;
    ObjectTransform->ScaleX = 10 + 0.5 * sin(2 * timer);
    ObjectTransform->ScaleY = 10 + 0.5 * cos(2 * timer);
    ObjectCollider->Width = 2 * ObjectTransform->ScaleX;
    ObjectCollider->Height = 2 * ObjectTransform->ScaleY;
    // ObjectTransform->Position = ObjectTransform->Position + ForLeaseEngine::Vector::Scale(ForLeaseEngine::Vector::Scale(ObjectPhysics->Velocity, dt), Object->Speed);
    ForLease->OSInput.ProcessAllInput();
    Collision.Update(Entities);
    Collide->Update();

    render.SetProjection(ForLeaseEngine::Point(-200, 0), 640, 360, 0, 100, 0);

    // Draw main models
    render.SetDrawingColor(0.0f, 0.0f, 0.5f);
    ForLeaseEngine::Point p(-200, 0);
    render.DrawMesh(*ModelDemo, p, 400, 50, timer);
    render.DrawMesh(*ModelDemo, p, 400, 50, 3.1415927 / 2 + timer);
    render.SetDrawingColor(0.5f, 0.0f, 0.0f);
    render.DrawMesh(*ModelDemo, p, 400, 50, 3.1415927 / 4 + timer);
    render.DrawMesh(*ModelDemo, p, 400, 50, 3.1415927 / 2 + 3.1415927 / 4 + timer);
    render.SetDrawingColor(0, 1, 0);
    render.DrawMesh(Object->Model, ObjectTransform->Position, ObjectTransform->ScaleX, ObjectTransform->ScaleY, ObjectTransform->Rotation);
    render.DrawMesh(Floor->Model, FloorTransform->Position, FloorTransform->ScaleX, FloorTransform->ScaleY, FloorTransform->Rotation);
    render.DrawMesh(Collide->Model, CollideTransform->Position, CollideTransform->ScaleX, CollideTransform->ScaleY, CollideTransform->Rotation);

    // Draw texture
    render.SetDrawingColor(1.0f, 1.0f, 1.0f);
    render.DrawTexture(ForLeaseEngine::Point(-300, -100), TransparencyDemo, 0.1f, 0.1f, 0);
    render.DrawTexture(ForLeaseEngine::Point(-200, -100), FullTextureDemo, 0.1f, 0.1f, 0);
    render.SetDrawingColor(sin(timer), cos(timer), cos(sin(timer)));
    render.DrawTextureRegion(ForLeaseEngine::Point(-100, -100), &TextureRegionDemo, (1.5 + sin(timer)) / 10, (1.5 + cos(timer)) / 10, timer);


    // If rendering debug
    if(DEBUG) {
        render.SetDrawingColor(1, 0, 0);
        render.DrawArrow(ObjectTransform->Position, ObjectPhysics->Velocity);
        render.DrawRectangle(FloorTransform->Position, FloorCollider->Width, FloorCollider->Height, 0);
        render.DrawRectangle(ObjectTransform->Position, ObjectCollider->Width, ObjectCollider->Height, 0);
        render.DrawRectangle(CollideTransform->Position, CollideCollider->Width, CollideCollider->Height, 0);
        render.SetDrawingColor(1, 1, 1);
        render.DrawSpriteText(*FPSCounter, ForLeaseEngine::Point(-500, 200));
    }

    render.SetDrawingColor(1.0f, 1.0f, 1.0f);
    render.DrawSpriteText(*TextDemo, ForLeaseEngine::Point(-100, 200));

    ForLease->GameWindow->UpdateGameWindow();
}

void EngineProofState::Deinitialize() {
}

void EngineProofState::Unload() {
    delete Object;
    delete ModelDemo;
    delete TextDemo;
    delete FontDemo;
    ForLeaseEngine::Texture::DeleteTexture(FullTextureDemo);
    ForLeaseEngine::Texture::DeleteTexture(TransparencyDemo);
}
