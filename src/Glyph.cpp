/*!
    \file   Glyph.cpp
    \author Christopher Hudson

    \brief
        Defines a class representing a glyph in a bitmap font

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

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
