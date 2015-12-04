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
        float halfWidth = collision->Width / 2 * transform->ScaleX;
        float halfHeight = collision->Height / 2 * transform->ScaleY;


        Point topLeft(position[0] - halfWidth, position[1] + halfHeight);
        Point topRight(position[0] + halfWidth, position[1] + halfHeight);
        Point botRight(position[0] + halfWidth, position[1] - halfHeight);
        Point botLeft(position[0] - halfWidth, position[1] - halfHeight);

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


        float minDist = 9999;
        Vector normal;
        Components::Collision::Side side;

        HalfPlane::CollisionInterval interval = GetHalfPlaneInterval(top);

        if (interval() && top.Dot(Start) > 0 && (minDist == Unlimited || interval.Start < minDist)) {
            if (Point::InBetween(topLeft, topRight, Start + Direction * Length * interval.Start)) {
                if (interval.Start < minDist) {
                    minDist = interval.Start;
                    normal = top.GetNormal();
                    side = Components::Collision::Side::Top;
                }
            }
        }

        interval = GetHalfPlaneInterval(left);

        if (interval() && left.Dot(Start) > 0 && (minDist == Unlimited || interval.Start < minDist)) {
            if (Point::InBetween(topLeft, botLeft, Start + Direction * Length * interval.Start)) {
                if (interval.Start < minDist) {
                    minDist = interval.Start;
                    normal = left.GetNormal();
                    side = Components::Collision::Side::Left;
                }
            }
        }

        interval = GetHalfPlaneInterval(right);

        if (interval() && right.Dot(Start) > 0 && (minDist == Unlimited || interval.Start < minDist)) {
            if (Point::InBetween(topRight, botRight, Start + Direction * Length * interval.Start)) {
                if (interval.Start < minDist) {
                    minDist = interval.Start;
                    normal = right.GetNormal();
                    side = Components::Collision::Side::Right;
                }
            }
        }

        interval = GetHalfPlaneInterval(bot);

        if (interval() && bot.Dot(Start) > 0 && (minDist == Unlimited || interval.Start < minDist)) {
            if (Point::InBetween(botLeft, botRight, Start + Direction * Length * interval.Start)) {
                if (interval.Start < minDist) {
                    minDist = interval.Start;
                    normal = bot.GetNormal();
                    side = Components::Collision::Side::Bottom;
                }
            }
        }

        if (minDist > 0 && minDist < 2 && Length * minDist < Length) {
            Length = Length * minDist;
            LastNormal = normal;
            LastSide = side;
            LastDistance = minDist;
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

    Point Ray::GetIntersectionPoint() {
        return GetStart() + GetScaledVector();
    }

    Vector Ray::GetLastNormal() {
        return LastNormal;
    }

    Components::Collision::Side Ray::GetLastSide() {
        return LastSide;
    }

    float Ray::GetLastDistance() {
        return LastDistance;
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

    Entity* Ray::CheckCollisions(Ray& ray, std::vector<Entity *>& entities) {
        Entity* colliding = 0;
        float dist = ray.GetScaledVector().Magnitude();

        for (Entity* entity : entities) {
            if (ray.IsColliding(entity)) {
                if (dist > ray.GetScaledVector().Magnitude()) {
                    dist = ray.GetScaledVector().Magnitude();
                    colliding = entity;
                }
            }
        }

        return colliding;
    }

    float Cross(Vector v1, Vector v2) {
        return v1[0]*v2[1] - v1[1]*v2[0];
    }

} // ForLeaseEngine
