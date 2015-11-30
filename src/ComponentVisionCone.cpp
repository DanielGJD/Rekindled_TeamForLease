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
#include <vector>
#include <iostream>

static const int ViewResolution = 12;

namespace ForLeaseEngine {
    namespace Components {
        VisionCone::VisionCone(Entity& parent, bool active, bool drawOutline, bool visible, Vector const& offset, float radius, Vector const& direction, float angle, Color const& indicatorColor)
                              : Component(parent),
                                Active(active), Visible(visible), DrawOutline(drawOutline),
                                Offset(offset), Radius(radius), Direction(direction),
                                Angle(angle), IndicatorColor(indicatorColor) {}

        VisionCone::~VisionCone() {}

        ComponentType VisionCone::GetType() { return Type; }

        void VisionCone::Update() {
            ViewMesh.ClearData();
            Direction.Normalize();
            Vector start = Vector::Rotate(Direction, -Angle / 2);

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
            std::vector<Entity*> detected = ForLease->GameStateManager().CurrentState().GetEntitiesInCone(trans->Position + Offset, Radius, Direction, Angle);
            for(std::vector<Entity*>::iterator i = detected.begin(); i != detected.end(); ++i) {
                if(*i == &Parent) {
                    detected.erase(i);
                    break;
                }
            }

            if(detected.size() > 0) {
                MultiEntityEvent multi_e = MultiEntityEvent("EntitiesSeen");
                unsigned long parentID = Parent.GetID();
                for(int i = 0; i < detected.size(); ++i) {
                    unsigned long detectedID = detected[i]->GetID();
                    if(detectedID != parentID) { // This if not needed now, should remove
                        // Check for line of sight, this will be slow
                        Collision* collide = detected[i]->GetComponent<Collision>();
                        Transform* targettrans = detected[i]->GetComponent<Transform>();
                        float halfwidth = collide->Width / 2 * targettrans->ScaleX;
                        float halfheight = collide->Height / 2 * targettrans->ScaleY;
                        Point tl = Point(targettrans->Position[0] - halfwidth, targettrans->Position[1] + halfheight);
                        Point tr = Point(targettrans->Position[0] + halfwidth, targettrans->Position[1] + halfheight);
                        Point br = Point(targettrans->Position[0] + halfwidth, targettrans->Position[1] - halfheight);
                        Point bl = Point(targettrans->Position[0] - halfwidth, targettrans->Position[1] - halfheight);

                        render->SetDrawingColor(0, 1, 0);
                        render->SetDebugPointSize(8);

                        Point rayStart = trans->Position + Offset;
                        // Check visibility for top left
                        Ray los = Ray(rayStart, tl - rayStart, Radius);
                        Entity* visible = Ray::CheckCollisions(los, detected);
                        Point rayEnd = los.GetStart() + los.GetScaledVector();
                        render->DrawArrow(rayStart, rayEnd);
                        // If not visible, check for top right
                        if(visible &&  // Didn't hit anything (Due to floating point error)
                           visible->GetID() != detected[i]->GetID() && // Hit something besides the object casting towards
                           Point::DistanceSquared(rayStart, rayEnd) < Point::DistanceSquared(rayStart, tl)) // Closer than the object casting towards
                        {
                            Ray los = Ray(rayStart, tr - rayStart, Radius);
                            visible = Ray::CheckCollisions(los, detected);
                            rayEnd = los.GetStart() + los.GetScaledVector();
                            render->DrawArrow(rayStart, rayEnd);
                            // If not visible, check bottom right
                            if(visible && visible->GetID() != detected[i]->GetID() && Point::DistanceSquared(rayStart, rayEnd) < Point::DistanceSquared(rayStart, tr)) {
                                Ray los = Ray(rayStart, br - rayStart, Radius);
                                visible = Ray::CheckCollisions(los, detected);
                                rayEnd = los.GetStart() + los.GetScaledVector();
                                render->DrawArrow(rayStart, rayEnd);
                                // If not visible, check bottom left
                                if(visible && visible->GetID() != detected[i]->GetID() && Point::DistanceSquared(rayStart, rayEnd) < Point::DistanceSquared(rayStart, br)) {
                                    Ray los = Ray(rayStart, bl - rayStart, Radius);
                                    visible = Ray::CheckCollisions(los, detected);
                                    rayEnd = los.GetStart() + los.GetScaledVector();
                                    render->DrawArrow(rayStart, rayEnd);
                                    // If not visible, entity is not visible
                                    if(visible && visible->GetID() != detected[i]->GetID() && Point::DistanceSquared(rayStart, rayEnd) < Point::DistanceSquared(rayStart, tr)) {
                                        render->SetDrawingColor(1, 0, 0);
                                        render->DrawPoint(rayEnd);
                                        continue;
                                    }
                                    else {
                                        render->DrawPoint(rayEnd);
                                    }
                                }
                                else {
                                    render->DrawPoint(rayEnd);
                                }
                            }
                            else {
                                render->DrawPoint(rayEnd);
                            }
                        }
                        else {
                            render->DrawPoint(rayEnd);
                        }
                        multi_e.EntityIDs.push_back(detectedID);
                    }
                }

                std::cout << "I see " << multi_e.EntityIDs.size() << " entities" << std::endl;
                ForLease->Dispatcher.DispatchTo(&multi_e, &Parent);
            }
        }

        void VisionCone::Serialize(Serializer& root) {
        }

        void VisionCone::Deserialize(Serializer& root) {
        }

        Mesh* VisionCone::GetVisionMesh() {
            return &ViewMesh;
        }
    }
}
