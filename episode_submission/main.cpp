#include <vector>
#include "State.h"
#include "Engine.h"
#include "Level.h"
#include "MainMenu.h"

#undef main

int main(int argc, char** argv) {
    std::vector<ForLeaseEngine::State *> states;
    MainMenu* state = new MainMenu();
    states.push_back(state);
    states.push_back(new ForLeaseEngine::Level("SecondStateTest.json"));

    ForLeaseEngine::Engine engine(states, 1024, 768, 60);
    engine.Run();

    return 0;
}
