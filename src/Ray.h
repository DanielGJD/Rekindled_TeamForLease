#include "Vector.h"
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
        private:
            Point Start;
            Vector Direction;
            float Scale;
            int Collisions;
    };

    std::vector<Entity *> CheckCollisions(const Ray& ray, std::vector<Entity *> entities);

} // ForLeaseEngine

#endif // RAY_H
