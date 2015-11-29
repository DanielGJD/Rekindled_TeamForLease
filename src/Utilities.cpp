/*!
    \file   Utilities.cpp
    \author Sean McGeer
    \date   11/29/15

    \brief
        Implements various small utilities that are used by the game or engine.

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "Utilities.h"
#include "Serialize.h"
#include <iostream>
#include <fstream>

namespace FLE = ForLeaseEngine;

std::vector<ForLeaseEngine::State *> LoadLevels(std::string levelList, std::vector<ForLeaseEngine::State *> appendTo) {
    FLE::Serializer serial;
    serial.ReadFile(levelList);
    FLE::ArraySerializer levels(serial);

    return appendTo;
}