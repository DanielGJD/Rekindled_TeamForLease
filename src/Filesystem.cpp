/*!
    \file   Filesystem.h
    \author Sean McGeer
    \date   2/20/16
    \brief
        Implements the Filesystem class.
    \see Filesystem.h

    \copyright ©Copyright 2016 DigiPen Institute of Technology, All Rights Reserved
*/

#include "Filesystem.h"
#include "Platforms.h"
#include "Engine.h"

namespace ForLeaseEngine {

    namespace Modules {

        /*!
            Basic constructor for the Filesystem.  Includes default locations.
        */
        Filesystem::Filesystem() {
            if (PathExists("levels/")) AssetPaths.insert({ AssetType::Level, "levels/" });
            if (PathExists("blueprints/")) AssetPaths.insert({ AssetType::Blueprint, "blueprints/" });
            if (PathExists("sounds/")) AssetPaths.insert({ AssetType::Sound, "sounds/" });
            if (PathExists("meshes/")) AssetPaths.insert({ AssetType::Mesh, "meshes/" });
            if (PathExists("animations/")) AssetPaths.insert({ AssetType::Animation, "animations/" });
            if (PathExists("images/")) AssetPaths.insert({ AssetType::Image, "images/" });
            if (PathExists("fonts/")) AssetPaths.insert({ AssetType::Font, "fonts/" });
        }

        Filesystem::Filesystem(std::string gameFile) {
            if (PathExists("levels/")) AssetPaths.insert({ AssetType::Level, "levels/" });
            if (PathExists("blueprints/")) AssetPaths.insert({ AssetType::Blueprint, "blueprints/" });
            if (PathExists("sounds/")) AssetPaths.insert({ AssetType::Sound, "sounds/" });
            if (PathExists("meshes/")) AssetPaths.insert({ AssetType::Mesh, "meshes/" });
            if (PathExists("animations/")) AssetPaths.insert({ AssetType::Animation, "animations/" });
            if (PathExists("images/")) AssetPaths.insert({ AssetType::Image, "images/" });
            if (PathExists("fonts/")) AssetPaths.insert({ AssetType::Font, "fonts/" });
            Initialize(gameFile);
        }

        /*!
            If we constructed the Filesystem instance already, but want to fix it
            with a game file.
        */
        void Filesystem::Initialize(std::string gameFile) {
            std::vector<AssetPath> newPaths = ParseAssetDirectoryList(gameFile);

            //for (AssetPath oldPath : AssetPaths) {
            //    for (AssetPath newPath : newPaths) {
            //        if (newPath.first == oldPath.first)
            //            oldPath.second = newPath.second;
            //    }
            //}

            for (AssetPath newPath : newPaths) {
                auto oldPath = AssetPaths.find(newPath.first);
                if (oldPath != AssetPaths.end()) oldPath->second = newPath.second;
                else AssetPaths.insert({ newPath.first, newPath.second });
            }
        }

        #ifdef FLE_WINDOWS
            std::vector<std::string> Filesystem::GetAllFilesInFolder(std::string folder) {
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

            bool Filesystem::PathExists(std::string path) {
                return GetFileAttributes(path.c_str()) != INVALID_FILE_ATTRIBUTES;
            }
        #else
            #include <dirent.h>
            #include <unistd.h>

            std::vector<std::string> Filesystem::GetAllFilesInFolder(std::string folder) {
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

            bool Filesystem::PathExists(std::string path) {
                return access(path.c_str(), F_OK) != -1;
            }
        #endif

        void Filesystem::LoadAsset(std::string file, AssetType type) {
            switch (type) {
                //case AssetType::Sound:
                //    ForLease->Resources.LoadSound(AssetPaths.at(AssetType::Sound) + file);
                //    break;
                case AssetType::Mesh:
                    ForLease->Resources.LoadMesh(file);
                    break;
                case AssetType::Animation:
                    ForLease->Resources.LoadMeshAnimation(file);
                    break;
                case AssetType::Image:
                    ForLease->Resources.LoadTexture(file);
                    break;
                case AssetType::Font:
                    ForLease->Resources.LoadFont(file);
                    break;
                default: // No idea how you got here.  You screwed something up if that happened.
                    break;
            }
        }

        void Filesystem::LoadAsset(AssetPath path) {
            LoadAsset(path.second, path.first);
        }

        void Filesystem::LoadAllAssets() {
            for (auto it = AssetPaths.begin(); it != AssetPaths.end(); ++it) {
                LoadAllAssetsInDirectory(AssetPath(it->first, it->second));
            }
        }

        void Filesystem::LoadAllAssetsInDirectory(AssetPath directory) {
            std::vector<std::string> filenames = GetAllFilesInFolder(directory.second);
            for (std::string file : filenames)
                LoadAsset(file, directory.first);
        }

        std::vector<std::string> Filesystem::GetAssetDirectoryListing(AssetType type) {
            std::vector<std::string> result;

            AssetPath pathInQuestion(type, AssetPaths.at(type));

            result = GetAllFilesInFolder(pathInQuestion.second);

            return result;
        }

        std::vector<Filesystem::AssetPath> Filesystem::GetAllAssetDirectoryListings() {
            std::vector<AssetPath> result;

            for (AssetPath assetDirectory : AssetPaths) {
                std::vector<std::string> fileListing = GetAssetDirectoryListing(assetDirectory.first);
                for (std::string file : fileListing) {
                    result.push_back(AssetPath(assetDirectory.first, file));
                }
            }

            return result;
        }

        std::vector<Filesystem::AssetPath> Filesystem::ParseAssetDirectoryList(std::string file) {
            Serializer serial;
            serial.ReadFile(file);
            Serializer assetArray = serial.GetChild("Assets");
            ArraySerializer assets(assetArray);

            std::vector<AssetPath> assetPaths;

            for (unsigned i = 0; i < assets.Size(); ++i) {
                Serializer asset = assets[i];

                AssetPath newAssetPath;

                unsigned type;
                asset.ReadUint("Type", type);
                newAssetPath.first = static_cast<AssetType>(type);
                asset.ReadString("File", newAssetPath.second);

                assetPaths.push_back(newAssetPath);
            }

            return assetPaths;
        }

        std::string Filesystem::AssetDirectory(AssetType type) {
            return AssetPaths.at(type);
        }

        std::vector<Filesystem::AssetPath> Filesystem::AllAssetDirectories() {
            std::vector<AssetPath> paths;

            for (auto it = AssetPaths.begin(); it != AssetPaths.end(); ++it)
                paths.push_back(AssetPath(it->first, it->second));

            return paths;
        }
    } // Modules

} // ForLeaseEngine
