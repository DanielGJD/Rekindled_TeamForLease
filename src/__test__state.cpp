#include "__test__state.h"
#include "Engine.h"
#include "GameStateManager.h"
#include <iostream>

void TestState::Load() { std::cout << "Loading" << std::endl; lives = 1; }

void TestState::Initialize() { std::cout << "Initializing" << std::endl; health = 100; }

void TestState::Update() {
    --health;
    if (health == 0) {
        if (lives == 0)
            ForLease->GameStateManager().SetAction(ForLeaseEngine::Modules::StateAction::Quit);
        else
            ForLease->GameStateManager().SetAction(ForLeaseEngine::Modules::StateAction::Restart);

        --lives;
    }

    std::cout << "Lives: " << lives << " | Health: " << health << std::endl;
}

void TestState::Deinitialize() { std::cout << "Deinitializing" << std::endl; }

void TestState::Unload() { std::cout << "Unloading" << std::endl; }
