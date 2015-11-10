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

        HalfPlane top(topLeft, topRight, position);
        HalfPlane right(topRight, botRight, position);
        HalfPlane left(topLeft, botLeft, position);
        HalfPlane bot(botLeft, botRight, position);


        ////////// DEBUG DRAWING //////////
        LevelComponents::Renderer* renderer = ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Renderer>();

        renderer->SetDrawingColor(Color(0, 0.5, 1));
        renderer->DrawRectangleFilled(top.GetAnchor(), 10, 10, 0);
        renderer->DrawArrow(top.GetAnchor(), top.GetNormal() * 30);

        renderer->SetDrawingColor(Color(0, 1, 0.5));
        renderer->DrawRectangleFilled(right.GetAnchor(), 10, 10, 0);
        renderer->DrawArrow(right.GetAnchor(), right.GetNormal() * 30);

        renderer->SetDrawingColor(Color(0, 1, 1));
        renderer->DrawRectangleFilled(left.GetAnchor(), 10, 10, 0);
        renderer->DrawArrow(left.GetAnchor(), left.GetNormal() * 30);

        renderer->SetDrawingColor(Color(0, 0.5, 0.5));
        renderer->DrawRectangleFilled(bot.GetAnchor(), 10, 10, 0);
        renderer->DrawArrow(bot.GetAnchor(), bot.GetNormal() * 30);
        ////////// DEBUG DRAWING //////////


        float minDist = Unlimited;

        Vector searchVec = Vector::Scale(Direction, Length);

        HalfPlane::CollisionInterval interval = GetHalfPlaneInterval(top);

        if (interval() && (minDist == Unlimited || interval.End < minDist))
            minDist = interval.End;

        interval = GetHalfPlaneInterval(left);

        if (interval() && (minDist == Unlimited || interval.End < minDist))
            minDist = interval.End;

        interval = GetHalfPlaneInterval(right);

        if (interval() && (minDist == Unlimited || interval.End < minDist))
            minDist = interval.End;

        interval = GetHalfPlaneInterval(bot);

        if (interval() && (minDist == Unlimited || interval.End < minDist))
            minDist = interval.End;

        if (minDist * Length < Scale && minDist != Unlimited) {
            //std::cout << "         " << minDist << std::endl;
            Length = Scale * minDist;
            std::cout << Length << std::endl;
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

    HalfPlane::CollisionInterval Ray::GetHalfPlaneInterval(const HalfPlane& halfPlane) {
        float dotStart = halfPlane.Dot(Start);
        float dotEnd = halfPlane.Dot(Start + Direction * Length);

        if (dotStart < 0 && dotEnd < 0) return HalfPlane::CollisionInterval(0, 1);
        if (dotStart > 0 && dotEnd > 0) return HalfPlane::CollisionInterval(1, 0);

        float ti = dotStart / (dotStart - dotEnd);

        if (dotStart > 0 && dotEnd <= 0) return HalfPlane::CollisionInterval(ti, 1);
        else return HalfPlane::CollisionInterval(0, ti);
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

//    float LineSegmentsIntersect(Point p1, Vector v1, Point p2, Vector v2) {
//        float cross = Cross(v1, v2);
//
//        if (cross == 0) return -1;
//
//        float t1 = Cross((p2 - p1), Vector(v2[0]/cross, v2[1]/cross));
//        float t2 = Cross((p2 - p1), Vector(v1[0]/cross, v1[1]/cross));
//
//        if (t1 >= 0 && t1 <= 1 && t2 >= 0 && t2 <= 1) {
////            std::cout <<
//            return t1;
//        }
//
//        return Ray::Unlimited;
//    }

} // ForLeaseEngine
