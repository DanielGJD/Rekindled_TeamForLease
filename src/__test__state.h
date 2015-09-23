#ifndef TEST_STATE_H
#define TEST_STATE_H

#include "State.h"

class TestState : public ForLeaseEngine::State {
    public:
        void Load();
        void Initialize();
        void Update();
        void Deinitialize();
        void Unload();
    private:
        int lives;
        int health;
};

#endif
