/*!
    \file   HalfPlane.h
    \author Sean McGeer
    \date   11/09/15
    \brief
        Defines a half plane.
    \see HalfPlane.cpp
*/

#include "Vector.h"

#ifndef HALF_PLANE_H
#define HALF_PLANE_H

namespace ForLeaseEngine {

    class HalfPlane {
        public:
            HalfPlane(Point anchor, Vector normal);
            HalfPlane(Point A, Point B, Point Interior);
            Hcoord GetHcoord();
        private:
            float Dot(const Hcoord& h, const Point& Q);

            Point Anchor;
            Vector Normal;
    };

} // ForLeaseEngine

#endif