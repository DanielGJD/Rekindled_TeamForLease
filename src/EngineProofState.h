#ifndef ENGINE_PROOF_STATE_H
#define ENGINE_PROOF_STATE_H

#include "Renderer.h"
#include "Mesh.h"
#include "Vector.h"
#include "Engine.h"
#include "Event.h"
#include "Entity.h"
#include "ComponentsInclude.h"
#include "SystemPhysics.h"
#include "SystemCollision.h"

class TestObject : public ForLeaseEngine::Entity {
    public:
        ForLeaseEngine::Mesh Model;
        ForLeaseEngine::Point Translation;
        float Rotation;
        float ScaleX;
        float ScaleY;
        ForLeaseEngine::Vector Velocity;
        float Speed;

        TestObject();
        void OnKeyDown(const ForLeaseEngine::Event* e);
        void OnKeyUp(const ForLeaseEngine::Event* e);
};

class TestFloor : public ForLeaseEngine::Entity {
    public:
        ForLeaseEngine::Mesh Model;
        TestFloor();
};

class EngineProofState : public ForLeaseEngine::State {
    public:
        EngineProofState();
        void Load();
        void Initialize();
        void Update();
        void Deinitialize();
        void Unload();
    private:
        ForLeaseEngine::Renderer Render;
        ForLeaseEngine::Engine* MainEngine;
        TestObject* Object;
        TestFloor* Floor;
        ForLeaseEngine::Renderer render;
        ForLeaseEngine::Systems::Physics Physics;
        ForLeaseEngine::Systems::Collision Collision;
};



#endif // ENGINE_PROOF_STATE_H
