#ifndef GLYPH_H
#define GLYPH_H

namespace ForLeaseEngine {
    struct Glyph {
        Glyph(char id, const TextureRegion& region, Vector& offset, float xAdvance) :
              ID(id), Region(region), Offset(offset), XAdvance(xAdvance);

        char ID;
        TextureRegion Region;
        Vector Offset;
        int XAdvance;
    };
}

#endif // GLYPH_H
