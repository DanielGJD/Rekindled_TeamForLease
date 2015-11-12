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
            struct CollisionInterval {
                float Start, End;
                CollisionInterval(float a = 0, float b = 1);
                bool operator() () const;
            };
            HalfPlane(Point anchor, Vector normal);
            HalfPlane(Point A, Point B, Point Interior);
            Hcoord GetHcoord() const;
            Point GetAnchor() const;
            Vector GetNormal() const;
            float Dot(const Point& Q) const;
        private:
            float Dot(const Hcoord& h, const Point& Q) const;

            Point Anchor;
            Vector Normal;
    };

} // ForLeaseEngine

#endif
