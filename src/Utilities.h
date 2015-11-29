/*!
    \file   Utilities.h
    \author Sean McGeer
    \date   11/29/15

    \brief
        Defines various small utilities that are used by the game or engine.

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "Level.h"
#include <string>
#include <vector>

#ifndef UTILITIES_H
#define UTILITIES_H

std::vector<ForLeaseEngine::State *> LoadLevels(std::string levelList, std::vector<ForLeaseEngine::State *> appendTo = std::vector<ForLeaseEngine::State *>());

#endif // UTILITIES_H