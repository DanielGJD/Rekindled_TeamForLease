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

#undef main

int Start(int argc = 0, char** argv = 0) {
    for (int i = 0; i < argc; ++i) {
        std::cout << argv[i] << std::endl;
    }

    std::vector<ForLeaseEngine::State *> states;

    // Used for parsing command line
    char** argStart = argv;
    char** argEnd = argv + argc;

    // Find the testing level (if there is one).
    CommandLine::StringArgument testingLevel = CommandLine::GetStringArgument(argStart, argEnd, "-level");

    if (testingLevel.first)
        states = LoadSingleLevel(testingLevel.second, states);
    else {
        states = LoadLevels("Game.json", states);
        states.push_back(new HowToPlay());
    }

    // Set other options to defaults
    unsigned resolutionX = 1920;
    unsigned resolutionY = 1080;
    unsigned fps         = 60;

    CommandLine::IntArgument rxArg  = CommandLine::GetIntArgument(argStart, argEnd, "-x");
    CommandLine::IntArgument ryArg  = CommandLine::GetIntArgument(argStart, argEnd, "-y");
    CommandLine::IntArgument fpsArg = CommandLine::GetIntArgument(argStart, argEnd, "-fps");

    if (rxArg.first) resolutionX = rxArg.second;
    if (ryArg.first) resolutionY = ryArg.second;
    if (fpsArg.first) fps = fpsArg.second;

    // Right now, windowed is the default.  We may wish to change this at submission.
    bool fullscreen = CommandLine::ArgumentExists(argStart, argEnd, "-fullscreen");

    ForLeaseEngine::Engine engine(states, resolutionX, resolutionY, fps, fullscreen);
    engine.Run();

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
