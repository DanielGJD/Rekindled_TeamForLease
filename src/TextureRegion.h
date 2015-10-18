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
