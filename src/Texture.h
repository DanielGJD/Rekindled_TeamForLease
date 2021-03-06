/*!
    \file   Texture.h
    \author Christopher Hudson

    \brief
        Defines a class containing information on a texture loaded into vram

    \copyright ęCopyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef TEXTURE_H
#define TEXTURE_H

#include <GL/gl.h>
#include <string>

namespace ForLeaseEngine{
    class Texture {
        public:
            static Texture* CreateTexture(std::string filename, std::string name);
            static void DeleteTexture(Texture* texture);
            GLuint GetID();
            const std::string& GetName();
            int GetHeight();
            int GetWidth();
        private:
            GLuint ID;
            std::string Name;
            int Width;
            int Height;

            Texture(const GLuint id, const std::string name, const int width, const int height);
    };

}

#endif // TEXTURE_H
