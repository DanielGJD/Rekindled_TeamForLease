#include "ResourceManager.h"
#include "Exception.h"
#include "GL/gl.h"
#include "Serialize.h"
#include <iostream>
#include <fstream>

namespace ForLeaseEngine {
    namespace Modules {
        ResourceManager::ResourceManager() : LoadedTextures(0), LoadedMeshes(0), LoadedFonts(0), TextureRamUsed(0), RamUsed(0) {}

        void ResourceManager::LoadTexture(std::string fileName) {
            UnloadTexture(fileName);
            Texture* texture = Texture::CreateTexture(fileName);
            if(!texture) {
                throw(new Exception(std::string("Failed to load texture :").append(fileName)));
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
            std::ifstream fontFile(fileName, std::ios_base::in | std::ios_base::binary);
            BmFont fontInfo;
            fontFile >> fontInfo;
            Font* font = new Font(fontInfo);
            Fonts.insert(std::make_pair(fileName.c_str(), font));
            std::cout << "Loaded Font: " << fileName << std::endl;
            ++LoadedFonts;
            RamUsed += sizeof(Font);
        }

        Font* ResourceManager::GetFont(std::string fileName) {
            std::unordered_map<std::string, Font*>::iterator i = Fonts.find(fileName);
            if(i != Fonts.end()) {
                return std::get<1>(*i);
            }

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
                std::cout << "Unloaded Font: " << fileName << std::endl;
            }
        }

        void ResourceManager::LoadMesh(std::string fileName) {
            UnloadMesh(fileName);
            Mesh* mesh = new Mesh();
            Serializer root;
            root.ReadFile(fileName);
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

        std::ostream& operator<<(std::ostream& os, const ResourceManager& rhs) {
            os << "Textures: " << rhs.LoadedTextures << std::endl
               << "Meshes: " << rhs.LoadedMeshes << std::endl
               << "Fonts: " << rhs.LoadedFonts << std::endl
               << "Texture Ram Used: " << rhs.TextureRamUsed << std::endl
               << "Ram Used: " << rhs.RamUsed;
            return os;
        }
    }
}
