/*!
    \file   main.cpp
    \author Sean McGeer
    \date   11/23/15

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include <vector>
#include "State.h"
#include "Engine.h"
#include "Level.h"
#include "MainMenu.h"
#include "Utilities.h"

#undef main

int main(int argc, char** argv) {
    std::vector<ForLeaseEngine::State *> states;
    //MainMenu* state = new MainMenu();
    //states.push_back(state);
    states = LoadLevels("Game.json", states);
    //states.push_back(new ForLeaseEngine::Level("MainMenu.json"));
    //states.push_back(new ForLeaseEngine::Level("Level1Modified.json"));
    //states.push_back(new ForLeaseEngine::Level("Level2.json"));
    //states.push_back(new ForLeaseEngine::Level("Level3.json"));

    ForLeaseEngine::Engine engine(states, 1920, 1080, 60);
    engine.Run();

    return 0;
}
