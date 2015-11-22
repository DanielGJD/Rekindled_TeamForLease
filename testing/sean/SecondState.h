#ifndef SECOND_STATE_H
#define SECOND_STATE_H

#include "Engine.h"
#include "Entity.h"
#include "Event.h"
#include "LevelComponentsInclude.h"
#include "State.h"

class SecondState : public ForLeaseEngine::State {
public:
    SecondState();
    void Load();
    void Initialize();
    void Update();
    void Deinitialize();
    void Unload();
private:
    int Health;
};

#endif // SECOND_STATE_H
