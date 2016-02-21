/*!
    \file   ResourceManager.cpp
    \author Christopher Hudson

    \brief
        Defines a class that manages all resources used by the engine

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "ResourceManager.h"
#include "Exception.h"
#include "GL/gl.h"
#include "Serialize.h"
#include "Engine.h"
#include <iostream>
#include <fstream>

namespace ForLeaseEngine {
    namespace Modules {
        ResourceManager::ResourceManager() : LoadedTextures(0), LoadedMeshes(0), LoadedFonts(0), TextureRamUsed(0), RamUsed(0) {}

        void ResourceManager::LoadTexture(std::string fileName) {
            UnloadTexture(fileName);
            Texture* texture = Texture::CreateTexture(ForLease->Filesystem.AssetDirectory(Filesystem::AssetType::Image) + fileName);
            if(!texture) {
                //throw(new Exception(std::string("Failed to load texture :").append(fileName)));
                std::cout << "Failed to load texture: " << fileName << std::endl;
                return;
            }
            Textures.insert(std::make_pair(fileName.c_str(), texture));
            ++LoadedTextures;
            TextureRamUsed += texture->GetWidth() * texture->GetHeight() * 4;
            RamUsed += sizeof(Texture);
        }

        Texture* ResourceManager::GetTexture(std::string fileName) {
            std::unordered_map<std::string, Texture*>::iterator i = Textures.find(fileName);
            if(i != Textures.end()) {
                return std::get<1>(*i);
            }

            LoadTexture(fileName);
            i = Textures.find(fileName);
            if(i != Textures.end()) {
                return std::get<1>(*i);
            }

            return NULL;
        }

        void ResourceManager::UnloadTexture(std::string fileName) {
            std::unordered_map<std::string, Texture*>::iterator i = Textures.find(fileName);
            if(i != Textures.end()) {
                Texture* texture = std::get<1>(*i);
                Textures.erase(i);
                --LoadedTextures;
                TextureRamUsed -= texture->GetWidth() * texture->GetHeight() * 4;
                RamUsed -= sizeof(Texture);
                delete texture;
            }
        }

        void ResourceManager::LoadFont(std::string fileName) {
            UnloadFont(fileName);
            std::ifstream fontFile(ForLease->Filesystem.AssetDirectory(Filesystem::AssetType::Font) + fileName, std::ios_base::in | std::ios_base::binary);
            if(!fontFile.is_open()) {
                std::cout << "Failed to load font: " << fileName << std::endl;
                return;
            }
            BmFont fontInfo;
            fontFile >> fontInfo;
            Font* font = new Font(fontInfo);
            Fonts.insert(std::make_pair(fileName.c_str(), font));
            //std::cout << "Loaded Font: " << fileName << std::endl;
            ++LoadedFonts;
            RamUsed += sizeof(Font);
        }

        Font* ResourceManager::GetFont(std::string fileName) {
            std::unordered_map<std::string, Font*>::iterator i = Fonts.find(fileName);
            if(i != Fonts.end()) {
                return std::get<1>(*i);
            }

            //////////////////////////////
            LoadFont(fileName);
            i = Fonts.find(fileName);
            if(i != Fonts.end()) {
                return std::get<1>(*i);
            }
            //////////////////////////////

            return NULL;
        }

        void ResourceManager::UnloadFont(std::string fileName) {
            std::unordered_map<std::string, Font*>::iterator i = Fonts.find(fileName);
            if(i != Fonts.end()) {
                Font* font = std::get<1>(*i);
                Fonts.erase(i);
                --LoadedFonts;
                RamUsed -= sizeof(Font);
                delete font;
                //std::cout << "Unloaded Font: " << fileName << std::endl;
            }
        }

        void ResourceManager::LoadMesh(std::string fileName) {
            UnloadMesh(fileName);
            Serializer root;
            if(!root.ReadFile(ForLease->Filesystem.AssetDirectory(Filesystem::AssetType::Mesh) + fileName))
            {
                std::cout << "Failed to load mesh: " << fileName << std::endl;
                return;
            }
            Mesh* mesh = new Mesh();
            mesh->Deserialize(root);
            Meshes.insert(std::make_pair(fileName.c_str(), mesh));
            ++LoadedMeshes;
            RamUsed += sizeof(Mesh) +
                           sizeof(Point) * mesh->GetVertexCount() * 2 +
                           sizeof(IndexedEdge) * mesh->GetEdgeCount() +
                           sizeof(IndexedFace) * mesh->GetFaceCount() +
                           sizeof(Color) * mesh->GetFaceCount();
        }

        Mesh* ResourceManager::GetMesh(std::string fileName) {
            std::unordered_map<std::string, Mesh*>::iterator i = Meshes.find(fileName);
            if(i != Meshes.end()) {
                return std::get<1>(*i);
            }

            LoadMesh(fileName);
            i = Meshes.find(fileName);
            if(i != Meshes.end()) {
                return std::get<1>(*i);
            }

            return NULL;
        }

        void ResourceManager::UnloadMesh(std::string fileName) {
            std::unordered_map<std::string, Mesh*>::iterator i = Meshes.find(fileName);
            if(i != Meshes.end()) {
                Mesh* mesh = std::get<1>(*i);
                Meshes.erase(i);
                --LoadedMeshes;
                RamUsed -= sizeof(Mesh) +
                           sizeof(Point) * mesh->GetVertexCount() * 2 +
                           sizeof(IndexedEdge) * mesh->GetEdgeCount() +
                           sizeof(IndexedFace) * mesh->GetFaceCount() +
                           sizeof(Color) * mesh->GetFaceCount();
                delete mesh;
            }
        }

        // void ResourceManager::LoadSound(std::string fileName) {
            // UnloadSound(fileName);
            // Sound* sound = ForLease->AudioSystem->CreateSound(fileName);

            // if(!sound) {
                // //throw(new Exception(std::string("Failed to load sound: ").append(fileName)));
                // std::cout << "Failed to load sound: " << fileName << std::endl;
                // return;
            // }

            // Sounds.insert(std::make_pair(fileName.c_str(), sound));
            // ++LoadedSounds;
        // }

        // Sound* ResourceManager::GetSound(std::string fileName) {
            // std::unordered_map<std::string, Sound*>::iterator i = Sounds.find(fileName);
            // if(i != Sounds.end()) {
                // return std::get<1>(*i);
            // }

            // LoadSound(fileName);
            // i = Sounds.find(fileName);
            // if(i != Sounds.end()) {
                // return std::get<1>(*i);
            // }

            // return NULL;
        // }

        // void ResourceManager::UnloadSound(std::string fileName) {
            // std::unordered_map<std::string, Sound*>::iterator i = Sounds.find(fileName);
            // if(i != Sounds.end()) {
                // Sound* sound = std::get<1>(*i);
                // Sounds.erase(i);
                // --LoadedSounds;
                // delete sound;
            // }
        // }

        void ResourceManager::LoadMeshAnimation(std::string fileName) {
            UnloadMeshAnimation(fileName);
            Serializer loader;
            if(!loader.ReadFile(ForLease->Filesystem.AssetDirectory(Filesystem::AssetType::Animation) + fileName)) {
                std::cout << "Failed to load mesh animation: " << fileName << std::endl;
                return;
            }
            MeshAnimation* animation = new MeshAnimation();
            animation->Deserialize(loader);
            MeshAnimations.insert(std::make_pair(fileName, animation));
        }

        MeshAnimation* ResourceManager::GetMeshAnimation(std::string fileName) {
            std::unordered_map<std::string, MeshAnimation*>::iterator i = MeshAnimations.find(fileName);
            if(i != MeshAnimations.end()) {
                return (*i).second;
            }

            LoadMeshAnimation(fileName);
            i = MeshAnimations.find(fileName);
            if(i != MeshAnimations.end()) {
                return (*i).second;
            }

            return NULL;
        }

        void ResourceManager::UnloadMeshAnimation(std::string fileName) {
            std::unordered_map<std::string, MeshAnimation*>::iterator i = MeshAnimations.find(fileName);
            if(i != MeshAnimations.end()) {
                delete (*i).second;
                MeshAnimations.erase(i);
            }
        }

        std::vector<std::string> ResourceManager::GetLoadedTextureNames() {
            std::vector<std::string> textureNames;
            for(std::unordered_map<std::string, Texture*>::iterator i = Textures.begin(); i != Textures.end(); ++i) {
                textureNames.push_back(std::get<0>(*i));
            }
            return textureNames;
        }

        std::vector<std::string> ResourceManager::GetLoadedFontNames() {
            std::vector<std::string> fontNames;
            for(std::unordered_map<std::string, Font*>::iterator i = Fonts.begin(); i != Fonts.end(); ++i) {
                fontNames.push_back(std::get<0>(*i));
            }
            return fontNames;
        }

        std::vector<std::string> ResourceManager::GetLoadedMeshNames() {
            std::vector<std::string> meshNames;
            for(std::unordered_map<std::string, Mesh*>::iterator i = Meshes.begin(); i != Meshes.end(); ++i) {
                meshNames.push_back(std::get<0>(*i));
            }
            return meshNames;
        }

        std::vector<std::string> ResourceManager::GetLoadedSoundNames() {
            std::vector<std::string> soundNames;
            // for(std::unordered_map<std::string, Sound*>::iterator i = Sounds.begin(); i != Sounds.end(); ++i) {
                // soundNames.push_back(std::get<0>(*i));
            // }
            return soundNames;
        }


        std::vector<std::string> ResourceManager::GetLoadedMeshAnimationNames() {
            std::vector<std::string> meshAnimationNames;
            for(std::unordered_map<std::string, MeshAnimation*>::iterator i = MeshAnimations.begin(); i != MeshAnimations.end(); ++i) {
                meshAnimationNames.push_back((*i).first);
            }
            return meshAnimationNames;
        }

        std::ostream& operator<<(std::ostream& os, const ResourceManager& rhs) {
            /*os << "Textures: " << rhs.LoadedTextures << std::endl
               << "Meshes: " << rhs.LoadedMeshes << std::endl
               << "Fonts: " << rhs.LoadedFonts << std::endl
               << "Texture Ram Used: " << rhs.TextureRamUsed << std::endl
               << "Ram Used: " << rhs.RamUsed;*/
            return os;
        }
    }
}
