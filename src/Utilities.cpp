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
#include "State.h"
#include "Level.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>
#include <sstream>
#include <stdexcept>
#include "Engine.h"

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

    namespace {

        template<typename T>
        T convert(const std::string& input) {
            std::istringstream inputStream(input);
            T converted;

            inputStream >> converted;

            if (!inputStream.eof())
                throw std::invalid_argument("Not integral.");

            return converted;
        }

    }

    IntArgument GetIntArgument(char** start, char** end, const std::string argument) {
        StringArgument result = GetStringArgument(start, end, argument);

        if (result.first) {
//            try {
//                return IntArgument(true, std::stoi(result.second));
//            } catch (std::invalid_argument) {
//                return IntArgument(false, -1);
//            }

            // Changed due to MinGW stoi incompatibility

            try {
                return IntArgument(true, convert<int>(result.second.c_str()));
            }
            catch (std::invalid_argument) {
                return IntArgument(false, -1);
            }
        }

        return IntArgument(false, -1);
    }
}

namespace FileSystem {
#ifdef FLE_WINDOWS
    std::vector<std::string> GetAllFilesInFolder(std::string folder) {
        std::vector<std::string> result;

        char search_path[200];
        sprintf(search_path, "%s/*.*", folder.c_str());
        WIN32_FIND_DATA fd;
        HANDLE hFind = FindFirstFile(search_path, &fd);
        if (hFind != INVALID_HANDLE_VALUE) {
            do {
                if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                    result.push_back(fd.cFileName);
                }
            } while (FindNextFile(hFind, &fd));
            FindClose(hFind);
        }
        return result;
    }
#else
    #include <dirent.h>

    std::vector<std::string> GetAllFilesInFolder(std::string folder) {
        std::vector<std::string> result;

        DIR *directory;
        struct dirent *entry;
        if ((directory = opendir(folder.c_str())) != NULL) {
            while ((entry = readdir(directory)) != NULL) {
                if (entry->d_type == DT_REG)
                    result.push_back(entry->d_name);
            }
            closedir(directory);
        }

        return result;
    }
#endif
}

namespace Preloader {
    void PreloadIndividualAsset(std::string file, Asset type) {
        switch (type) {
            case Asset::Sound:
                ForLease->Resources.LoadSound(file);
                break;
            case Asset::Mesh:
                ForLease->Resources.LoadMesh(file);
                break;
            case Asset::MeshAnimation:
                ForLease->Resources.LoadMeshAnimation(file);
                break;
            case Asset::Texture:
                ForLease->Resources.LoadTexture(file);
                break;
            case Asset::Font:
                ForLease->Resources.LoadFont(file);
                break;
            default: // No idea how you got here.  You screwed something up if that happened.
                break;
        }
    }

    void PreloadAssets(std::vector<AssetPath> paths) {
        for (AssetPath path : paths) {
            std::vector<std::string> filenames = FileSystem::GetAllFilesInFolder(path.second);
            for (std::string file : filenames) {
                PreloadIndividualAsset(path.second + file, path.first);
            }
        }
    }
}
