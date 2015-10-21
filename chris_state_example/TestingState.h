#ifndef TESTINGSTATE_H
#define TESTINGSTATE_H

#include "LevelComponentsInclude.h"
#include "Mesh.h"
#include "Vector.h"
#include "Engine.h"
#include "Event.h"
#include "Entity.h"
#include "ComponentsInclude.h"

class TestingState : public ForLeaseEngine::State {
    public:
        //TestingState();
        void Load();
        void Initialize();
        void Update();
        void Deinitialize();
        void Unload();
    private:
        ForLeaseEngine::Engine* MainEngine;
};



#endif // ENGINE_PROOF_STATE_H

