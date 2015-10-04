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
            int ID = static_cast<int>(info.ID);
            Glyphs[ID].ID = info.ID;
            int left = info.X;
            int right = info.X + info.Width;
            int top = info.Y;
            int bottom = info.Y + info.Height;
            Glyphs[ID].Region = TextureRegion(*(FontTextures + info.Page), left, right, top, bottom);
            Glyphs[ID].Offset = Vector(info.XOffset, info.YOffset);
            Glyphs[ID].XAdvance = info.XAdvance;
            Glyphs[ID].Width = info.Width;
            Glyphs[ID].Height = info.Height;
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
        return &(Glyphs[static_cast<int>(letter)].Region);
    }

    const Glyph& Font::GetGlyph(char letter) {
        return Glyphs[static_cast<int>(letter)];
    }

    Texture* Font::GetTexture(int texture) {
        return FontTextures[texture];
    }
}