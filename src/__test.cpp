/*!
    \file   __test.cpp
    \author Sean McGeer
    \date   9/14/15
    \brief
        A test driver for the For Lease Engine.  Feel free to put whatever you
        want in this file to test stuff.
*/

#include <iostream>
#include <iomanip>
#include <vector>

#include "Engine.h"
#include "__test__state.h"
#include "State.h"

int main() {
    std::vector<ForLeaseEngine::State *> states;
    states.push_back(new TestState());

    ForLeaseEngine::Engine engine(states);
    engine.Run();

    return 0;
}
