/*!
    \file   ComponentLight.cpp

    \author Christopher Hudson

    \brief
        Implementation of a component that creates a light mesh

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "ComponentLight.h"
#include "ComponentOccluder.h"
#include "State.h"
#include "Entity.h"
#include "MultiEntityEvent.h"
#include "HalfPlane.h"
#include "LevelComponentRenderer.h"
#include "Ray.h"
#include <cmath>
#include <vector>
#include <unordered_set>
#include <map>

namespace ForLeaseEngine {

    namespace Components {

        Light::Light(Entity& parent, bool active, bool visible, bool drawOutline, Vector offset,
                     Vector direction, float angle, Color lightColor) :
                     Component(parent),
                     Active(active), Visible(visible), DrawOutline(drawOutline), Offset(offset),
                     Direction(direction), Angle(angle), LightColor(lightColor) {}

        Light::~Light() {}

        void Light::Update() {
            if(!Active) {
                return;
            }

            LevelComponents::Renderer* render = ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Renderer>();

            LightMesh.ClearData();
            Vector normalized = Vector(Direction);
            normalized.Normalize();
            Vector start = Vector::Rotate(normalized, -Angle / 2);

            // Check for observed objects
            Components::Transform* trans = Parent.GetComponent<Components::Transform>();

            render->SetDrawingColor(Color(1, 1, 1, 1));
            render->DrawArrow(trans->Position, Direction * 4);
            render->DrawArrow(trans->Position, Vector::Rotate(Direction * 4, Angle / 2));
            render->DrawArrow(trans->Position, Vector::Rotate(Direction * 4, -Angle / 2));

            std::vector<Entity*> detected = ForLease->GameStateManager().CurrentState().GetEntitiesInCone(trans->Position + Offset, 0, normalized, Angle);
            for(std::vector<Entity*>::iterator i = detected.begin(); i != detected.end(); ++i) {
                if(*i == &Parent) {
                    detected.erase(i);
                    break;
                }
            }
            std::cout << detected.size() << " POTENTIAL ENTITIES" << std::endl;

            // not sure why i did this?
            if(true) {
                std::unordered_set<unsigned long> litEntitiyIDs;
                std::map<float, Point> collisionPoints;
                std::vector<Point> castingPoints;
                MultiEntityEvent multi_e = MultiEntityEvent("EntitiesLit");
                Point castingPoint = trans->Position + Offset;
                //unsigned long parentID = Parent.GetID();

                // Get half planes for vision cone
                Point mid = castingPoint + normalized;
                Point bot = castingPoint + Vector::Rotate(normalized, Angle / 2);
                Point top = castingPoint + Vector::Rotate(normalized, -Angle / 2);
                HalfPlane hp1 = HalfPlane(castingPoint, top, mid);
                HalfPlane hp2 = HalfPlane(castingPoint, bot, mid);
                Vector zeroAngleVector = top - castingPoint;
                //float radius2 = Radius * Radius;
                Vector worldToModel = Point(0, 0) - trans->Position;

                render->SetDrawingColor(0, 0, 1);
                render->SetDebugPointSize(8);
                // Add corners of screen, skipping for now

                castingPoints.push_back(top);
                castingPoints.push_back(bot);

                // Get all casting points
                for(int i = 0; i < detected.size();) {
                    Components::Occluder* occluder = detected[i]->GetComponent<Components::Occluder>();
                    // Object must have an occluder
                    if(!occluder || !occluder->BlocksLight) {
                        detected.erase(detected.begin() + i);
                        continue;
                    }
                    Components::Transform* detectedTrans = detected[i]->GetComponent<Components::Transform>();
                    Components::Collision* detectedCollision = detected[i]->GetComponent<Components::Collision>();
                    float halfWidth = detectedCollision->Width / 2 * detectedTrans->ScaleX;
                    float halfHeight = detectedCollision->Height / 2 * detectedTrans->ScaleY;
                    Point colliderCenter = detectedTrans->Position + Vector(detectedCollision->OffsetX, detectedCollision->OffsetY);
                    Point tl = Point(colliderCenter[0] - halfWidth, colliderCenter[1] + halfHeight);
                    Point tr = Point(colliderCenter[0] + halfWidth, colliderCenter[1] + halfHeight);
                    Point br = Point(colliderCenter[0] + halfWidth, colliderCenter[1] - halfHeight);
                    Point bl = Point(colliderCenter[0] - halfWidth, colliderCenter[1] - halfHeight);

                    if(hp1.Dot(tl) < 0 && hp2.Dot(tl) < 0) {
                        castingPoints.push_back(tl);
                        render->DrawPoint(tl);
                    }
                    if(hp1.Dot(tr) < 0 && hp2.Dot(tr) < 0) {
                        castingPoints.push_back(tr);
                        render->DrawPoint(tr);
                    }
                    if(hp1.Dot(br) < 0 && hp2.Dot(br) < 0) {
                        castingPoints.push_back(br);
                        render->DrawPoint(br);
                    }
                    if(hp1.Dot(bl) < 0 && hp2.Dot(bl) < 0) {
                        castingPoints.push_back(bl);
                        render->DrawPoint(bl);
                    }

                    ++i;
                }

                std::cout << detected.size() << " OCCLUDING ENTITIES" << std::endl;

                //render->SetDrawingColor(Color(0, 1, 0));
                const float angleOffset = 0.00001;
                // This whole thing should be adjusted to only do 1 ray cast when ray casting can return more than one entity
                for(std::vector<Point>::iterator i = castingPoints.begin(); i != castingPoints.end(); ++i) {
                    Point point = (*i);
                    Point prePoint = castingPoint + Vector::Rotate(point - castingPoint, -angleOffset);
                    Point postPoint = castingPoint + Vector::Rotate(point - castingPoint, angleOffset);

                    Ray preRay = Ray(castingPoint, prePoint - castingPoint, 9999);
                    Ray ray = Ray(castingPoint, point - castingPoint, 9999);
                    Ray postRay = Ray(castingPoint, postPoint - castingPoint, 9999);

                    Entity* preHit = Ray::CheckCollisions(preRay, detected);
                    Entity* hit = Ray::CheckCollisions(ray, detected);
                    Entity* postHit = Ray::CheckCollisions(postRay, detected);

                    if(preHit) {
                        render->SetDrawingColor(Color(1, 0, 0));
                        litEntitiyIDs.insert(preHit->GetID());
                        render->DrawArrow(trans->Position, preRay.GetScaledVector());
                    }
                    float preAngle = Vector::AngleBetween(zeroAngleVector, preRay.GetIntersectionPoint() - castingPoint);
                    if(preAngle < 0)
                        preAngle += 2 * PI;
                    collisionPoints.insert(std::make_pair(preAngle, preRay.GetIntersectionPoint()));
                    //render->DrawLine(castingPoint, preRay.GetScaledVector());

                    if(hit) {
                        render->SetDrawingColor(Color(0, 1, 0));
                        litEntitiyIDs.insert(hit->GetID());
                        render->DrawArrow(trans->Position, ray.GetScaledVector());
                    }
                    float angle = Vector::AngleBetween(zeroAngleVector, ray.GetIntersectionPoint() - castingPoint);
                    if(angle < 0)
                        angle += 2 * PI;
                    collisionPoints.insert(std::make_pair(angle, ray.GetIntersectionPoint()));
                    //render->DrawLine(castingPoint, ray.GetScaledVector());

                    if(postHit) {
                        render->SetDrawingColor(Color(0, 0, 1));
                        litEntitiyIDs.insert(postHit->GetID());
                        render->DrawArrow(trans->Position, postRay.GetScaledVector());
                    }
                    float postAngle = Vector::AngleBetween(zeroAngleVector, postRay.GetIntersectionPoint() - castingPoint);
                    if(postAngle < 0)
                        postAngle += 2 * PI;
                    collisionPoints.insert(std::make_pair(postAngle, postRay.GetIntersectionPoint()));
                    //render->DrawLine(castingPoint, postRay.GetScaledVector());
                }

                std::cout << litEntitiyIDs.size() << " LIT ENTITIES" << std::endl;
                std::cout << collisionPoints.size() << " COLLISION POINTS" << std::endl;
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

                //multi_e.EntityIDs = std::vector<unsigned long>(visibleEntityIDs.begin(), visibleEntityIDs.end());

                //std::cout << "I see " << multi_e.EntityIDs.size() << " entities" << std::endl;
                //ForLease->Dispatcher.DispatchToParent(&multi_e, &Parent);
        }

        void Light::Serialize(Serializer& root) {
//            Serializer light = root.GetChild("Light");
//            light.WriteUint("Type", static_cast<unsigned>(Type));
//            light.WriteVec("Direction", Direction);
//            light.WriteFloat("Angle", Angle);
//            DrawColor.Serialize(light);
//            root.Append(light, "Light");
        }

        void Light::Deserialize(Serializer& root) {
//            Serializer light = root.GetChild("Light");
//            light.ReadVec("Direction", Direction);
//            light.ReadFloat("Sweep", Sweep);
//            light.ReadUint("Rays", Rays);
//            DrawColor.Deserialize(light);
        }

        Mesh* Light::GetLightMesh() {
            return &LightMesh;
        }
    } // Components

} // ForLeaseEngine
