/*!
    \file   Font.h
    \author Christopher Hudson

    \brief
        Defines a class containing information about a font

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef FONT_H
#define FONT_H

#include <string>
#include <GL/gl.h>
#include "Glyph.h"
#include "BmFont.h"
#include "TextureRegion.h"

namespace ForLeaseEngine {
    class Font {
        public:
            Font(BmFont& font);
            ~Font();
            TextureRegion* GetLetter(char letter);
            const Glyph& GetGlyph(char letter);
            std::string GetTexture(int texture);
        //private:
            std::string* FontTextures;
            int NumTextures;
            int LineHeight;
            int Base;
            Glyph Glyphs[256];
    };
}

#endif // FONT_H
