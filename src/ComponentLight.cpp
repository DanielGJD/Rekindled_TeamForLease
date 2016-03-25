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
#include <fstream>

namespace ForLeaseEngine {

    namespace Components {
        Light::Light(Entity& parent, bool active, bool visible, bool drawOutline, Vector offset,
                     Vector direction, float angle, Color lightColor, BlendMode lightMode, float radius, std::string lightTexture, bool pointLight) :
                     Component(parent),
                     Active(active), Visible(visible), DrawOutline(drawOutline), Offset(offset),
                     Direction(direction), Angle(angle), LightColor(lightColor), LightMode(lightMode), Radius(radius), LightTexture(lightTexture), PointLight(pointLight) {}

        Light::~Light() {}

        void Light::Update() {}

        std::unordered_set<unsigned long> Light::ComputeLighting() {
            //if(!Active) {
            //    return std::unordered_set;
            //}

            LevelComponents::Renderer* render = ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Renderer>();
            Entity* cameraObject = ForLease->GameStateManager().CurrentState().GetEntityByID(render->GetCameraID());
            Components::Camera* camera = cameraObject->GetComponent<Components::Camera>();
            Components::Transform* cameraTrans = cameraObject->GetComponent<Components::Transform>();
            float camHHeight = camera->Size / 2;
            float camHWidth = static_cast<float>(ForLease->GameWindow->GetXResolution()) / ForLease->GameWindow->GetYResolution() * camHHeight;
            Point camCorners[4];
            camCorners[0] = Point(cameraTrans->Position[0] - camHWidth, cameraTrans->Position[1] + camHHeight);
            camCorners[1] = Point(cameraTrans->Position[0] + camHWidth, cameraTrans->Position[1] + camHHeight);
            camCorners[2] = Point(cameraTrans->Position[0] + camHWidth, cameraTrans->Position[1] - camHHeight);
            camCorners[3] = Point(cameraTrans->Position[0] - camHWidth, cameraTrans->Position[1] - camHHeight);

            LightMesh.ClearData();
            Vector normalized = Vector(Direction);
            normalized.Normalize();
            //Vector start = Vector::Rotate(normalized, -Angle / 2);

            // Check for observed objects
            Components::Transform* trans = Parent.GetComponent<Components::Transform>();

//            render->SetDrawingColor(Color(0, 0, 1, 1));
//            render->DrawArrow(trans->Position, Direction * 4);
//            render->DrawArrow(trans->Position, Vector::Rotate(Direction * 4, Angle / 2));
//            render->DrawArrow(trans->Position, Vector::Rotate(Direction * 4, -Angle / 2));

            std::vector<Entity*> broadDetected;
            if(!PointLight) {
                broadDetected = ForLease->GameStateManager().CurrentState().GetEntitiesInCone(trans->Position + Offset, 0, normalized, Angle);
            }
            else {
                //broadDetected = ForLease->GameStateManager().CurrentState().GetEntitiesInRadius(trans->Position + Offset, Radius);
                broadDetected = ForLease->GameStateManager().CurrentState().GetAllEntities();
            }
            std::vector<Entity*> detected;
            for(std::vector<Entity*>::iterator i = broadDetected.begin(); i != broadDetected.end(); ++i) {
//                if(*i == &Parent) {
//                    detected.erase(i);
//                    break;
//                }
                Components::Occluder* occluder = (*i)->GetComponent<Components::Occluder>();
                if(*i != &Parent && occluder && occluder->BlocksLight) {
                    detected.push_back(*i);
                }
            }
            //std::cout << detected.size() << " POTENTIAL ENTITIES" << std::endl;

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
                Point bot = castingPoint + Vector::Rotate(Direction, Angle / 2);
                Point top = castingPoint + Vector::Rotate(Direction, -Angle / 2);
                HalfPlane hp1 = HalfPlane(castingPoint, top, mid);
                HalfPlane hp2 = HalfPlane(castingPoint, bot, mid);
                Vector zeroAngleVector = Direction;
                Vector worldToModel = Point(0, 0) - trans->Position;

                if(!PointLight) {
                    castingPoints.push_back(top);
                    castingPoints.push_back(bot);
                }

                for(unsigned int i = 0; i < 4; ++i) {
                    if(!PointLight) {
                        if(Angle <= PI) {
                            if(hp1.Dot(camCorners[i]) < 0 && hp2.Dot(camCorners[i]) < 0)
                                castingPoints.push_back(camCorners[i]);
                        }
                        else {
                            if(hp1.Dot(camCorners[i]) < 0 || hp2.Dot(camCorners[i]) < 0)
                                castingPoints.push_back(camCorners[i]);
                                //std::cout << "Adding cam corner " << i << std::endl;
                        }
                    }
                    else {
                        castingPoints.push_back(camCorners[i]);
                    }
                }

                // Get all casting points
                for(unsigned int i = 0; i < detected.size();) {
                    Components::Occluder* occluder = detected[i]->GetComponent<Components::Occluder>();
                    // Object must have an occluder
                    if(!occluder) {
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

                    if(!PointLight) {
                        if(Angle <= PI) {
                            if(hp1.Dot(tl) <= 0 && hp2.Dot(tl) <= 0) {
                                castingPoints.push_back(tl);
                                //render->DrawPoint(tl);
                            }
                            if(hp1.Dot(tr) <= 0 && hp2.Dot(tr) <= 0) {
                                castingPoints.push_back(tr);
                                //render->DrawPoint(tr);
                            }
                            if(hp1.Dot(br) <= 0 && hp2.Dot(br) <= 0) {
                                castingPoints.push_back(br);
                                //render->DrawPoint(br);
                            }
                            if(hp1.Dot(bl) <= 0 && hp2.Dot(bl) <= 0) {
                                castingPoints.push_back(bl);
                                //render->DrawPoint(bl);
                            }
                        }
                        else {
                            if(hp1.Dot(tl) <= 0 || hp2.Dot(tl) <= 0) {
                                castingPoints.push_back(tl);
                                render->DrawPoint(tl);
                            }
                            if(hp1.Dot(tr) <= 0 || hp2.Dot(tr) <= 0) {
                                castingPoints.push_back(tr);
                                render->DrawPoint(tr);
                            }
                            if(hp1.Dot(br) <= 0 || hp2.Dot(br) <= 0) {
                                castingPoints.push_back(br);
                                render->DrawPoint(br);
                            }
                            if(hp1.Dot(bl) <= 0 || hp2.Dot(bl) <= 0) {
                                castingPoints.push_back(bl);
                                render->DrawPoint(bl);
                            }
                        }
                    }
                    else {
                        castingPoints.push_back(tl);
                        castingPoints.push_back(tr);
                        castingPoints.push_back(br);
                        castingPoints.push_back(bl);
                    }

                    ++i;
                }

                //render->SetDrawingColor(0, 1, 0);
                for(unsigned int i = 0; i < castingPoints.size(); ++i) {
                    //render->DrawPoint(castingPoints[i]);
                }

                //std::cout << detected.size() << " OCCLUDING ENTITIES" << std::endl;

                //render->SetDrawingColor(Color(0, 1, 0));
                //const float angleOffset = 0.001;
                // This whole thing should be adjusted to only do 1 ray cast when ray casting can return more than one entity
                for(std::vector<Point>::iterator i = castingPoints.begin(); i != castingPoints.end(); ++i) {
                    Point point = (*i);
                    Vector offset = point - castingPoint;
                    offset = Vector(-offset[1], offset[0]);
                    offset.Normalize();
                    //Point prePoint = castingPoint + Vector::Rotate(point - castingPoint, -angleOffset);
                    //Point postPoint = castingPoint + Vector::Rotate(point - castingPoint, angleOffset);
                    Point prePoint = point + offset * 0.001;
                    Point postPoint = point - offset * 0.001;

                    Ray preRay = Ray(castingPoint, prePoint - castingPoint, 9999, -1);
                    Ray ray = Ray(castingPoint, point - castingPoint, 9999, -1);
                    Ray postRay = Ray(castingPoint, postPoint - castingPoint, 9999, -1);

                    //Entity* preHit = Ray::CheckCollisions(preRay, detected);
                    //Entity* hit = Ray::CheckCollisions(ray, detected);
                    //Entity* postHit = Ray::CheckCollisions(postRay, detected);
                    std::vector<Ray::Collision> preHit = Ray::CheckCollisionsMultipleEntities(preRay, detected);
                    std::vector<Ray::Collision> hit = Ray::CheckCollisionsMultipleEntities(ray, detected);
                    std::vector<Ray::Collision> postHit = Ray::CheckCollisionsMultipleEntities(postRay, detected);


                    for(unsigned i = 0; i < preHit.size(); ++i) {
                        render->SetDrawingColor(1, 0, 0, 1);
                        render->SetDebugPointSize(8);
                        render->DrawPoint(preHit[i].Point);
                        if(preHit[i].Entity->HasComponent(ComponentType::Occluder)) {
                            litEntitiyIDs.insert(preHit[i].Entity->GetID());

                            if(preHit[i].Entity->GetComponent<Components::Occluder>()->BlocksLight) {
                                float preAngle = Vector::AngleBetween(zeroAngleVector, preHit[i].Point - castingPoint);

//                                if(preAngle < 0) {
//                                    preAngle += 2 * PI;
//                                }

                                collisionPoints.insert(std::make_pair(preAngle, preHit[i].Point));
                                break;
                            }
                        }
                    }
                    if(preHit.empty()) {
                        Point collision = CheckRayAgainstWindow(preRay, camCorners[0], camCorners[1], camCorners[2], camCorners[3], cameraTrans->Position);
                        float preAngle = Vector::AngleBetween(zeroAngleVector, collision - castingPoint);
//                        if(preAngle < 0)
//                            preAngle += 2 * PI;
                        collisionPoints.insert(std::make_pair(preAngle, collision));
                    }

                    for(unsigned i = 0; i < postHit.size(); ++i) {
                        render->SetDrawingColor(1, 0, 0, 1);
                        render->SetDebugPointSize(8);
                        render->DrawPoint(postHit[i].Point);
                        if(postHit[i].Entity->HasComponent(ComponentType::Occluder)) {
                            litEntitiyIDs.insert(postHit[i].Entity->GetID());

                            if(postHit[i].Entity->GetComponent<Components::Occluder>()->BlocksLight) {
                                float postAngle = Vector::AngleBetween(zeroAngleVector, postHit[i].Point - castingPoint);

//                                if(postAngle < 0) {
//                                    postAngle += 2 * PI;
//                                }

                                collisionPoints.insert(std::make_pair(postAngle, postHit[i].Point));
                                break;
                            }
                        }
                    }
                    if(postHit.empty()) {
                        Point collision = CheckRayAgainstWindow(postRay, camCorners[0], camCorners[1], camCorners[2], camCorners[3], cameraTrans->Position);
                        float postAngle = Vector::AngleBetween(zeroAngleVector, collision - castingPoint);
//                        if(postAngle < 0)
//                            postAngle += 2 * PI;
                        collisionPoints.insert(std::make_pair(postAngle, collision));
                    }

                    for(unsigned i = 0; i < hit.size(); ++i) {
                        render->SetDrawingColor(1, 0, 0, 1);
                        render->SetDebugPointSize(8);
                        render->DrawPoint(hit[i].Point);
                        if(hit[i].Entity->HasComponent(ComponentType::Occluder)) {
                            litEntitiyIDs.insert(hit[i].Entity->GetID());

                            if(hit[i].Entity->GetComponent<Components::Occluder>()->BlocksLight) {
                                float angle = Vector::AngleBetween(zeroAngleVector, hit[i].Point - castingPoint);

//                                if(angle < 0) {
//                                    angle += 2 * PI;
//                                }

                                collisionPoints.insert(std::make_pair(angle, hit[i].Point));
                                break;
                            }
                        }
                    }
                    if(hit.empty()) {
                        Point collision = CheckRayAgainstWindow(ray, camCorners[0], camCorners[1], camCorners[2], camCorners[3], cameraTrans->Position);
                        float angle = Vector::AngleBetween(zeroAngleVector, collision - castingPoint);
//                        if(angle < 0)
//                            angle += 2 * PI;
                        collisionPoints.insert(std::make_pair(angle, collision));
                    }

//                    if(preHit) {
//                        //render->SetDrawingColor(Color(1, 0, 0));
//                        //litEntitiyIDs.insert(preHit->GetID());
//                        //render->DrawArrow(trans->Position, preRay.GetScaledVector());
//
//                        //float preAngle = Vector::AngleBetween(zeroAngleVector, preRay.GetIntersectionPoint() - castingPoint);
//                        //if(preAngle < 0)
//                            //preAngle += 2 * PI;
//                        //collisionPoints.insert(std::make_pair(preAngle, preRay.GetIntersectionPoint()));
//                        //->DrawLine(castingPoint, preRay.GetScaledVector());
//                    }
//                    else {
//                        Point collision = CheckRayAgainstWindow(preRay, camCorners[0], camCorners[1], camCorners[2], camCorners[3], cameraTrans->Position);
//                        float preAngle = Vector::AngleBetween(zeroAngleVector, collision - castingPoint);
//                        if(preAngle < 0)
//                            preAngle += 2 * PI;
//                        collisionPoints.insert(std::make_pair(preAngle, collision));
//                    }
//
//                    if(hit) {
//                        //render->SetDrawingColor(Color(0, 1, 0));
//                        //litEntitiyIDs.insert(hit->GetID());
//                        //render->DrawArrow(trans->Position, ray.GetScaledVector());
//
//                        //float angle = Vector::AngleBetween(zeroAngleVector, ray.GetIntersectionPoint() - castingPoint);
//                        //if(angle < 0)
//                            //angle += 2 * PI;
//                        //collisionPoints.insert(std::make_pair(angle, ray.GetIntersectionPoint()));
//                        //render->DrawLine(castingPoint, ray.GetScaledVector());
//                    }
//                    else {
//                        Point collision = CheckRayAgainstWindow(ray, camCorners[0], camCorners[1], camCorners[2], camCorners[3], cameraTrans->Position);
//                        float preAngle = Vector::AngleBetween(zeroAngleVector, collision - castingPoint);
//                        if(preAngle < 0)
//                            preAngle += 2 * PI;
//                        collisionPoints.insert(std::make_pair(preAngle, collision));
//                    }
//
//                    if(postHit) {
//                        //render->SetDrawingColor(Color(0, 0, 1));
//                        //litEntitiyIDs.insert(postHit->GetID());
//                        //render->DrawArrow(trans->Position, postRay.GetScaledVector());
//                        //float postAngle = Vector::AngleBetween(zeroAngleVector, postRay.GetIntersectionPoint() - castingPoint);
//                        //if(postAngle < 0)
//                            //postAngle += 2 * PI;
//                        //collisionPoints.insert(std::make_pair(postAngle, postRay.GetIntersectionPoint()));
//                        //render->DrawLine(castingPoint, postRay.GetScaledVector());
//                    }
//                    else {
//                        Point collision = CheckRayAgainstWindow(postRay, camCorners[0], camCorners[1], camCorners[2], camCorners[3], cameraTrans->Position);
//                        float preAngle = Vector::AngleBetween(zeroAngleVector, collision - castingPoint);
//                        if(preAngle < 0)
//                            preAngle += 2 * PI;
//                        collisionPoints.insert(std::make_pair(preAngle, collision));
//                    }
                }

                //std::cout << litEntitiyIDs.size() << " LIT ENTITIES" << std::endl;
               // std::cout << collisionPoints.size() << " COLLISION POINTS" << std::endl;

                // Add vertices to mesh
                LightMesh.AddVertex(0, 0, 0.5, 0.5);
                float textureScale = 1 / (2 * Radius);
                Vector toLightSpace = Point() - castingPoint;
                //std::cout << textureScale << std::endl;
                for(std::map<float, Point>::iterator i = collisionPoints.begin(); i != collisionPoints.end(); ++i) {
                    Point uv;
                    if(LightTexture.compare("") != 0) {
                        Point inLightSpace = (*i).second + toLightSpace;
                        Vector uvVec = inLightSpace - Point();
                        float dist = uvVec.Magnitude();
                        float newDist = dist * textureScale;
                        uvVec.Normalize();
                        uvVec = uvVec * newDist;
                        uv = Point() + uvVec;
                        //Matrix toUV = Matrix::Translation(Vector(0.5, 0.5));
                        uv = uv + Vector(0.5, 0.5);
                    }

                    LightMesh.AddVertex((*i).second + worldToModel, uv);
                }

//                static bool writeLight = true;
//                if(writeLight && Parent.GetName().compare("GreenLight") == 0) {
//                    writeLight = false;
//                    std::ofstream out(Parent.GetName(), std::ofstream::out);
//                    out << collisionPoints.size() << std::endl;
//                    out << "[" << castingPoint[0] << "," << castingPoint[1] << "]" << std::endl;
//                    for(std::map<float, Point>::iterator i = collisionPoints.begin(); i != collisionPoints.end(); ++i) {
//                        Point p = (*i).second;
//                        out << (*i).first << ":[" << p[0] << "," << p[1] << "]" <<  std::endl;
//                    }
//                    out.close();
//                }

                // Debug drawing
//                render->SetDrawingColor(1, 0, 0);
//                for(int i = 1; i < LightMesh.GetVertexCount(); ++i) {
//                    render->DrawLine(LightMesh.GetVertex(0) + (trans->Position  - Point(0, 0)), LightMesh.GetVertex(i) + (trans->Position - Point(0, 0)));
//                }



                // Create Faces
                for(int i = 2; i < LightMesh.GetVertexCount(); ++i) {
                    LightMesh.AddFace(IndexedFace(0, i - 1, i), LightColor);
                }
                if(PointLight) {
                    LightMesh.AddFace(IndexedFace(0, 1, LightMesh.GetVertexCount() - 1), LightColor);
                }

                // Add edges
//                for(int i = 1; i < LightMesh.GetVertexCount(); ++i) {
//                    LightMesh.AddEdge(i - 1, i);
//                }
//                LightMesh.AddEdge(LightMesh.GetVertexCount() - 1, 0);
                //LightMesh.AddEdge(IndexedEdge(0, 1), 100);
                //LightMesh.AddEdge(IndexedEdge(0, LightMesh.GetVertexCount() - 1), 100);

                //render->SetModelView(trans);
                //render->DrawMesh(&LightMesh, false, false);

                //multi_e.EntityIDs = std::vector<unsigned long>(visibleEntityIDs.begin(), visibleEntityIDs.end());

                //std::cout << "I see " << multi_e.EntityIDs.size() << " entities" << std::endl;
                //ForLease->Dispatcher.DispatchToParent(&multi_e, &Parent);
//                static bool write = true;
//                if(Parent.GetName().compare("GreenLight") == 0 && write) {
//                    Serializer lightMeshWrite;
//                    LightMesh.Serialize(lightMeshWrite);
//                    lightMeshWrite.WriteFile("LightMesh.json");
//                    write = false;
//                }

                return litEntitiyIDs;
            }
        }

