/*!
    \file   ComponentVisionCone.cpp
    \author Christopher Hudson

    \brief
        A component for detecting entities in a cone

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "ComponentVisionCone.h"
#include "Engine.h" // Just for testing, remove when done
#include "LevelComponentRenderer.h" // Testing
#include "MultiEntityEvent.h"
#include "Ray.h"
#include "ComponentTransform.h"
#include "ComponentCollision.h"
#include "HalfPlane.h"
#include <vector>
#include <iostream>
#include <map>
#include <unordered_set>

//static const int ViewResolution = 12;

namespace ForLeaseEngine {
    namespace Components {
        VisionCone::VisionCone(Entity& parent, bool active, bool drawOutline, bool visible, Vector const& offset, float radius, Vector const& direction, float angle, Color const& indicatorColor)
                              : Component(parent, ComponentType::Transform | ComponentType::Collision),
                                Active(active), Visible(visible), DrawOutline(drawOutline),
                                Offset(offset), Radius(radius), Direction(direction),
                                Angle(angle), IndicatorColor(indicatorColor) {}

        VisionCone::~VisionCone() {}

        ComponentType VisionCone::GetType() { return Type; }

        void VisionCone::Update() {
            ////////////////////////////////////////////
            float scaledDt = ForLease->FrameRateController().GetDt();
            float realDt = ForLease->FrameRateController().GetUnscaledDt();
            float ratio = scaledDt / realDt;
            float tolerance = 0.00001;

            // Normal speed
            if(ratio > 1 - tolerance && ratio < 1 + tolerance) {
                Visible = false;
            }
            // Slow speed
            else {
                Visible = true;
            }
            ////////////////////////////////////////////
            ViewMesh.ClearData();
            if(!Active) {
                return;
            }
            Vector normalized = Vector(Direction);
            normalized.Normalize();
            Vector start = Vector::Rotate(normalized, -Angle / 2);

            // Testing code
            LevelComponents::Renderer* render = ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Renderer>();
            ViewMesh.AddVertex(Offset, Point(0, 0));
            ViewMesh.AddVertex(Point(0, 0) + Offset + start * Radius, Point(0, 0));
            int numPoints = 5;
            for(int i = 1; i < numPoints; ++i) {
                Vector currentDir = Vector::Rotate(start, Angle / (numPoints - 1) * i);
                ViewMesh.AddVertex(Point(0, 0) + Offset + currentDir * Radius, Point(0, 0));
            }

            // Add faces
            for(int i = 2; i < ViewMesh.GetVertexCount(); ++i) {
                ViewMesh.AddFace(IndexedFace(0, i - 1, i), IndicatorColor);
            }

            // Add edges
            for(int i = 1; i < ViewMesh.GetVertexCount(); ++i) {
                ViewMesh.AddEdge(i - 1, i);
            }
            ViewMesh.AddEdge(ViewMesh.GetVertexCount() - 1, 0);

            // Check for observed objects
            Components::Transform* trans = Parent.GetComponent<Components::Transform>();
            std::vector<Entity*> detected = ForLease->GameStateManager().CurrentState().GetEntitiesInCone(trans->Position + Offset, Radius, normalized, Angle);
            for(std::vector<Entity*>::iterator i = detected.begin(); i != detected.end(); ++i) {
                if(*i == &Parent) {
                    detected.erase(i);
                    break;
                }
            }

            if(true) {
                std::unordered_set<unsigned long> visibleEntityIDs;
                std::map<float, Point> collisionPoints;
                std::vector<Point> castingPoints;
                MultiEntityEvent multi_e = MultiEntityEvent("EntitiesSeen");
                Point castingPoint = trans->Position + Offset;
                //unsigned long parentID = Parent.GetID();

                // Get half planes for vision cone
                Point mid = castingPoint + normalized * Radius;
                Point bot = castingPoint + Vector::Rotate(normalized, Angle / 2) * Radius;
                Point top = castingPoint + Vector::Rotate(normalized, -Angle / 2) * Radius;
                HalfPlane hp1 = HalfPlane(castingPoint, top, mid);
                HalfPlane hp2 = HalfPlane(castingPoint, bot, mid);
                Vector zeroAngleVector = top - castingPoint;
                float radius2 = Radius * Radius;
                Vector worldToModel = Point(0, 0) - trans->Position;

                render->SetDrawingColor(0, 0, 1);
                render->SetDebugPointSize(8);
                // Add corners of screen, skipping for now

                // Add required points for outside edge
                /*const int numPoints = 30;
                for(int i = 0; i < numPoints; ++i) {
                    Vector currentDir = Vector::Rotate(start, Angle / (numPoints - 1) * i);
                    castingPoints.push_back(castingPoint + currentDir);
                }*/
                castingPoints.push_back(top);
                castingPoints.push_back(bot);

                // Get all casting points
                for(int i = 0; i < detected.size(); ++i) {
                    Components::Transform* detectedTrans = detected[i]->GetComponent<Components::Transform>();
                    Components::Collision* detectedCollision = detected[i]->GetComponent<Components::Collision>();
                    float halfWidth = detectedCollision->Width / 2 * detectedTrans->ScaleX;
                    float halfHeight = detectedCollision->Height / 2 * detectedTrans->ScaleY;
                    Point colliderCenter = detectedTrans->Position + Vector(detectedCollision->OffsetX, detectedCollision->OffsetY);
                    Point tl = Point(colliderCenter[0] - halfWidth, colliderCenter[1] + halfHeight);
                    Point tr = Point(colliderCenter[0] + halfWidth, colliderCenter[1] + halfHeight);
                    Point br = Point(colliderCenter[0] + halfWidth, colliderCenter[1] - halfHeight);
                    Point bl = Point(colliderCenter[0] - halfWidth, colliderCenter[1] - halfHeight);

                    if(hp1.Dot(tl) < 0 && hp2.Dot(tl) < 0 && Point::DistanceSquared(tl, castingPoint) < radius2) {
                        castingPoints.push_back(tl);
                        //render->DrawPoint(tl);
                    }
                    if(hp1.Dot(tr) < 0 && hp2.Dot(tr) < 0 && Point::DistanceSquared(tr, castingPoint) < radius2) {
                        castingPoints.push_back(tr);
                        //render->DrawPoint(tr);
                    }
                    if(hp1.Dot(br) < 0 && hp2.Dot(br) < 0 && Point::DistanceSquared(br, castingPoint) < radius2) {
                        castingPoints.push_back(br);
                        //render->DrawPoint(br);
                    }
                    if(hp1.Dot(bl) < 0 && hp2.Dot(bl) < 0 && Point::DistanceSquared(bl, castingPoint) < radius2) {
                        castingPoints.push_back(bl);
                        //render->DrawPoint(bl);
                    }
                }

                const float angleOffset = 0.00001;
                // This whole thing should be adjusted to only do 1 ray cast when ray casting can return more than one entity
                for(std::vector<Point>::iterator i = castingPoints.begin(); i != castingPoints.end(); ++i) {
                    Point point = (*i);
                    Point prePoint = castingPoint + Vector::Rotate(point - castingPoint, -angleOffset);
                    Point postPoint = castingPoint + Vector::Rotate(point - castingPoint, angleOffset);

                    Ray preRay = Ray(castingPoint, prePoint - castingPoint, Radius);
                    Ray ray = Ray(castingPoint, point - castingPoint, Radius);
                    Ray postRay = Ray(castingPoint, postPoint - castingPoint, Radius);

                    Entity* preHit = Ray::CheckCollisions(preRay, detected);
                    Entity* hit = Ray::CheckCollisions(ray, detected);
                    Entity* postHit = Ray::CheckCollisions(postRay, detected);

                    if(preHit) {
                        visibleEntityIDs.insert(preHit->GetID());
                    }
//                    float preAngle = Vector::AngleBetween(zeroAngleVector, preRay.GetIntersectionPoint() - castingPoint);
//                    if(preAngle < 0)
//                        preAngle += 2 * PI;
//                    collisionPoints.insert(std::make_pair(preAngle, preRay.GetIntersectionPoint()));
                    //render->DrawLine(castingPoint, preRay.GetScaledVector());

                    if(hit) {
                        visibleEntityIDs.insert(hit->GetID());
                    }
//                    float angle = Vector::AngleBetween(zeroAngleVector, ray.GetIntersectionPoint() - castingPoint);
//                    if(angle < 0)
//                        angle += 2 * PI;
//                    collisionPoints.insert(std::make_pair(angle, ray.GetIntersectionPoint()));
                    //render->DrawLine(castingPoint, ray.GetScaledVector());

                    if(postHit) {
                        visibleEntityIDs.insert(postHit->GetID());
                    }
//                    float postAngle = Vector::AngleBetween(zeroAngleVector, postRay.GetIntersectionPoint() - castingPoint);
//                    if(postAngle < 0)
//                        postAngle += 2 * PI;
//                    collisionPoints.insert(std::make_pair(postAngle, postRay.GetIntersectionPoint()));
                    //render->DrawLine(castingPoint, postRay.GetScaledVector());
                }

                // Add vertices to mesh
                /*for(std::map<float, Point>::iterator i = collisionPoints.begin(); i != collisionPoints.end(); ++i) {
                    ViewMesh.AddVertex((*i).second + worldToModel, Point(0, 0));
                }*/

                // Debug drawing
                /*render->SetDrawingColor(1, 0, 0);
                for(int i = 1; i < ViewMesh.GetVertexCount(); ++i) {
                    render->DrawLine(ViewMesh.GetVertex(0) + (trans->Position  - Point(0, 0)), ViewMesh.GetVertex(i) + (trans->Position - Point(0, 0)));
                }*/

                // Create Faces
                /*for(int i = 2; i < ViewMesh.GetVertexCount(); ++i) {
                    ViewMesh.AddFace(IndexedFace(0, i - 1, i), IndicatorColor);
                }

                // Add edges
                for(int i = 1; i < ViewMesh.GetVertexCount(); ++i) {
                    ViewMesh.AddEdge(i - 1, i);
                }
                ViewMesh.AddEdge(ViewMesh.GetVertexCount() - 1, 0);*/

                multi_e.EntityIDs = std::vector<unsigned long>(visibleEntityIDs.begin(), visibleEntityIDs.end());

                //std::cout << "I see " << multi_e.EntityIDs.size() << " entities" << std::endl;
                ForLease->Dispatcher.DispatchToParent(&multi_e, &Parent);
            }
        }

        void VisionCone::Serialize(Serializer& root) {
            root.WriteUint("Type", static_cast<unsigned int>(Type));
            Serializer visionCone = root.GetChild("VisionCone");
            visionCone.WriteUint("Type", static_cast<unsigned int>(Type));
            visionCone.WriteBool("Active", Active);
            visionCone.WriteBool("Visible", Visible);
            visionCone.WriteBool("DrawOutlines", DrawOutline);
            Serializer offset = visionCone.GetChild("Offset");
            offset.WriteFloat("X", Offset[0]);
            offset.WriteFloat("Y", Offset[1]);
            visionCone.Append(offset, "Offset");
            visionCone.WriteFloat("Radius", Radius);
            Serializer direction = visionCone.GetChild("Direction");
            direction.WriteFloat("X", Direction[0]);
            direction.WriteFloat("Y", Direction[1]);
            visionCone.Append(direction, "Direction");
            visionCone.WriteFloat("Angle", Angle);
            Serializer indicatorColor = visionCone.GetChild("IndicatorColor");
            IndicatorColor.Serialize(indicatorColor);
            visionCone.Append(indicatorColor, "IndicatorColor");
            root.Append(visionCone, "VisionCone");
        }

        void VisionCone::Deserialize(Serializer& root) {
            Serializer visionCone = root.GetChild("VisionCone");
            visionCone.ReadBool("Active", Active);
            visionCone.ReadBool("Visible", Visible);
            visionCone.ReadBool("DrawOutlines", DrawOutline);
            Serializer offset = visionCone.GetChild("Offset");
            offset.ReadFloat("X", Offset[0]);
            offset.ReadFloat("Y", Offset[1]);
            visionCone.ReadFloat("Radius", Radius);
            Serializer direction = visionCone.GetChild("Direction");
            direction.ReadFloat("X", Direction[0]);
            direction.ReadFloat("Y", Direction[1]);
            visionCone.ReadFloat("Angle", Angle);
            Serializer indicatorColor = visionCone.GetChild("IndicatorColor");
            IndicatorColor.Deserialize(indicatorColor);
        }

        Mesh* VisionCone::GetVisionMesh() {
            return &ViewMesh;
        }
    }
}
