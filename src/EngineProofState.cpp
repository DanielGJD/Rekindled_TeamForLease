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
}

void TestObject::OnKeyDown(const ForLeaseEngine::Event* e){
    const ForLeaseEngine::KeyboardEvent* keyboardE = reinterpret_cast<const ForLeaseEngine::KeyboardEvent*>(e);
    ForLeaseEngine::Vector direction;
    if(keyboardE->Key == ForLeaseEngine::Keys::Up)
    {
        Velocity += ForLeaseEngine::Vector(0, 1);
        //Velocity = ForLeaseEngine::Vector::Scale(Velocity, Speed);
    }
    if(keyboardE->Key == ForLeaseEngine::Keys::Down)
    {
        Velocity += ForLeaseEngine::Vector(0, -1);
        //Velocity = ForLeaseEngine::Vector::Scale(Velocity, Speed);
    }
    if(keyboardE->Key == ForLeaseEngine::Keys::Left)
    {
        Velocity += ForLeaseEngine::Vector(-1, 0);
        //Velocity = ForLeaseEngine::Vector::Scale(Velocity, Speed);
    }
    if(keyboardE->Key == ForLeaseEngine::Keys::Right)
    {
        Velocity += ForLeaseEngine::Vector(1, 0);
        //Velocity = ForLeaseEngine::Vector::Scale(Velocity, Speed);
    }
    if(keyboardE->Key == ForLeaseEngine::Keys::Q){
       ForLease->GameStateManager().SetAction(ForLeaseEngine::Modules::StateAction::Quit);
    }
}

void TestObject::OnKeyUp(const ForLeaseEngine::Event* e){
    const ForLeaseEngine::KeyboardEvent* keyboardE = reinterpret_cast<const ForLeaseEngine::KeyboardEvent*>(e);
    if(keyboardE->Key == ForLeaseEngine::Keys::Up)
    {
        Velocity += ForLeaseEngine::Vector(0, -1);
        //Velocity = ForLeaseEngine::Vector::Scale(Velocity, Speed);
    }
    if(keyboardE->Key == ForLeaseEngine::Keys::Down)
    {
        Velocity += ForLeaseEngine::Vector(0, 1);
        //Velocity = ForLeaseEngine::Vector::Scale(Velocity, Speed);
    }
    if(keyboardE->Key == ForLeaseEngine::Keys::Left)
    {
        Velocity += ForLeaseEngine::Vector(1, 0);
        //Velocity = ForLeaseEngine::Vector::Scale(Velocity, Speed);
    }
    if(keyboardE->Key == ForLeaseEngine::Keys::Right)
    {
        Velocity += ForLeaseEngine::Vector(-1, 0);
        //Velocity = ForLeaseEngine::Vector::Scale(Velocity, Speed);
    }
}

EngineProofState::EngineProofState()  {
    //MainEngine = ForLease;
}

void EngineProofState::Load() {
    Object = new TestObject();
}

void EngineProofState::Initialize() {
    Object->Translation = ForLeaseEngine::Point(-400, 0);
    Object->Rotation = 0;
    Object->ScaleX = 100;
    Object->ScaleY = 100;
    Object->Velocity = ForLeaseEngine::Vector(0, 0);
    Object->Speed = 100;
    ForLease->Dispatcher.Attach(&ForLease->OSInput, Object, "KeyDown", &TestObject::OnKeyDown);
    ForLease->Dispatcher.Attach(&ForLease->OSInput, Object, "KeyUp", &TestObject::OnKeyUp);
}

void EngineProofState::Update() {
    double dt = ForLease->FrameRateController().GetDt();
    std::cout << dt << std::endl;
    Object->Translation = Object->Translation + ForLeaseEngine::Vector::Scale(ForLeaseEngine::Vector::Scale(Object->Velocity, dt), Object->Speed);
    ForLease->OSInput.ProcessAllInput();
    render.SetDrawingColor(1, 0, 0);
    render.SetProjection(ForLeaseEngine::Point(0, 0), 800, 600, 0, 100, 0);
    render.DrawMesh(Object->Model, Object->Translation, Object->ScaleX, Object->ScaleY, Object->Rotation);
    ForLease->Window->UpdateWindow();
}

void EngineProofState::Deinitialize() {
}

void EngineProofState::Unload() {
    delete Object;
}
