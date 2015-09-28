#ifndef FONT_H
#define FONT_H

#include <string>
#include <GL\gl.h>
#include "Glyph.h"
#include "BmFont.h"
#include "TextureRegion.h"

namespace ForLeaseEngine {
    class Font {
        public:
            Font(BmFont& font);
            ~Font();
            TextureRegion* GetLetter(char letter);
            Texture* GetTexture(int texture);
        private:
            Texture** FontTextures;
            int NumTextures;
            int LineHeight;
            int Base;
            Glyph Glyphs[256];
    };
}

#endif // FONT_H
