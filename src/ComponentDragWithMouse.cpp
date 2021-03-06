/*!
    \file   ComponentDragWithMouse.cpp
    \author Christopher Hudson

    \brief
        Defines a component to allow clicking and dragging with the mouse

    \copyright ęCopyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "ComponentDragWithMouse.h"
#include "MouseButtonEvent.h"
#include "MouseMotionEvent.h"
#include "Engine.h"
#include "ComponentTransform.h"
#include "ComponentCollision.h"
#include "LevelComponentRenderer.h"
#include "Vector.h"
#include "Entity.h"
#include "Mouse.h"
#include <iostream>

namespace ForLeaseEngine {
    namespace Components {
        DragWithMouse::DragWithMouse(Entity& owner) : Component(owner, ComponentType::Transform | ComponentType::Collision | ComponentType::Physics), Active(false), MouseDown(false) {
        }

        DragWithMouse::~DragWithMouse() {
            ForLease->Dispatcher.Detach(this, "MouseButtonDown");
            ForLease->Dispatcher.Detach(this, "MouseButtonUp");
            ForLease->Dispatcher.Detach(this, "MouseMotion");
        }

        DragWithMouse* DragWithMouse::Create(Entity& owner) {
            DragWithMouse* mouseDrag = new DragWithMouse(owner);
            mouseDrag->Initialize();
            return mouseDrag;
        }

        ComponentType DragWithMouse::GetType() {
            return Type;
        }

        void DragWithMouse::Update() {
            if(MouseDown && Active) {
                Components::Physics* phy = Parent.GetComponent<Components::Physics>();
                Components::Transform* trans = Parent.GetComponent<Components::Transform>();

                // EXPERIMENTAL CODE: KEEP OBJECTS INSIDE A RADIUS OF PLAYER
                Entity* player = ForLease->GameStateManager().CurrentState().GetEntityByName("Player");
                Components::TransformModeControls* controls = player->GetComponent<Components::TransformModeControls>();
                float radius;
                if(!controls)
                    radius = 5;
                else
                    radius = controls->InfluenceRadius;
                if(player) {
                    Point playerLoc = player->GetComponent<Components::Transform>()->Position;
                    float distance = Point::Distance(playerLoc, Target);
                    if(distance > radius) {
                        Vector direction = Target - playerLoc;
                        direction.Normalize();
                        Target = playerLoc + direction * radius;
                    }
                }
                else {
                    std::cout << "Player not found" << std::endl;
                }
                //////////////////////////////////////////////////////////////

                phy->Velocity = (Target - trans->Position) * (1 / ForLease->FrameRateController().GetDt() / 2); // This is wildly unstable with fluctuating frame rate!

            }
        }

        void DragWithMouse::Serialize(Serializer& root) {
            root.WriteUint("Type", static_cast<unsigned>(Type));
            Serializer dragWithMouse = root.GetChild("DragWithMouse");
            dragWithMouse.WriteBool("Active", Active);
            dragWithMouse.WriteUint("Type", static_cast<unsigned>(Type));
            root.Append(dragWithMouse, "DragWithMouse");
        }

        void DragWithMouse::Deserialize(Serializer& root) {
            Serializer dragWithMouse = root.GetChild("DragWithMouse");
            dragWithMouse.ReadBool("Active", Active);
        }

        void DragWithMouse::Initialize() {
            ForLease->Dispatcher.Attach(NULL, this, "MouseButtonDown", &DragWithMouse::OnMouseDown);
            ForLease->Dispatcher.Attach(NULL, this, "MouseButtonUp", &DragWithMouse::OnMouseUp);
            ForLease->Dispatcher.Attach(NULL, this, "MouseMotion", &DragWithMouse::OnMouseMove);
        }

        void DragWithMouse::OnMouseDown(Event const* e) {
            MouseButtonEvent const* mouse_e = static_cast<MouseButtonEvent const*>(e);
            if(mouse_e->Button == MouseButton::Left) {
                State& state = ForLease->GameStateManager().CurrentState();
                LevelComponents::Renderer* render = state.GetLevelComponent<LevelComponents::Renderer>();
                Point location = render->ScreenToWorld(Point(mouse_e->ScreenLocation));
                Components::Collision* collider = Parent.GetComponent<Components::Collision>();
                Components::Transform* trans = Parent.GetComponent<Components::Transform>();
                if(location[0] >= trans->Position[0] - collider->Width * trans->ScaleX / 2 &&
                   location[0] <= trans->Position[0] + collider->Width * trans->ScaleX / 2 &&
                   location[1] >= trans->Position[1] - collider->Height * trans->ScaleY / 2 &&
                   location[1] <= trans->Position[1] + collider->Height * trans->ScaleY / 2) {
                    MouseDown = true;
                }
            }
        }

        void DragWithMouse::OnMouseUp(Event const*) {
            //MouseButtonEvent const* mouse_e = static_cast<MouseButtonEvent const*>(e);
            if(MouseDown) {
                Parent.GetComponent<Components::Physics>()->Velocity = Vector();
                Target = Parent.GetComponent<Components::Transform>()->Position;
            }
            MouseDown = false;
        }

        void DragWithMouse::OnMouseMove(Event const* e) {
            if(Active) {
                MouseMotionEvent const* mouse_e = static_cast<MouseMotionEvent const*>(e);

                if(MouseDown) {
                    //std::cout << "MOVING OBJECT" << std::endl;
                    //Components::Transform* trans = Parent.GetComponent<Components::Transform>();
                    //Components::Physics* phy = Parent.GetComponent<Components::Physics>();
                    //Point old = ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Renderer>()->ScreenToWorld(Point(mouse_e->X - mouse_e->RelativeX, mouse_e->Y - mouse_e->RelativeY));
                    //Point current = ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Renderer>()->ScreenToWorld(Point(mouse_e->X, mouse_e->Y));
                    //trans->Position += current - old;
                    //phy->Velocity = current - trans->Position;
                    Target = ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Renderer>()->ScreenToWorld(Point(mouse_e->X, mouse_e->Y));
                }
                else {
                    //Parent.GetComponent<Components::Physics>()->Velocity = Vector();
                }
            }
        }
    }
}
