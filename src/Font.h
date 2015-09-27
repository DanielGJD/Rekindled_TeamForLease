#ifndef FONT_H
#define FONT_H

#include <string>
#include <GL\gl.h>
#include "Glyph.h"

namespace ForLeaseEngine {
    class Font {
        public:
            Font(std::string fontName);
            ~Font();
        private:
            std::string Name;
            GLuint TextureID;
            Texture* FontTexture;
            int LineHeight;
            Glyph Glyphs[256];
    };
}

#endif // FONT_H
