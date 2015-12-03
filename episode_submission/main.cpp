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
#include "Platforms.h"

#undef main

int Start() {
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


#ifdef FLE_WINDOWS

#ifndef FLE_DEBUG
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow) {
    return Start();
}
#else
int main(int argc, char**argv) {
    return Start();
}
#endif

#else
int main(int argc, char** argv) {
    return Start();
}
#endif
