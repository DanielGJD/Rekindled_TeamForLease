#ifndef GLYPH_H
#define GLYPH_H

#include "TextureRegion.h"
#include "Vector.h"

namespace ForLeaseEngine {
    struct Glyph {
        Glyph();
        Glyph(char id, TextureRegion region, Vector& offset, float xAdvance) :
              ID(id), Region(region), Offset(offset), XAdvance(xAdvance) {}

        char ID;
        TextureRegion Region;
        Vector Offset;
        int XAdvance;

        Glyph& operator=(Glyph& rhs);
    };
}

#endif // GLYPH_H
