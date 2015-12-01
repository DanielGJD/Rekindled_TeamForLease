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
//#include "SegmentPrototypeState.h"

#undef main

int main(int argc, char** argv){
    std::vector<ForLeaseEngine::State *> states;
    //ForLeaseEngine::SegmentPrototypeState* state = new ForLeaseEngine::SegmentPrototypeState();
    //SeanState* state = new SeanState();
    //states.push_back(state);
    //states.push_back(new SecondState());
    //states = LoadLevels("Game.json", states);
    states.push_back(new RaycastTest());

    ForLeaseEngine::Engine engine(states, 1024, 768, 60);
    engine.Run();

    return 0;
}
