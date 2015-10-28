#include <vector>
#include "State.h"
#include "Engine.h"
#include "SeanState.h"

int main(int argc, char** argv){
    std::vector<ForLeaseEngine::State *> states;
    SeanState* state = new SeanState();
    states.push_back(state);

    ForLeaseEngine::Engine engine(states, 1024, 768, 60);
    engine.Run();

    return 0;
}
