/*!
    \file   Ray.cpp
    \author Sean McGeer
    \date   10/21/15
    \brief
        Implements a ray, as well as some helper functions involved in raycasting.
    \see Ray.h
*/

#include "Ray.h"
#include "Entity.h"
#include "Engine.h"
#include "LevelComponentsInclude.h"
#include "HalfPlane.h"

namespace ForLeaseEngine {

    Ray::Ray(Point start, Vector direction, float scale, int collisions)
    : Start(start), Direction(direction), Scale(scale), Length(scale), Collisions(collisions) {
        Direction.Normalize();
    }

    bool Ray::IsColliding(Entity* entity) {
        if (!entity->HasComponent(ComponentType::Collision)) return false;
        Point endPoint = Start + Direction * Scale;
        Components::Transform* transform = entity->GetComponent<Components::Transform>();
        if (!transform) return false;
        Point position = transform->Position;
        Components::Collision* collision = entity->GetComponent<Components::Collision>();


        Point topLeft(position[0] - collision->Width / 2, position[1] + collision->Height / 2);
        Point topRight(position[0] + collision->Width / 2, position[1] + collision->Height / 2);
        Point botRight(position[0] + collision->Width / 2, position[1] - collision->Height / 2);
        Point botLeft(position[0] - collision->Width / 2, position[1] - collision->Height / 2);

        Vector topVec = topRight - topLeft;
        Vector rightVec = topRight - botRight;
        Vector leftVec = botLeft - topLeft;
        Vector botVec = botLeft - botRight;

        LevelComponents::Renderer* renderer = ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Renderer>();

        renderer->SetDrawingColor(Color(0, 1, 0));
        renderer->DrawRectangleFilled(topLeft, 10, 10, 0);
        renderer->DrawArrow(topLeft, topVec);
        renderer->DrawArrow(topLeft, leftVec);

        renderer->SetDrawingColor(Color(0, 0, 1));
        renderer->DrawRectangleFilled(botRight, 10, 10, 0);
        renderer->DrawArrow(botRight, rightVec);
        renderer->DrawArrow(botRight, botVec);

        
        renderer->SetDrawingColor(Color(1, 1, 1));
        renderer->DrawArrow(Start, Direction * Length);


        float minDist = Unlimited;

        Vector searchVec = Vector::Scale(Direction, Length);

        float dist = LineSegmentsIntersect(Start, searchVec, topLeft, topVec);

        if (minDist < 0 || dist < minDist) minDist = dist;

        dist = LineSegmentsIntersect(Start, searchVec, topLeft, leftVec);

        if (minDist < 0 || dist < minDist) minDist = dist;

        dist = LineSegmentsIntersect(Start, searchVec, botRight, rightVec);

        if (minDist < 0 || dist < minDist) minDist = dist;

        dist = LineSegmentsIntersect(Start, searchVec, botRight, botVec);

        if (minDist < 0 || dist < minDist) minDist = dist;

        if (minDist * Length < Length && minDist != Unlimited) {
            Length = Length * minDist;
            return true;
        }

        return false;
    }

    void Ray::ResetLength() {
        Length = Scale;
    }

    Point Ray::GetStart() {
        return Start;
    }

    Vector Ray::GetScaledVector() {
        if (Length == Unlimited)
            return Direction * 9999;
        else
            return Direction * Length;
    }

    std::vector<Entity *> CheckCollisions(Ray& ray, std::vector<Entity *> entities) {
        std::vector<Entity *> colliding;

        for (Entity* entity : entities)
            if (ray.IsColliding(entity))
                colliding.push_back(entity);

        return colliding;
    }

    float Cross(Vector v1, Vector v2) {
        return v1[0]*v2[1] - v1[1]*v2[0];
    }

    float LineSegmentsIntersect(Point p1, Vector v1, Point p2, Vector v2) {
        float cross = Cross(v1, v2);

        if (cross == 0) return -1;

        float t1 = Cross((p2 - p1), Vector(v2[0]/cross, v2[1]/cross));
        float t2 = Cross((p2 - p1), Vector(v1[0]/cross, v1[1]/cross));

        if (t1 >= 0 && t1 <= 1 && t2 >= 0 && t2 <= 1) {
//            std::cout <<
            return t1;
        }

        return Ray::Unlimited;
    }

} // ForLeaseEngine
