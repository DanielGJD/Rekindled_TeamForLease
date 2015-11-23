/*!
    \file   Ray.cpp
    \author Sean McGeer
    \date   10/21/15
    \brief
        Implements a ray, as well as some helper functions involved in raycasting.
    \see Ray.h

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "Ray.h"
#include "Entity.h"
#include "Engine.h"
#include "LevelComponentsInclude.h"

int i = 0;

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

        if (right.Dot(Start) < 0 && bot.Dot(Start) < 0 && left.Dot(Start) < 0 && top.Dot(Start) < 0) {
            Length = 0;
            return true;
        }


        //////////// DEBUG DRAWING //////////
        //LevelComponents::Renderer* renderer = ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Renderer>();

        //renderer->SetDrawingColor(Color(0, 0.5, 1));
        //renderer->DrawRectangleFilled(top.GetAnchor(), 10, 10, 0);
        //renderer->DrawArrow(top.GetAnchor(), top.GetNormal() * 30);
        ////renderer->DrawLine(top.GetAnchor() - Vector::Rotate(top.GetNormal(), 1.5708) * 2000, Vector::Rotate(top.GetNormal(), 1.5708) * 4000);
        //renderer->DrawLine(topLeft, topRight);

        //renderer->SetDrawingColor(Color(0, 1, 0.5));
        //renderer->DrawRectangleFilled(right.GetAnchor(), 10, 10, 0);
        //renderer->DrawArrow(right.GetAnchor(), right.GetNormal() * 30);
        ////renderer->DrawLine(right.GetAnchor() - Vector::Rotate(right.GetNormal(), 1.5708) * 2000, Vector::Rotate(right.GetNormal(), 1.5708) * 4000);
        //renderer->DrawLine(topRight, botRight);

        //renderer->SetDrawingColor(Color(0, 1, 1));
        //renderer->DrawRectangleFilled(left.GetAnchor(), 10, 10, 0);
        //renderer->DrawArrow(left.GetAnchor(), left.GetNormal() * 30);
        ////renderer->DrawLine(left.GetAnchor() - Vector::Rotate(left.GetNormal(), 1.5708) * 2000, Vector::Rotate(left.GetNormal(), 1.5708) * 4000);
        //renderer->DrawLine(topLeft, botLeft);

        //renderer->SetDrawingColor(Color(0, 0.5, 0.5));
        //renderer->DrawRectangleFilled(bot.GetAnchor(), 10, 10, 0);
        //renderer->DrawArrow(bot.GetAnchor(), bot.GetNormal() * 30);
        ////renderer->DrawLine(bot.GetAnchor() - Vector::Rotate(bot.GetNormal(), 1.5708) * 2000, Vector::Rotate(bot.GetNormal(), 1.5708) * 4000);
        //renderer->DrawLine(botLeft, botRight);
        //////////// DEBUG DRAWING //////////


        float minDist = Unlimited;

        Vector searchVec = Vector::Scale(Direction, Length);

        HalfPlane::CollisionInterval interval = GetHalfPlaneInterval(top);

        if (interval() && top.Dot(Start) > 0 && (minDist == Unlimited || interval.Start < minDist))
            if (Point::InBetween(topLeft, topRight, Start + Direction * Scale * interval.Start))
                minDist = interval.Start;

        interval = GetHalfPlaneInterval(left);

        if (interval() && left.Dot(Start) > 0 && (minDist == Unlimited || interval.Start < minDist))
            if (Point::InBetween(topLeft, botLeft, Start + Direction * Scale * interval.Start))
                minDist = interval.Start;

        interval = GetHalfPlaneInterval(right);

        if (interval() && right.Dot(Start) > 0 && (minDist == Unlimited || interval.Start < minDist))
            if (Point::InBetween(topRight, botRight, Start + Direction * Scale * interval.Start))
                minDist = interval.Start;

        interval = GetHalfPlaneInterval(bot);

        if (interval() && bot.Dot(Start) > 0 && (minDist == Unlimited || interval.Start < minDist))
            if (Point::InBetween(botLeft, botRight, Start + Direction * Scale * interval.Start))
                minDist = interval.Start;

        if (minDist > 0 && Scale * minDist < Length) {
            Length = Scale * minDist;
            return true;
        }
        else {
            //Length = Scale;
            return false;
        }
    }

    void Ray::ResetLength() {
        Length = Scale;
    }

    Point Ray::GetStart() {
        return Start;
    }

    Vector Ray::GetScaledVector() {
        if (Length < 0)
            if (Scale < 0)
                return Direction * 99999;
            else
                return Direction * Scale;
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

} // ForLeaseEngine
