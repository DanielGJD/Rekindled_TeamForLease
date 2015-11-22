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
        DragWithMouse::DragWithMouse(Entity& owner) : Component(owner, ComponentType::Transform | ComponentType::Collision), Active(false), MouseDown(false) {
        }

        DragWithMouse::~DragWithMouse() {
            ForLease->Dispatcher.Detach(&Parent, "MouseButtonDown");
            ForLease->Dispatcher.Detach(&Parent, "MouseButtonUp");
            ForLease->Dispatcher.Detach(&Parent, "MouseMotion");
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
                if(location[0] >= trans->Position[0] - collider->Width / 2 &&
                   location[0] <= trans->Position[0] + collider->Width / 2 &&
                   location[1] >= trans->Position[1] - collider->Height / 2 &&
                   location[1] <= trans->Position[1] + collider->Height / 2)
                    MouseDown = true;
            }
        }

        void DragWithMouse::OnMouseUp(Event const*) {
            //MouseButtonEvent const* mouse_e = static_cast<MouseButtonEvent const*>(e);
            MouseDown = false;
        }

        void DragWithMouse::OnMouseMove(Event const* e) {
            if(Active) {
                MouseMotionEvent const* mouse_e = static_cast<MouseMotionEvent const*>(e);

                if(MouseDown) {
                    Components::Transform* trans = Parent.GetComponent<Components::Transform>();
                    Point old = ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Renderer>()->ScreenToWorld(Point(mouse_e->X - mouse_e->RelativeX, mouse_e->Y - mouse_e->RelativeY));
                    Point current = ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Renderer>()->ScreenToWorld(Point(mouse_e->X, mouse_e->Y));
                    trans->Position += current - old;
                }
            }
        }
    }
}
