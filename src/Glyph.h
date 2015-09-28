#ifndef GLYPH_H
#define GLYPH_H

#include "TextureRegion.h"
#include "Vector.h"

namespace ForLeaseEngine {
    struct Glyph {
        Glyph();
        Glyph(char id, TextureRegion region, Vector& offset, int xAdvance, int width, int height) :
              ID(id), Region(region), Offset(offset), XAdvance(xAdvance) {}

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
