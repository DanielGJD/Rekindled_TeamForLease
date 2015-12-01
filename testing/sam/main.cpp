#include <vector>
#include <iostream>
#include "State.h"
#include "Engine.h"
#include "LevelEditor.h"
#include "Exception.h"
#include "ResourceManager.h"
#include "Texture.h"
#undef main

using namespace ForLeaseEngine;

int main(int argc, char** argv){
    std::vector<ForLeaseEngine::State *> states;
    LevelEditor* state = new LevelEditor();
    states.push_back(state);

    ForLeaseEngine::Engine engine(states, 1280, 720, 60);
    try {
        engine.Run();
    }
    catch(Exception* e) {
        std::cout << e->GetInfo() << std::endl;
        char c;
        std::cin >> c;
    }


    return 0;
}
