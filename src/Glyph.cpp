#include "Glyph.h"

namespace ForLeaseEngine {
    Glyph::Glyph(){
    }

    Glyph& Glyph::operator=(Glyph& rhs) {
        ID = rhs.ID;
        Region = rhs.Region;
        Offset = rhs.Offset;
        XAdvance = rhs.XAdvance;
        return *this;
    }
}
