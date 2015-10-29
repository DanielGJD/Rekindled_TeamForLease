#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "Texture.h"
#include "Mesh.h"
#include "Font.h"
#include <unordered_map>
#include <string>

namespace ForLeaseEngine {
    namespace Modules {
        class ResourceManager {
            public:
                ResourceManager();
                void LoadTexture(std::string fileName);
                void LoadMesh(std::string fileName);
                void LoadFont(std::string fileName);
                void UnloadTexture(std::string fileName);
                void UnloadMesh(std::string fileName);
                void UnloadFont(std::string fileName);
                void UnloadAllResources();
                Texture* GetTexture(std::string fileName);
                Mesh* GetMesh(std::string fileName);
                Font* GetFont(std::string fileName);

                unsigned int GetNumTextures();
                unsigned int GetNumMeshes();
                unsigned int GetNumFonts();
                unsigned int GetTextureRamUsed();
                unsigned int GetRamUsed();

                std::vector<std::string> GetLoadedTextureNames();
                std::vector<std::string> GetLoadedFontNames();
                std::vector<std::string> GetLoadedMeshNames();

                friend std::ostream& operator<<(std::ostream& os, const ResourceManager& resources);

            private:
                std::unordered_map<std::string, Texture*> Textures;
                std::unordered_map<std::string, Mesh*> Meshes;
                std::unordered_map<std::string, Font*> Fonts;

                unsigned int LoadedTextures;
                unsigned int LoadedMeshes;
                unsigned int LoadedFonts;
                unsigned int TextureRamUsed;
                unsigned int RamUsed;
        };
    }
}

#endif // RESOURCE_MANAGER_H
