#ifndef TESTINGSTATE_H
#define TESTINGSTATE_H

#include "LevelComponentsInclude.h"
#include "Mesh.h"
#include "Vector.h"
#include "Engine.h"
#include "Event.h"
#include "Entity.h"
#include "ComponentsInclude.h"

class TestState : public ForLeaseEngine::State {
    public:
        void Load();
        void Initialize();
        void Update();
        void Deinitialize();
        void Unload();
};



#endif // ENGINE_PROOF_STATE_H


