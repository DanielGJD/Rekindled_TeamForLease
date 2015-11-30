/*!
    \file   BmFont.h
    \author Christopher Hudson

    \brief
        Defines a class for loading and storing data for a bitmap font

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef BMFONT_H
#define BMFONT_H

#include <cstdint>
#include <iostream>

namespace ForLeaseEngine {
    #define BM_FONT_CHAR_BLOCK_SIZE 20

    struct BmFontInfo {
        uint8_t     Identifier;
        uint32_t    BlockSize;
        int16_t     FontSize;
        int8_t      BitField;
        uint8_t     CharSet;
        uint16_t    StretchH;
        uint8_t     AA;
        uint8_t     PaddingUp;
        uint8_t     PaddingRight;
        uint8_t     PaddingDown;
        uint8_t     PaddingLeft;
        uint8_t     SpacingHoriz;
        uint8_t     SpacingVert;
        uint8_t     Outline;
        std::string FontName;
    };

    struct BmFontCommon {
        uint8_t  Identifier;
        uint32_t BlockSize;
        uint16_t LineHeight;
        uint16_t Base;
        uint16_t ScaleW;
        uint16_t ScaleH;
        uint16_t Pages;
        int8_t   BitField;
        uint8_t  AlphaChnl;
        uint8_t  RedChnl;
        uint8_t  GreenChnl;
        uint8_t  BlueChnl;
    };

    struct BmFontPages {
        uint8_t      Identifier;
        uint32_t     BlockSize;
        std::string* PageNames;

        BmFontPages();
        ~BmFontPages();
    };

    struct BmFontCharInfo {
        uint32_t ID;
        uint16_t X;
        uint16_t Y;
        uint16_t Width;
        uint16_t Height;
        int16_t  XOffset;
        int16_t  YOffset;
        int16_t  XAdvance;
        uint8_t  Page;
        uint8_t  Chnl;
    };

    struct BmFontChars {
        uint8_t     Identifier;
        uint32_t    BlockSize;
        BmFontCharInfo* Chars;

        BmFontChars();
        ~BmFontChars();
    };

    struct BmFont {
        char         FileID[4];
        uint8_t      FileVersion;
        BmFontInfo   Info;
        BmFontCommon Common;
        BmFontPages  Pages;
        BmFontChars  Chars;

    };

    std::istream& operator>>(std::istream& is, BmFont& font);
    std::istream& operator>>(std::istream& is, BmFontInfo& font);
    std::istream& operator>>(std::istream& is, BmFontCommon& font);
    std::istream& operator>>(std::istream& is, BmFontPages& font);
    std::istream& operator>>(std::istream& is, BmFontChars& font);
    std::istream& operator>>(std::istream& is, BmFontCharInfo& font);
}

#endif // BMFONT_H