        void Light::Serialize(Serializer& root) {
            root.WriteUint("Type", static_cast<unsigned>(ComponentType::Light));
            Serializer light = root.GetChild("Light");
            light.WriteUint("Type", static_cast<unsigned>(ComponentType::Light));
            light.WriteBool("Active", Active);
            light.WriteBool("Visible", Visible);
            light.WriteBool("DrawOutline", DrawOutline);
            light.WriteVec("Offset", Offset);
            light.WriteVec("Direction", Direction);
            light.WriteFloat("Angle", Angle);
            Serializer lightColor = light.GetChild("LightColor");
            LightColor.Serialize(lightColor);
            light.Append(lightColor, "LightColor");
            light.WriteInt("LightMode", LightMode);
            light.WriteFloat("Radius", Radius);
            light.WriteString("LightTexture", LightTexture);
            light.WriteBool("PointLight", PointLight);
            root.Append(light, "Light");
        }

        void Light::Deserialize(Serializer& root) {
            Serializer light = root.GetChild("Light");
            light.ReadBool("Active", Active);
            light.ReadBool("Visible", Visible);
            light.ReadBool("DrawOutline", DrawOutline);
            light.ReadVec("Offset", Offset);
            light.ReadVec("Direction", Direction);
            light.ReadFloat("Angle", Angle);
            Serializer lightColor = light.GetChild("LightColor");
            LightColor.Deserialize(lightColor);
            int lightMode;
            light.ReadInt("LightMode", lightMode);
            lightMode = lightMode;
            light.ReadFloat("Radius", Radius);
            light.ReadString("LightTexture", LightTexture);
            light.ReadBool("PointLight", PointLight);
        }

