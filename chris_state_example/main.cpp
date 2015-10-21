#include <vector>
#include <iostream>
#include "State.h"
#include "Engine.h"
#include "TestingState.h"
#include "Exception.h"
#include "ResourceManager.h"
#include "Texture.h"
#undef main

using namespace ForLeaseEngine;

int main(int argc, char** argv){
    std::vector<ForLeaseEngine::State *> states;
    TestingState* state = new TestingState();
    states.push_back(state);

    ForLeaseEngine::Engine engine(states, 720, 720, 60);
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
