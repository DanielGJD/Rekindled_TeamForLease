/*!
    \file   Texture.cpp
    \author Christopher Hudson

    \brief
        Defines a class containing data on a texture loaded into vram

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include <bitset>
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
        /*else {
            std::cout << "Loaded texture " << filename << ":" << std::endl;
            std::cout << "  Dim: " << surface->w << "x" << surface->h << std::endl;
            std::cout << "  Pitch: " << surface->pitch << std::endl;
            std::cout << "  Pixel Format:" << std::endl;
            std::cout << "    Pallet: " << surface->format->palette << std::endl;
            std::cout << "    Bits Per Pixel: " << static_cast<int>(surface->format->BitsPerPixel) << std::endl;
            std::cout << "    Bytes Per Pixel: " << static_cast<int>(surface->format->BytesPerPixel) << std::endl;
            std::cout << "    Red Mask:   " << std::bitset<32>(surface->format->Rmask) << std::endl;
            std::cout << "    Green Mask: " << std::bitset<32>(surface->format->Gmask) << std::endl;
            std::cout << "    Blue Mask:  " << std::bitset<32>(surface->format->Bmask) << std::endl;
            std::cout << "    Alpha Mask: " << std::bitset<32>(surface->format->Amask) << std::endl;
        }*/

        GLenum type;
        if(surface->format->Amask == 0) {
            type = GL_RGB;
        }
        else {
            type = GL_RGBA;
        }

        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        glTexImage2D(GL_TEXTURE_2D, 0, type, surface->w, surface->h, 0, type, GL_UNSIGNED_BYTE, surface->pixels);
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
