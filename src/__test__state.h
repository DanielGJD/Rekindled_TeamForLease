#ifndef TEST_STATE_H
#define TEST_STATE_H

#include "State.h"
#include <iostream>

class TestState : public ForLeaseEngine::State {
    public:
        void Load() { std::cout << "Loading" << std::endl; lives = 1; }
        void Initialize() { std::cout << "Initializing" << std::endl; health = 100; }
        void Update() {
            --health;
            if (health == 0) {
                if (lives == 0)
                    Parent.GameStateManager().SetAction(StateAction::Quit);
                else
                    Parent.GameStateManager().SetAction(StateAction::Restart);

                --lives;
            }

            std::cout << "Lives: " << lives << " | Health: " << health << std::endl;
        };
        void Deinitialize() { std::cout << "Deinitializing" << std::endl; }
        void Unload() { std::cout << "Unloading" << std::endl; }
    private:
        int lives;
        int health;
};

#endif
