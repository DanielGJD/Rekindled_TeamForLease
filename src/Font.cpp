#include "Font.h"
#include "BmFont.h"
#include <iostream>

namespace ForLeaseEngine {
    Font::Font(BmFont& font) {
        FontTextures = new Texture*[font.Common.Pages];
        for(int i = 0; i < font.Common.Pages; ++i)
        {
            FontTextures[i] = Texture::CreateTexture(font.Pages.PageNames[i]);
        }
        int numChars = font.Chars.BlockSize / sizeof(BmFontCharInfo);
        for(int i = 0; i < numChars; ++i) {
            BmFontCharInfo info = font.Chars.Chars[i];
            Glyphs[i].ID = info.ID;
            int left = info.X;
            int right = info.X + info.Width;
            int top = info.Y;
            int bottom = info.Y - info.Height;
            Glyphs[i].Region = TextureRegion(*(FontTextures + info.Page), left, right, top, bottom);
            Glyphs[i].Offset = Vector(info.XOffset, info.YOffset);
            Glyphs[i].XAdvance = info.XAdvance;
        }
        LineHeight = font.Common.LineHeight;
        Base = font.Common.Base;
        NumTextures = font.Common.Pages;
    }

    Font::~Font() {
        for(int i = 0; i < NumTextures; ++i) {
            Texture::DeleteTexture(FontTextures[i]);
            delete[] FontTextures;
        }
    }

    TextureRegion* Font::GetLetter(char letter) {
        return &(Glyphs[letter].Region);
    }

    Texture* Font::GetTexture(int texture) {
        return FontTextures[texture];
    }
}
