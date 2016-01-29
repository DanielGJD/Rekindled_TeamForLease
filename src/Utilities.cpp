/*!
    \file   Utilities.cpp
    \author Sean McGeer
    \date   11/29/15

    \brief
        Implements various small utilities that are used by the game or engine.

    \copyright ęCopyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "Utilities.h"
#include "Serialize.h"
#include "State.h"
#include "Level.h"
#include <iostream>
#include <fstream>
#include <algorithm>

namespace FLE = ForLeaseEngine;

std::vector<ForLeaseEngine::State *> LoadLevels(std::string levelList, std::vector<ForLeaseEngine::State *> appendTo) {
    FLE::Serializer serial;
    serial.ReadFile(levelList);
    FLE::Serializer levelArray = serial.GetChild("Levels");
    FLE::ArraySerializer levels(levelArray);

    for (unsigned i = 0; i < levels.Size(); ++i) {
        FLE::Serializer level = levels[i];
        std::string file;
        level.ReadString("File", file);

        FLE::Level* levelInstance = new FLE::Level(file);

        appendTo.push_back(levelInstance);
        std::cout << "Just got " << file << std::endl;
        std::cout << "Name: " << levelInstance->GetName() << std::endl;
    }

    return appendTo;
}

std::vector<ForLeaseEngine::State *> LoadSingleLevel(std::string levelFileName, std::vector<ForLeaseEngine::State *> appendTo) {
    FLE::Level* level = new FLE::Level(levelFileName);
    appendTo.push_back(level);
    return appendTo;
}

namespace CommandLine {
    bool ArgumentExists(char** start, char** end, const std::string argument) {
        char** result = std::find(start, end, argument);

        if (result == end)
            return false;
        else
            return true;
    }

    StringArgument GetStringArgument(char** start, char** end, const std::string argument) {
        char ** result = std::find(start, end, argument);

        if (result != end && ++result != end)
            return std::pair<bool, std::string>(true, *result);

        return StringArgument(false, "Argument not found.");
    }

    IntArgument GetIntArgument(char** start, char** end, const std::string argument) {
        StringArgument result = GetStringArgument(start, end, argument);

        if (result.first) {
            try {
                return IntArgument(true, stoi(result.second));
            } catch (std::invalid_argument) {
                return IntArgument(false, -1);
            }
        }

        return IntArgument(false, -1);
    }
}