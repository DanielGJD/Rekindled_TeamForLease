#include "Vector.h"
#include <vector>

#ifndef RAY_H
#define RAY_H

namespace ForLeaseEngine {

  class Entity;

  class Ray {
    public:
      Ray(Point start = Point(0,0), Vector direction = Vector(1,1), float scale = 1, int collisions = 1);
      bool IsColliding(Entity* entity);
    private:
      Point Start;
      Vector Direction;
      float Scale;
      int Collisions;
  };

  std::vector<Entity *> CheckCollisions(Ray& ray, std::vector<Entity *> entities);

} // ForLeaseEngine

#endif // RAY_H