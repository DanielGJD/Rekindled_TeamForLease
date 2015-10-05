#include <vector>
#include <iostream>
#include "State.h"
#include "Engine.h"
#include "EngineProofState.h"
#include "TestingState.h"
#include "Exception.h"
#undef main

bool quit = false;
using namespace ForLeaseEngine;

int main(int argc, char** argv){
    std::vector<ForLeaseEngine::State *> states;
    TestingState* state = new TestingState();
    states.push_back(state);

//    SDL_Init(0);
//    GraphicsProperties properties = GraphicsProperties();
//    properties.xResolution = 1280;
//    properties.yResolution = 720;
//    Graphics* graphics = Graphics::CreateGraphics(properties);
//    EventDispatcher dispatcher = EventDispatcher();

    //ForLeaseEngine::GraphicsProperties properties;
    //ForLeaseEngine::Graphics* graphics = ForLeaseEngine::Graphics::CreateGraphics(properties);
    ForLeaseEngine::Engine engine(states, 720, 720, 60);
    try {
        engine.Run();
    }
    catch(Exception* e) {
        std::cout << e->GetInfo() << std::endl;
        char c;
        std::cin >> c;
    }
    //while(true);

//    Graphics::DestroyGraphics(graphics);
//    SDL_Quit();
    return 0;
//    Engine engine = Engine(states);
//    Modules::FrameRateController controller = Modules::FrameRateController(engine);
//
//    controller.Start();
//    controller.End();
//    double dt = controller.GetDt();
//    while(true) {
//    controller.Start();
//    controller.End();
//    dt = controller.GetDt();
//    std::cout << dt << std::endl;
//    }
//
//    return 0;
}

void Quit(const Event* e) {
    quit = true;
}