        Mesh* Light::GetLightMesh() {
            return &LightMesh;
        }

        Point Light::CheckRayAgainstWindow(Ray& ray, Point& tl, Point& tr, Point& br, Point& bl, Point& position) {
            ray.ResetLength();
            HalfPlane top = HalfPlane(tl, tr, tl + Vector(0, 1));
            HalfPlane left = HalfPlane(tl, bl, tl + Vector(-1, 0));
            HalfPlane bottom = HalfPlane(bl, br, br + Vector(0, -1));
            HalfPlane right = HalfPlane(tr, br, br + Vector(1, 0));

            HalfPlane::CollisionInterval topInt = ray.GetHalfPlaneInterval(top);
            HalfPlane::CollisionInterval leftInt = ray.GetHalfPlaneInterval(left);
            HalfPlane::CollisionInterval bottomInt = ray.GetHalfPlaneInterval(bottom);
            HalfPlane::CollisionInterval rightInt = ray.GetHalfPlaneInterval(right);

            Vector rayVec = ray.GetScaledVector();

            Point closest = ray.GetScaledVector() + rayVec;
            float distance2 = rayVec.MagnitudeSquared();

            // Check against top
            if(topInt.Start != 0 && topInt.Start <= topInt.End) {
                Point collide = ray.GetStart() + rayVec * topInt.Start;

                if(Vector::DotProduct(collide - tl, collide - tr) <= 0) {
                    float currDist2 = Point::DistanceSquared(ray.GetStart(), collide);

                    if(currDist2 < distance2)
                        closest = collide;
                }
            }

            // Check against left
            if(leftInt.Start != 0 && leftInt.Start <= leftInt.End) {
                Point collide = ray.GetStart() + rayVec * leftInt.Start;

                if(Vector::DotProduct(collide - tl, collide - bl) <= 0) {
                    float currDist2 = Point::DistanceSquared(ray.GetStart(), collide);

                    if(currDist2 < distance2)
                        closest = collide;
                }
            }

            // Check against bottom
            if(bottomInt.Start != 0 && bottomInt.Start <= bottomInt.End) {
                Point collide = ray.GetStart() + rayVec * bottomInt.Start;

                if(Vector::DotProduct(collide - bl, collide - br) <= 0) {
                    float currDist2 = Point::DistanceSquared(ray.GetStart(), collide);

                    if(currDist2 < distance2)
                        closest = collide;
                }
            }

            // Check against right
            if(rightInt.Start != 0 && rightInt.Start <= rightInt.End) {
                Point collide = ray.GetStart() + rayVec * rightInt.Start;

                if(Vector::DotProduct(collide - tr, collide - br) <= 0) {
                    float currDist2 = Point::DistanceSquared(ray.GetStart(), collide);

                    if(currDist2 < distance2)
                        closest = collide;
                }
            }

            return closest;
        }
    } // Components

} // ForLeaseEngine
