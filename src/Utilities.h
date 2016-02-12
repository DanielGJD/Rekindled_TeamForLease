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
#include "ResourceManager.h"

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

namespace FileSystem {
    std::vector<std::string> GetAllFilesInFolder(std::string folder);
}

namespace Preloader {
    enum class Asset : char {
        Sound = 1,
        Mesh = 2,
        MeshAnimation = 3,
        Texture = 4,
        Font = 5
    };

    typedef std::pair<Asset, std::string> AssetPath;

    void PreloadAssets(std::vector<AssetPath> paths);
}

#endif // UTILITIES_H