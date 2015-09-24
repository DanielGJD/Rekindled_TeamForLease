#ifndef TEXTUREREGION_H
#define TEXTUREREGION_H

#include "Texture.h"
#include "Vector.h"

namespace ForLeaseEngine {
    class TextureRegion {
        public:
            TextureRegion(Texture* texture, int left, int right, int top, int bottom);
            GLuint GetTextureID();
            int GetWidth();
            int GetHeight();
            Point* GetUV();
        private:
            //Texture* Source;
            GLuint TextureID;
            Point UV[4];
            int Width;
            int Height;
    };
}

#endif // TEXTUREREGION_H
