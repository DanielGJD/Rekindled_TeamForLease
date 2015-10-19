#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "Texture.h"

namespace ForLeaseEngine {
    Texture::Texture(GLuint id, std::string name, int width, int height) : ID(id), Name(name), Width(width), Height(height) {}

    Texture* Texture::CreateTexture(std::string filename) {
        SDL_Surface* surface = IMG_Load(filename.c_str());
        GLuint id;
        if(surface == NULL) {
            std::cout << "Unable to load texture: " << filename << std::endl;
            return NULL;
        }

        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glBindTexture(GL_TEXTURE_2D, 0);
        Texture* texture = new Texture(id, filename, surface->w, surface->h);
        SDL_FreeSurface(surface);

        /*std::cout << "Created texture:" << std::endl << "ID: " << static_cast<unsigned int>(texture->GetID()) << std::endl
                  << "Filename: " << texture->GetName() << std::endl << "Dimensions: " << texture->GetWidth() << "x" << texture->GetHeight() << std::endl;*/
        return texture;
    }

    void Texture::DeleteTexture(Texture* texture) {
        GLuint id = texture->GetID();
        glDeleteTextures(1, &id);
        delete texture;
        texture = NULL;
    }

    GLuint Texture::GetID() {
        return ID;
    }

    const std::string& Texture::GetName() {
        return Name;
    }

    int Texture::GetWidth() {
        return Width;
    }

    int Texture::GetHeight() {
        return Height;
    }
}
