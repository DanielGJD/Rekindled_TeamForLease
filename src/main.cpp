#include <vector>
#include <iostream>
#include "EngineProofState.h"
#include "__test__state.h"
#include "Graphics.h"
#include "Input.h"
#include "FrameRateController.h"
#include <SDL.h>
#undef main

bool quit = false;
using namespace ForLeaseEngine;

int main(int argc, char** argv){
    std::vector<ForLeaseEngine::State *> states;
    states.push_back(new EngineProofState());

//    SDL_Init(0);
//    GraphicsProperties properties = GraphicsProperties();
//    properties.xResolution = 1280;
//    properties.yResolution = 720;
//    Graphics* graphics = Graphics::CreateGraphics(properties);
//    EventDispatcher dispatcher = EventDispatcher();

    //ForLeaseEngine::GraphicsProperties properties;
    //ForLeaseEngine::Graphics* graphics = ForLeaseEngine::Graphics::CreateGraphics(properties);
    ForLeaseEngine::Engine engine(states, 800, 600, 60);
    engine.Run();
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