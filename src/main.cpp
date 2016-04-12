/*!
    \file   main.cpp
    \author Sean McGeer
    \date   11/23/15
    \brief
        This is the file that is considered the "definitive" main.cpp file.  This
        is what we build the engine on before designing levels/submitting the game.

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include <vector>
#include <string>
#include "State.h"
#include "Engine.h"
#include "Level.h"
#include "MainMenu.h"
#include "HowToPlay.h"
#include "Utilities.h"
#include "Platforms.h"
#include "Loading.h"
#include "Filesystem.h"
#include "Debug.h"
#include "Exception.h"
#include "DigiPenLogo.h"
#include "Interpolation.h" 

#undef main

int Start(int argc = 0, char** argv = 0) {
    std::vector<ForLeaseEngine::State *> states;

    // Used for parsing command line
    char** argStart = argv;
    char** argEnd = argv + argc;

    // Find the testing level (if there is one).
    CommandLine::StringArgument testingLevel = CommandLine::GetStringArgument(argStart, argEnd, "-level");

    if (testingLevel.first)
        states = LoadSingleLevel(testingLevel.second, states);
    else if (ForLeaseEngine::Modules::Filesystem::PathExists("Game.json")) {
        states.push_back(new DigiPenLogo());
        states.push_back(new Loading("Game.json"));
        states.push_back(new MainMenu());
        states = LoadLevels("Game.json", states);
        states.push_back(new HowToPlay());
    }
    else { // This is a pretty screwed-up place to be.  Just loads the main menu and the how to play screen.  No gameplay is loaded.
        states.push_back(new MainMenu());
        states.push_back(new HowToPlay());
    }

    // Set other options to defaults
    unsigned resolutionX = 1280;
    unsigned resolutionY = 720;
    unsigned fps         = 60;

    CommandLine::IntArgument rxArg  = CommandLine::GetIntArgument(argStart, argEnd, "-x");
    CommandLine::IntArgument ryArg  = CommandLine::GetIntArgument(argStart, argEnd, "-y");
    CommandLine::IntArgument fpsArg = CommandLine::GetIntArgument(argStart, argEnd, "-fps");

    if (rxArg.first) resolutionX = rxArg.second;
    if (ryArg.first) resolutionY = ryArg.second;
    if (fpsArg.first) fps = fpsArg.second;

    // Right now, windowed is the default.  We may wish to change this at submission.
    bool fullscreen = CommandLine::ArgumentExists(argStart, argEnd, "-fullscreen");

    ForLeaseEngine::Modules::Debug debug;
    debug.Draw.Collision = CommandLine::ArgumentExists(argStart, argEnd, "-dDrawCollision");

    ForLeaseEngine::Engine engine(states, resolutionX, resolutionY, fps, fullscreen, debug);

    try{
        engine.Run();
    }
    catch(ForLeaseEngine::Exception* e) {
        std::cout << e->GetInfo() << std::endl;
    }
    catch(ForLeaseEngine::Exception& e) {
        std::cout << e.GetInfo() << std::endl;
    }

    return 0;
}


#ifdef FLE_WINDOWS

    #ifdef _MSC_VER
        #ifndef FLE_DEBUG
        int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow) {
            return Start(__argc, __argv);
        }
        #else
        int main(int argc, char**argv) {
            return Start(argc, argv);
        }
        #endif //FLE_DEBUG
    #else
    int main(int argc, char** argv) {
        return Start(argc, argv);
    }
    #endif // _MSC_VER

#else
    int main(int argc, char** argv) {
        return Start(argc, argv);
    }
#endif // FLE_WINDOWS
