/*!
    \file   HalfPlane.cpp
    \author Sean McGeer
    \date   11/09/15
    \brief
        Implements a half-plane.
    \see HalfPlane.h
*/

#include "HalfPlane.h"

namespace ForLeaseEngine {

    HalfPlane::HalfPlane(Point anchor, Vector normal) : Anchor(anchor), Normal(normal) {
        Normal.Normalize();
    }

    HalfPlane::HalfPlane(Point A, Point B, Point Interior) : Anchor(A) {
        Vector AB = B - A;
        Normal = Vector(AB[1], -AB[0]).Normalize;

        if (!(Dot(GetHcoord(), Interior) < 0)) Normal = Vector(-AB[1], AB[0]);
    }

    Hcoord HalfPlane::GetHcoord() {
        Hcoord halfPlane;
        halfPlane[0] = Normal[0];
        halfPlane[1] = Normal[1];
        halfPlane[2] = -(halfPlane[0] * Anchor[0] + halfPlane[1] * Anchor[1]);

        return halfPlane;
    }

    float HalfPlane::Dot(const Hcoord& h, const Point& Q) {
        return (h[0] * Q[0] + h[1] * Q[1] + h[2] * Q[2]);
    }

} // ForLeaseEngine