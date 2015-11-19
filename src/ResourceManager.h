#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "Texture.h"
#include "Mesh.h"
#include "Font.h"
#include "Sound.h"
#include "MeshAnimation.h"
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
                void LoadSound(std::string fileName);
                void LoadMeshAnimation(std::string fileName);
                void UnloadTexture(std::string fileName);
                void UnloadMesh(std::string fileName);
                void UnloadFont(std::string fileName);
                void UnloadSound(std::string fileName);
                void UnloadMeshAnimation(std::string fileName);
                void UnloadAllResources();
                Texture* GetTexture(std::string fileName);
                Mesh* GetMesh(std::string fileName);
                Font* GetFont(std::string fileName);
                Sound* GetSound(std::string fileName);
                MeshAnimation* GetMeshAnimation(std::string fileName);

                unsigned int GetNumTextures();
                unsigned int GetNumMeshes();
                unsigned int GetNumFonts();
                unsigned int GetNumSounds();
                unsigned int GetNumMeshAnimations();
                unsigned int GetTextureRamUsed();
                unsigned int GetRamUsed();

                std::vector<std::string> GetLoadedTextureNames();
                std::vector<std::string> GetLoadedFontNames();
                std::vector<std::string> GetLoadedMeshNames();
                std::vector<std::string> GetLodedSoundNames();
                std::vector<std::string> GetLoadedMeshAnimationNames();

                friend std::ostream& operator<<(std::ostream& os, const ResourceManager& resources);

            private:
                std::unordered_map<std::string, Texture*> Textures;
                std::unordered_map<std::string, Mesh*> Meshes;
                std::unordered_map<std::string, Font*> Fonts;
                std::unordered_map<std::string, Sound*> Sounds;
                std::unordered_map<std::string, MeshAnimation*> MeshAnimations;

                unsigned int LoadedTextures;
                unsigned int LoadedMeshes;
                unsigned int LoadedFonts;
                unsigned int LoadedSounds;
                unsigned int TextureRamUsed;
                unsigned int RamUsed;
        };
    }
}

#endif // RESOURCE_MANAGER_H
