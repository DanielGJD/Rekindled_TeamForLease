#ifndef SEAN_STATE_H
#define SEAN_STATE_H

#include "Engine.h"
#include "Entity.h"
#include "Event.h"
#include "LevelComponentsInclude.h"
#include "State.h"

class SeanState : public ForLeaseEngine::State {
    public:
        void Load();
        void Initialize();
        void Update();
        void Deinitialize();
        void Unload();
};

#endif // SEAN_STATE_H
