#include "Ray.h"
#include "Entity.h"

namespace ForLeaseEngine {

    Ray::Ray(Point start, Vector direction)
    : Start(start), Direction(direction) {}

    bool Ray::IsColliding(Entity* entity) {
        Point endPoint = Start + Direction;
        Components::Transform* transform = entity->GetComponent<Components::Transform*>(ComponentType::Transform);
        if (!transform) return false;

        // Super simple bounding box collision detection
        // Testing only--will be replaced with a line-detecting algorithm
        if (endPoint[0] > transform->Position[0] - transform->ScaleX/2 &&
            endPoint[0] < transform->Position[0] + transform->ScaleX/2 &&
            endPoint[1] > transform->Position[1] - transform->ScaleY/2 &&
            endPoint[1] < transform->Position[1] + transform->ScaleY/2)
                return true;

        return false;
    }

    std::vector<Entity *> CheckCollisions(Ray& ray, std::vector<Entity *> entities) {
        std::vector<Entity *> colliding;

        for (Entity* entity : entities)
            if (ray.IsColliding(entity))
                colliding.push_back(entity);

        return colliding;
    }

} // ForLeaseEngine