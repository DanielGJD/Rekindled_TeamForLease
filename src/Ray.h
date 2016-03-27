/*!
    \file   Ray.h
    \author Sean McGeer
    \date   10/21/15
    \brief
        Defines a ray, as well as some helper functions involved in raycasting.
    \see Ray.cpp

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
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
            const static unsigned Unlimited = static_cast<unsigned>(-1);
            Ray(Point start = Point(0,0), Vector direction = Vector(1,1), float scale = 20, int collisions = 1);
            bool IsColliding(Entity* entity);
            //std::vector<Point> GetCollisionPoints();
            void ResetLength();
            Point GetStart();
            Vector GetScaledVector();
            Vector GetLastNormal();
            float GetLastDistance();
            CollisionSide GetLastSide();
            Point GetIntersectionPoint();
            HalfPlane::CollisionInterval GetHalfPlaneInterval(const HalfPlane& halfPlane);
            static Entity* CheckCollisions(Ray& ray, std::vector<Entity *>& entities, Entity* ignore = 0);

            class Collision {
                public:
                    Collision(float distance = 0, Entity* entity = 0, Point point = ForLeaseEngine::Point())
                        : Distance(distance), Entity(entity), Point(point) {}
                    float Distance;
                    ForLeaseEngine::Entity* Entity;
                    ForLeaseEngine::Point Point;
            };

            static std::vector<Collision> CheckCollisionsMultipleEntities(Ray& ray, std::vector<Entity *>& entities, Entity* ignore = 0);
        private:
            Point Start;
            Vector Direction;
            const float Scale;
            float Length;
            int Collisions;
            Vector LastNormal;
            CollisionSide LastSide;
            float LastDistance;
    };

    //Entity* CheckCollisions(const Ray& ray, std::vector<Entity *>& entities);

} // ForLeaseEngine

#endif // RAY_H
