#include "BmFont.h"

namespace ForLeaseEngine {
    BmFontPages::BmFontPages() {
        PageNames = NULL;
    }

    BmFontPages::~BmFontPages() {
        if(PageNames != NULL) {
            delete[] PageNames;
        }
    }

    BmFontChars::BmFontChars() {
        Chars = NULL;
    }

    BmFontChars::~BmFontChars() {
        if(Chars != NULL) {
            delete[] Chars;
        }
    }

    std::istream& operator>>(std::istream& is, BmFont& font) {
        for(int i = 0; i < 3; ++i) {
            char byte;
            is >> byte;
            font.FileID[i] = byte;
        }
        font.FileID[3] = 0;
        is >> font.FileVersion;

        is >> font.Info;
        is >> font.Common;
        is >> font.Pages;
        is >> font.Chars;

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

        //is >> info.FontName;
        char c;
        is >> c;
        while(c != 0) {
            info.FontName.push_back(c);
            is >> c;
        }

        return is;
    }

    std::istream& operator>>(std::istream& is, BmFontCommon& common) {
        is.read(reinterpret_cast<char*>(&common.Identifier), sizeof(common.Identifier));
        is.read(reinterpret_cast<char*>(&common.BlockSize), sizeof(common.BlockSize));
        is.read(reinterpret_cast<char*>(&common.LineHeight), sizeof(common.LineHeight));
        is.read(reinterpret_cast<char*>(&common.Base), sizeof(common.Base));
        is.read(reinterpret_cast<char*>(&common.ScaleW), sizeof(common.ScaleW));
        is.read(reinterpret_cast<char*>(&common.ScaleH), sizeof(common.ScaleH));
        is.read(reinterpret_cast<char*>(&common.Pages), sizeof(common.Pages));
        is.read(reinterpret_cast<char*>(&common.BitField), sizeof(common.BitField));
        is.read(reinterpret_cast<char*>(&common.AlphaChnl), sizeof(common.AlphaChnl));
        is.read(reinterpret_cast<char*>(&common.RedChnl), sizeof(common.RedChnl));
        is.read(reinterpret_cast<char*>(&common.GreenChnl), sizeof(common.GreenChnl));
        is.read(reinterpret_cast<char*>(&common.BlueChnl), sizeof(common.BlueChnl));

        return is;
    }

    std::istream& operator>>(std::istream& is, BmFontPages& pages) {
        is.read(reinterpret_cast<char*>(&pages.Identifier), sizeof(pages.Identifier));
        is.read(reinterpret_cast<char*>(&pages.BlockSize), sizeof(pages.BlockSize));

        std::string firstPage;
        char c;
        is >> c;
        while(c != 0) {
            firstPage.push_back(c);
            is >> c;
        }
        int numPages = pages.BlockSize / (firstPage.size() + 1);
        pages.PageNames = new std::string[numPages];
        pages.PageNames[0] = firstPage;
        for(int i = 1; i < numPages; ++i) {
            char c;
            is >> c;
            while(c != 0) {
                pages.PageNames[i].push_back(c);
                is >> c;
            }
        }

//        for(int i = 0; i < numPages; ++i) {
//            std::cout << pages.PageNames[i] << ", ";
//        }
//        std::cout << std::endl;
        return is;
    }
    std::istream& operator>>(std::istream& is, BmFontChars& chars) {
        is.read(reinterpret_cast<char*>(&chars.Identifier), sizeof(chars.Identifier));
        is.read(reinterpret_cast<char*>(&chars.BlockSize), sizeof(chars.BlockSize));
        int numChars = chars.BlockSize / sizeof(BmFontCharInfo);
        chars.Chars = new BmFontCharInfo[numChars];
        for(int i = 0; i < numChars; ++i) {
            is >> chars.Chars[i];
        }
        return is;
    }
    std::istream& operator>>(std::istream& is, BmFontCharInfo& info) {
        is.read(reinterpret_cast<char*>(&info.ID), sizeof(info.ID));
        is.read(reinterpret_cast<char*>(&info.X), sizeof(info.X));
        is.read(reinterpret_cast<char*>(&info.Y), sizeof(info.Y));
        is.read(reinterpret_cast<char*>(&info.Width), sizeof(info.Width));
        is.read(reinterpret_cast<char*>(&info.Height), sizeof(info.Height));
        is.read(reinterpret_cast<char*>(&info.XOffset), sizeof(info.XOffset));
        is.read(reinterpret_cast<char*>(&info.YOffset), sizeof(info.YOffset));
        is.read(reinterpret_cast<char*>(&info.XAdvance), sizeof(info.XAdvance));
        is.read(reinterpret_cast<char*>(&info.Page), sizeof(info.Page));
        is.read(reinterpret_cast<char*>(&info.Chnl), sizeof(info.Chnl));
        return is;
    }
}
