/*!
    \file   Utilities.h
    \author Sean McGeer
    \date   11/29/15

    \brief
        Defines various small utilities that are used by the game or engine.

    \copyright ęCopyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "Level.h"
#include <string>
#include <vector>

#ifndef UTILITIES_H
#define UTILITIES_H

std::vector<ForLeaseEngine::State *> LoadLevels(std::string levelList, std::vector<ForLeaseEngine::State *> appendTo = std::vector<ForLeaseEngine::State *>());
std::vector<ForLeaseEngine::State *> LoadSingleLevel(std::string levelFileName, std::vector<ForLeaseEngine::State *> appendTo = std::vector<ForLeaseEngine::State *>());

namespace CommandLine {
    typedef std::pair<bool, std::string> StringArgument;
    typedef std::pair<bool, int> IntArgument;

    bool ArgumentExists(char** start, char** end, const std::string argument);
    StringArgument GetStringArgument(char** start, char** end, const std::string argument);
    IntArgument GetIntArgument(char** start, char** end, const std::string argument);
}

void SpawnProcess(const std::string process, std::string arguments);
void SpawnNewLevelProcess(const std::string level);

#endif // UTILITIES_H