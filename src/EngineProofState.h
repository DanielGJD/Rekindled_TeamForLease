#ifndef ENGINE_PROOF_STATE_H
#define ENGINE_PROOF_STATE_H

#include "Renderer.h"
#include "Mesh.h"
#include "Vector.h"
#include "Engine.h"
#include "Event.h"

class TestObject {
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
        ForLeaseEngine::Renderer render;
};



#endif // ENGINE_PROOF_STATE_H
