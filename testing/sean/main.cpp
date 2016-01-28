/*!
    \file   main.cpp
    \author Sean McGEer

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include <vector>
#include "State.h"
#include "Engine.h"
#include "SeanState.h"
#include "SecondState.h"
#include "RaycastTest.h"
#include "Utilities.h"
#include "RaycastTest.h"
#include "Platforms.h"
#include "HowToPlay.h"
//#include "SegmentPrototypeState.h"

#undef main

int Start(){
    std::vector<ForLeaseEngine::State *> states;
    //ForLeaseEngine::SegmentPrototypeState* state = new ForLeaseEngine::SegmentPrototypeState();
    //SeanState* state = new SeanState();
    //states.push_back(new SeanState());
    //states.push_back(new SecondState());
    //states.push_back(new SecondState());
    //states = LoadLevels("Game.json", states);
    //states.push_back(new RaycastTest());

    states = LoadLevels("Game.json", states);
    states.push_back(new HowToPlay());
    //states.push_back(new SeanState());


    ForLeaseEngine::Engine engine(states, 1024, 768, 60);
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
