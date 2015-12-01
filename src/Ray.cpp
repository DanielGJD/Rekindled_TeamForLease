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
    static float DotPointVector(Point const& p, Vector const& v) {
        return p[0] * v[0] + p[1] * v[1];
    }

    Ray::Ray(Point start, Vector direction, float scale, int collisions)
    : Start(start), Direction(direction), Scale(scale), Length(scale), Collisions(collisions) {
        Direction.Normalize();
    }

    /*bool Ray::IsColliding(Entity* entity) {
        if (!entity->HasComponent(ComponentType::Collision)) return false;
        Point endPoint = Start + Direction * Scale;
        Components::Transform* transform = entity->GetComponent<Components::Transform>();
        //if (!transform) return false;
        Components::Collision* collision = entity->GetComponent<Components::Collision>();
        Point position = transform->Position; // add offset in here

        float halfwidth = collision->Width / 2 * transform->ScaleX;
        float halfheight = collision->Height / 2 * transform->ScaleY;

        Point topLeft(position[0] - halfwidth, position[1] + halfheight);
        Point topRight(position[0] + halfwidth, position[1] + halfheight);
        Point botRight(position[0] + halfwidth, position[1] - halfheight);
        Point botLeft(position[0] - halfwidth, position[1] - halfheight);

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
    }*/

    // calculates the intersection time of the ray and an entity
    float Ray::IsColliding(Entity* entity) {
        if (!entity->HasComponent(ComponentType::Collision)) return -1;
        Point endPoint = Start + Direction * Length;
        Components::Transform* transform = entity->GetComponent<Components::Transform>();
        //if (!transform) return false;
        Components::Collision* collision = entity->GetComponent<Components::Collision>();
        Point position = transform->Position; // add offset in here

        float halfwidth = collision->Width / 2 * transform->ScaleX;
        float halfheight = collision->Height / 2 * transform->ScaleY;

        Point topLeft(position[0] - halfwidth, position[1] + halfheight);
        Point topRight(position[0] + halfwidth, position[1] + halfheight);
        Point botRight(position[0] + halfwidth, position[1] - halfheight);
        Point botLeft(position[0] - halfwidth, position[1] - halfheight);

        Vector topNorm = Vector(topLeft[1] - topRight[1], -(topLeft[0] - topRight[0]));
        Vector botNorm = Vector(botLeft[1] - botRight[1], -(botLeft[0] - botRight[0]));
        Vector leftNorm = Vector(topLeft[1] - botLeft[1], -(topLeft[0] - botLeft[0]));
        Vector rightNorm = Vector(topRight[1] - botRight[1], -(topRight[0] - botRight[0]));
        topNorm.Normalize();
        botNorm.Normalize();
        rightNorm.Normalize();
        leftNorm.Normalize();

        float closest_t = -1;

        // Detect for top
        float segmentStartDotNorm = DotPointVector(topRight, topNorm);
        float distanceRayStart = DotPointVector(Start, topNorm) - segmentStartDotNorm;
        float distanceRayEnd = DotPointVector(endPoint, topNorm) - segmentStartDotNorm;

        // Ensure start and end point of ray are on opposite sides of line
        if(distanceRayStart * distanceRayEnd <= 0) {
            // Calculate intersection time
            Vector rayVector = endPoint - Start;
            float t = (segmentStartDotNorm - DotPointVector(Start, topNorm)) / Vector::DotProduct(rayVector, topNorm);

            // Ensure ray actually hit the line
            if(t >=0 && t <= 1) {
                // Calculate intersection point
                Point intersectPoint = Start + rayVector * t;

                // Ensure intersection point is on the line segment
                Vector lineStartToIntersection = intersectPoint - topRight;
                Vector lineEndToIntersection = intersectPoint - topLeft;

                if(Vector::DotProduct(lineStartToIntersection, lineEndToIntersection) <= 0) {
                    // Set store t if it's the closest calculated so far
                    if(closest_t < 0 || t < closest_t) {
                        closest_t = t;
                    }
                }
            }
        }

        // Detection for bottom
        segmentStartDotNorm = DotPointVector(botRight, botNorm);
        distanceRayStart = DotPointVector(Start, botNorm) - segmentStartDotNorm;
        distanceRayEnd = DotPointVector(endPoint, botNorm) - segmentStartDotNorm;

        // Ensure start and end point of ray are on opposite sides of line
        if(distanceRayStart * distanceRayEnd <= 0) {
            // Calculate intersection time
            Vector rayVector = endPoint - Start;
            float t = (segmentStartDotNorm - DotPointVector(Start, botNorm)) / Vector::DotProduct(rayVector, botNorm);

            // Ensure ray actually hit the line
            if(t >=0 && t <= 1) {
                // Calculate intersection point
                Point intersectPoint = Start + rayVector * t;

                // Ensure intersection point is on the line segment
                Vector lineStartToIntersection = intersectPoint - botRight;
                Vector lineEndToIntersection = intersectPoint - botLeft;

                if(Vector::DotProduct(lineStartToIntersection, lineEndToIntersection) <= 0) {
                    // Set store t if it's the closest calculated so far
                    if(closest_t < 0 || t < closest_t) {
                        closest_t = t;
                    }
                }
            }
        }

        // Detection for left
        segmentStartDotNorm = DotPointVector(botLeft, leftNorm);
        distanceRayStart = DotPointVector(Start, leftNorm) - segmentStartDotNorm;
        distanceRayEnd = DotPointVector(endPoint, leftNorm) - segmentStartDotNorm;

        // Ensure start and end point of ray are on opposite sides of line
        if(distanceRayStart * distanceRayEnd <= 0) {
            // Calculate intersection time
            Vector rayVector = endPoint - Start;
            float t = (segmentStartDotNorm - DotPointVector(Start, leftNorm)) / Vector::DotProduct(rayVector, leftNorm);

            // Ensure ray actually hit the line
            if(t >=0 && t <= 1) {
                // Calculate intersection point
                Point intersectPoint = Start + rayVector * t;

                // Ensure intersection point is on the line segment
                Vector lineStartToIntersection = intersectPoint - botLeft;
                Vector lineEndToIntersection = intersectPoint - topLeft;

                if(Vector::DotProduct(lineStartToIntersection, lineEndToIntersection) <= 0) {
                    // Set store t if it's the closest calculated so far
                    if(closest_t < 0 || t < closest_t) {
                        closest_t = t;
                    }
                }
            }
        }

        // Detection for right
        segmentStartDotNorm = DotPointVector(botRight, rightNorm);
        distanceRayStart = DotPointVector(Start, rightNorm) - segmentStartDotNorm;
        distanceRayEnd = DotPointVector(endPoint, rightNorm) - segmentStartDotNorm;

        // Ensure start and end point of ray are on opposite sides of line
        if(distanceRayStart * distanceRayEnd <= 0) {
            // Calculate intersection time
            Vector rayVector = endPoint - Start;
            float t = (segmentStartDotNorm - DotPointVector(Start, rightNorm)) / Vector::DotProduct(rayVector, rightNorm);

            // Ensure ray actually hit the line
            if(t >=0 && t <= 1) {
                // Calculate intersection point
                Point intersectPoint = Start + rayVector * t;

                // Ensure intersection point is on the line segment
                Vector lineStartToIntersection = intersectPoint - botRight;
                Vector lineEndToIntersection = intersectPoint - topRight;

                if(Vector::DotProduct(lineStartToIntersection, lineEndToIntersection) <= 0) {
                    // Set store t if it's the closest calculated so far
                    if(closest_t < 0 || t < closest_t) {
                        closest_t = t;
                    }
                }
            }
        }

        return closest_t;
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

    HalfPlane::CollisionInterval Ray::GetHalfPlaneInterval(const HalfPlane& halfPlane) {
        float dotStart = halfPlane.Dot(Start);
        float dotEnd = halfPlane.Dot(Start + Direction * Length);

        if (dotStart < 0 && dotEnd < 0) return HalfPlane::CollisionInterval(0, 1);
        if (dotStart > 0 && dotEnd > 0) return HalfPlane::CollisionInterval(1, 0);

        float ti = dotStart / (dotStart - dotEnd);

        if (dotStart > 0 && dotEnd <= 0) return HalfPlane::CollisionInterval(ti, 1);
        else if(dotStart <= 0 && dotEnd > 0) return HalfPlane::CollisionInterval(0, ti);
        else return HalfPlane::CollisionInterval(1, 0);
    }

    Entity* Ray::CheckCollisions(Ray& ray, std::vector<Entity *>& entities) {
        //std::cout << "Checking ray against " << entities.size() << " entities" << std::endl;
        Entity* colliding = 0;
        //float dist = ray.GetScaledVector().Magnitude();

        for (Entity* entity : entities) {
            /*ray.ResetLength();
            if (ray.IsColliding(entity)) {
                std::cout << "Collided with entity " << entity << std::endl;
                if (dist > ray.GetScaledVector().Magnitude()) {
                    dist = ray.GetScaledVector().Magnitude();
                    colliding = entity;
                }
            }*/
            float t = ray.IsColliding(entity);
            if(t > 0) {
                colliding = entity;
                ray.Length *= t;
            }
        }
        //std::cout << "Closest entity is " << colliding << std::endl;

        return colliding;
    }

    float Cross(Vector v1, Vector v2) {
        return v1[0]*v2[1] - v1[1]*v2[0];
    }

} // ForLeaseEngine
