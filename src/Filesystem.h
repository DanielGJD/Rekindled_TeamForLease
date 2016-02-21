/*!
    \file   Filesystem.h
    \author Sean McGeer
    \date   2/20/16
    \brief
        Defines the Filesystem class.
    \see Filesystem.cpp

    \copyright ©Copyright 2016 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <vector>
#include <string>
#include <unordered_map>

namespace ForLeaseEngine {

    namespace Modules {

        /*!
            \class Filesystem
            \brief
                Handles all platform-specific filesystem operations.
        */
        class Filesystem {

            public:
                Filesystem();
                Filesystem::Filesystem(std::string gameFile);
                void Initialize(std::string gameFile);

                static std::vector<std::string> GetAllFilesInFolder(std::string folder);
                static bool PathExists(std::string path);

                enum class AssetType : unsigned {
                    Sound = 1,
                    Mesh = 2,
                    Animation = 3,
                    Image = 4,
                    Font = 5
                };

                typedef std::pair<AssetType, std::string> AssetPath;

                static void LoadAsset(std::string file, AssetType type);
                static void LoadAsset(AssetPath path);
                static void LoadAllAssetsInDirectory(AssetPath directory);
                void LoadAllAssets();
                std::vector<std::string> GetAssetDirectoryListing(AssetType type);
                std::vector<AssetPath>   GetAllAssetDirectoryListings();
                static std::vector<AssetPath> ParseAssetDirectoryList(std::string file);

                std::string AssetDirectory(AssetType type);
                std::vector<AssetPath> AllAssetDirectories();
            private:
                std::unordered_map<AssetType, std::string> AssetPaths;
                //std::vector<AssetPath> AssetPaths;
        };

    } // Modules

} // ForLeaseEngine

#endif // FILESYSTEM_H
