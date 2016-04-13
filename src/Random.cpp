/*!
    \file   Random.cpp
    \author Christopher Hudson

    \brief
        Defines functions for generating random numbers

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "Random.h"

namespace ForLeaseEngine {
    void RandomSeed() {
        srand(time(NULL));
    }

    int RandomInt(int min, int max) {
        return rand() % (max - min) + min;
    }

    float RandomFloat(float min, float max) {
        return static_cast<float>(rand()) / RAND_MAX * (max - min) + min;
    }
}
