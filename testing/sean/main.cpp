/*!
    \file   main.cpp
    \author Sean McGeer

    \copyright ęCopyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include <vector>
#include "State.h"
#include "Engine.h"
#include "SeanState.h"
#include "MenuTest.h"
#include "SecondState.h"
#include "RaycastTest.h"
#include "Utilities.h"
#include "RaycastTest.h"
#include "Platforms.h"
#include "HowToPlay.h"
#include "Loading.h"
#include "Debug.h"
#include "MainMenu.h"
#include "PauseMenu.h"
#include "HowToPlay.h"
//#include "SegmentPrototypeState.h"

#undef main

int Start(){
    std::vector<ForLeaseEngine::State *> states;
    //states.push_back(new Loading("Game.json"));
//    states.push_back(new Loading("Game.json"));
    //states.push_back(new Loading("Game.json"));
    //states.push_back(new PauseMenu());
    //states.push_back(new MainMenu());
    //states.push_back(new MenuTest());

    //states.push_back(new Loading("Game.json"));
    states.push_back(new HowToPlay());
    std::cout << "Here" << std::endl;

    ForLeaseEngine::Modules::Debug debug;
    debug.Draw.Collision = true;

    ForLeaseEngine::Engine engine(states, 1024, 768, 60, false, debug);
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
