/*!
    \file   Random.h
    \author Christopher Hudson

    \brief
        Defines a functions for generating random numbers

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef FLE_RANDOM_H
#define FLE_RANDOM_H

#include <cstdlib>
#include <ctime>

namespace ForLeaseEngine {
    void RandomSeed();
    int RandomInt(int min = 0, int max = RAND_MAX);
    float RandomFloat(float min = 0, float max = 1);
}

#endif // FLE_RANDOM_H
