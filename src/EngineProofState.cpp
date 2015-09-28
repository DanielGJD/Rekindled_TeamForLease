#include "EngineProofState.h"
#include "Face.h"
#include "Edge.h"
#include "Vector.h"
#include "KeyboardEvent.h"
#include "Event.h"
#include "Keys.h"
#include "Vector.h"
#include <iostream>

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
    collision->Width = 10;
    collision->Height = 10;
}

void TestObject::OnKeyDown(const ForLeaseEngine::Event* e){
    const ForLeaseEngine::KeyboardEvent* keyboardE = reinterpret_cast<const ForLeaseEngine::KeyboardEvent*>(e);
    ForLeaseEngine::Components::Physics * ObjectPhysics = reinterpret_cast<ForLeaseEngine::Components::Physics *>(GetComponent(ForLeaseEngine::ComponentType::Physics));

    ForLeaseEngine::Vector direction;
    if(keyboardE->Key == ForLeaseEngine::Keys::Up)
    {
        ObjectPhysics->Velocity += ForLeaseEngine::Vector(0, 20);
        //Velocity = ForLeaseEngine::Vector::Scale(Velocity, Speed);
    }
    if(keyboardE->Key == ForLeaseEngine::Keys::Down)
    {
        ObjectPhysics->Velocity += ForLeaseEngine::Vector(0, -20);
        //Velocity = ForLeaseEngine::Vector::Scale(Velocity, Speed);
    }
    if(keyboardE->Key == ForLeaseEngine::Keys::Left)
    {
        ObjectPhysics->Velocity += ForLeaseEngine::Vector(-20, 0);
        //Velocity = ForLeaseEngine::Vector::Scale(Velocity, Speed);
    }
    if(keyboardE->Key == ForLeaseEngine::Keys::Right)
    {
        ObjectPhysics->Velocity += ForLeaseEngine::Vector(20, 0);
        //Velocity = ForLeaseEngine::Vector::Scale(Velocity, Speed);
    }
    if(keyboardE->Key == ForLeaseEngine::Keys::Q){
       ForLease->GameStateManager().SetAction(ForLeaseEngine::Modules::StateAction::Quit);
    }
}

void TestObject::OnKeyUp(const ForLeaseEngine::Event* e){
    const ForLeaseEngine::KeyboardEvent* keyboardE = reinterpret_cast<const ForLeaseEngine::KeyboardEvent*>(e);
    ForLeaseEngine::Components::Physics * ObjectPhysics = reinterpret_cast<ForLeaseEngine::Components::Physics *>(GetComponent(ForLeaseEngine::ComponentType::Physics));


    if(keyboardE->Key == ForLeaseEngine::Keys::Up)
    {
        ObjectPhysics->Velocity += ForLeaseEngine::Vector(0, -20);
        //Velocity = ForLeaseEngine::Vector::Scale(Velocity, Speed);
    }
    if(keyboardE->Key == ForLeaseEngine::Keys::Down)
    {
        ObjectPhysics->Velocity += ForLeaseEngine::Vector(0, 20);
        //Velocity = ForLeaseEngine::Vector::Scale(Velocity, Speed);
    }
    if(keyboardE->Key == ForLeaseEngine::Keys::Left)
    {
        ObjectPhysics->Velocity += ForLeaseEngine::Vector(20, 0);
        //Velocity = ForLeaseEngine::Vector::Scale(Velocity, Speed);
    }
    if(keyboardE->Key == ForLeaseEngine::Keys::Right)
    {
        ObjectPhysics->Velocity += ForLeaseEngine::Vector(-20, 0);
        //Velocity = ForLeaseEngine::Vector::Scale(Velocity, Speed);
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
    collision->Width = 100;
    collision->Height = 10;
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

    ForLeaseEngine::Components::Physics * ObjectPhysics = reinterpret_cast<ForLeaseEngine::Components::Physics *>(Object->GetComponent(ForLeaseEngine::ComponentType::Physics));
    ObjectPhysics->Velocity = ForLeaseEngine::Vector(0, 0);
    Object->Speed = 100;
    ForLease->Dispatcher.Attach(&ForLease->OSInput, Object, "KeyDown", &TestObject::OnKeyDown);
    ForLease->Dispatcher.Attach(&ForLease->OSInput, Object, "KeyUp", &TestObject::OnKeyUp);


}

void EngineProofState::Update() {
    double dt = ForLease->FrameRateController().GetDt();
    // std::cout << dt << std::endl;
    Physics.Update(Entities);
    ForLeaseEngine::Components::Transform * ObjectTransform = reinterpret_cast<ForLeaseEngine::Components::Transform *>(Object->GetComponent(ForLeaseEngine::ComponentType::Transform));
    ForLeaseEngine::Components::Physics * ObjectPhysics = reinterpret_cast<ForLeaseEngine::Components::Physics *>(Object->GetComponent(ForLeaseEngine::ComponentType::Physics));
    ForLeaseEngine::Components::Transform * FloorTransform = reinterpret_cast<ForLeaseEngine::Components::Transform *>(Floor->GetComponent(ForLeaseEngine::ComponentType::Transform));
    std::cout << ObjectTransform->Position[0] << "," << ObjectTransform->Position[1] << std::endl;
    // ObjectTransform->Position = ObjectTransform->Position + ForLeaseEngine::Vector::Scale(ForLeaseEngine::Vector::Scale(ObjectPhysics->Velocity, dt), Object->Speed);
    ForLease->OSInput.ProcessAllInput();
    Collision.Update(Entities);
    render.SetDrawingColor(1, 0, 0);
    render.SetProjection(ForLeaseEngine::Point(0, 0), 800, 600, 0, 100, 0);
    render.DrawMesh(Object->Model, ObjectTransform->Position, ObjectTransform->ScaleX, ObjectTransform->ScaleY, ObjectTransform->Rotation);
    render.DrawMesh(Floor->Model, FloorTransform->Position, FloorTransform->ScaleX, FloorTransform->ScaleY, FloorTransform->Rotation);
    ForLease->Window->UpdateWindow();
}

void EngineProofState::Deinitialize() {
}

void EngineProofState::Unload() {
    delete Object;
}
