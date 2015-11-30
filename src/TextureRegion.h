/*!
    \file   TextureRegion.h
    \author Christopher Hudson

    \brief
        Defines a class containing information about a portion of a texture

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef TEXTUREREGION_H
#define TEXTUREREGION_H

#include "Texture.h"
#include "Vector.h"
#include "Serializable.h"
#include "Serialize.h"

namespace ForLeaseEngine {
    class TextureRegion  : public Serializable {
        public:
            TextureRegion();
            TextureRegion(Texture* texture, int left, int right, int top, int bottom);

            void Serialize(Serializer& root);
            void Deserialize(Serializer& root);

            GLuint GetTextureID();
            std::string& GetTexture();
            int GetWidth();
            int GetHeight();
            Point* GetUV();

        private:
            std::string Source;
            GLuint TextureID;
            Point UV[4];
            int Width;
            int Height;
    };
}

#endif // TEXTUREREGION_H
