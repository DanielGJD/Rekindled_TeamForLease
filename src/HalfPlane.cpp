/*!
    \file   HalfPlane.cpp
    \author Sean McGeer
    \date   11/09/15
    \brief
        Implements a half-plane.
    \see HalfPlane.h

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "HalfPlane.h"

namespace ForLeaseEngine {

    HalfPlane::CollisionInterval::CollisionInterval(float a, float b) : Start(a), End(b) {};

    bool HalfPlane::CollisionInterval::operator() () const {
        return End >= Start;
    }

    HalfPlane::HalfPlane(Point anchor, Vector normal) : Anchor(anchor), Normal(normal) {
        Normal.Normalize();
    }

    HalfPlane::HalfPlane(Point A, Point B, Point Interior) {
        Vector AB = B - A;
        Anchor = A + Vector::Scale(AB, 0.5);
        Normal = Vector(AB[1], -AB[0]);
        Normal.Normalize();

        if (!(Dot(GetHcoord(), Interior) < 0)) {
            Normal = Vector(-AB[1], AB[0]);
            Normal.Normalize();
        }
    }

    Hcoord HalfPlane::GetHcoord() const {
        Hcoord halfPlane;
        halfPlane[0] = Normal[0];
        halfPlane[1] = Normal[1];
        halfPlane[2] = -(halfPlane[0] * Anchor[0] + halfPlane[1] * Anchor[1]);

        return halfPlane;
    }

    Point HalfPlane::GetAnchor() const {
        return Anchor;
    }

    Vector HalfPlane::GetNormal() const {
        return Normal;
    }

    float HalfPlane::Dot(const Point& Q) const {
        return Dot(GetHcoord(), Q);
    }

    float HalfPlane::Dot(const Hcoord& h, const Point& Q) const {
        return (h[0] * Q[0] + h[1] * Q[1] + h[2] * Q[2]);
    }

} // ForLeaseEngine
