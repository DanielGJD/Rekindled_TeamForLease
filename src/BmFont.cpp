#include "BmFont.h"

namespace ForLeaseEngine {
    std::istream& operator>>(std::istream& is, BmFont& font) {
        for(int i = 0; i < 3; ++i) {
            char byte;
            is >> byte;
            font.FileID[i] = byte;
        }
        font.FileID[3] = 0;
        is >> font.FileVersion;

        is >> font.Info;
        //is >> font.Common;
        //is >> font.Pages;
        //is >> font.Chars;

        return is;
    }

    std::istream& operator>>(std::istream& is, BmFontInfo& info) {
        //int8_t oneByte;
        //int16_t twoBytes;
        //int32_t fourBytes;
        is.read(reinterpret_cast<char*>(&info.Identifier), sizeof(info.Identifier));
        is.read(reinterpret_cast<char*>(&info.BlockSize), sizeof(info.BlockSize));
        is.read(reinterpret_cast<char*>(&info.FontSize), sizeof(info.FontSize));
        is.read(reinterpret_cast<char*>(&info.BitField), sizeof(info.BitField));
        is.read(reinterpret_cast<char*>(&info.CharSet), sizeof(info.CharSet));
        is.read(reinterpret_cast<char*>(&info.StretchH), sizeof(info.StretchH));
        is.read(reinterpret_cast<char*>(&info.AA), sizeof(info.AA));
        is.read(reinterpret_cast<char*>(&info.PaddingUp), sizeof(info.Identifier));
        is.read(reinterpret_cast<char*>(&info.PaddingRight), sizeof(info.PaddingRight));
        is.read(reinterpret_cast<char*>(&info.PaddingDown), sizeof(info.PaddingDown));
        is.read(reinterpret_cast<char*>(&info.PaddingLeft), sizeof(info.PaddingLeft));
        is.read(reinterpret_cast<char*>(&info.SpacingHoriz), sizeof(info.SpacingHoriz));
        is.read(reinterpret_cast<char*>(&info.SpacingVert), sizeof(info.SpacingVert));
        is.read(reinterpret_cast<char*>(&info.Outline), sizeof(info.Outline));

        is >> info.FontName;

        return is;
    }

    std::istream& operator>>(std::istream& is, BmFontCommon& font) {
        return is;
    }
    std::istream& operator>>(std::istream& is, BmFontPages& font) {
        return is;
    }
    std::istream& operator>>(std::istream& is, BmFontChars& font) {
        return is;
    }
    std::istream& operator>>(std::istream& is, BmFontChar& font) {
        return is;
    }
}
