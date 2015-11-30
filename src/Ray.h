/*!
    \file   Ray.h
    \author Sean McGeer
    \date   10/21/15
    \brief
        Defines a ray, as well as some helper functions involved in raycasting.
    \see Ray.cpp
*/

#include "Vector.h"
#include "HalfPlane.h"
#include "ComponentCollision.h"
#include <vector>

#ifndef RAY_H
#define RAY_H

namespace ForLeaseEngine {

    class Entity;

    class Ray {
        public:
            const static int Unlimited = -1;
            Ray(Point start = Point(0,0), Vector direction = Vector(1,1), float scale = Unlimited, int collisions = Unlimited);
            bool IsColliding(Entity* entity);
            std::vector<Point> GetCollisionPoints();
            void ResetLength();
            Point GetStart();
            Vector GetScaledVector();
            Vector GetLastNormal();
            float GetLastDistance();
            Components::Collision::Side GetLastSide();
            HalfPlane::CollisionInterval GetHalfPlaneInterval(const HalfPlane& halfPlane);
        private:
            Point Start;
            Vector Direction;
            const float Scale;
            float Length;
            int Collisions;
            Vector LastNormal;
            Components::Collision::Side LastSide;
            float LastDistance;
    };

    std::vector<Entity *> CheckCollisions(const Ray& ray, std::vector<Entity *> entities);

} // ForLeaseEngine

#endif // RAY_H
