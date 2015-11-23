/*!
    \file   Glyph.h
    \author Christopher Hudson

    \brief
        Defines a class containing information about a glyph in a font

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef GLYPH_H
#define GLYPH_H

#include "TextureRegion.h"
#include "Vector.h"

namespace ForLeaseEngine {
    struct Glyph {
        Glyph();
        Glyph(char id, TextureRegion region, Vector& offset, int xAdvance, int width, int height) :
              ID(id), Region(region), Offset(offset), XAdvance(xAdvance), Width(width), Height(height) {}

        char ID;
        TextureRegion Region;
        Vector Offset;
        int XAdvance;
        float Width;
        float Height;

        Glyph& operator=(Glyph& rhs);
    };
}

#endif // GLYPH_H
