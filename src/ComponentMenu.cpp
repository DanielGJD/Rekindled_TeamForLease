/*!
    \file   ComponentMenu.cpp
    \author Sean McGeer
    \date   11/20/15

    \brief
        Implements the menu component.

    \see ComponentMenu.h
*/

#include "ComponentMenu.h"
#include "Engine.h"
#include "State.h"
#include <sstream>

namespace ForLeaseEngine {

    namespace Components {

        Menu::Menu(Entity& owner, float unfocusedScale, float focusedScale, Vector spacing, bool active)
            : Component(owner, ComponentType::Transform), UnfocusedScale(unfocusedScale),
              FocusedScale(focusedScale), Spacing(spacing), Active(active), LastActive(0) {
            if (Active) Activate();

            ForLease->Dispatcher.Attach(NULL, this, "MouseMotion", &Menu::OnMouseMotion);
            ForLease->Dispatcher.Attach(NULL, this, "MouseButtonDown", &Menu::OnMouseDown);
        }

        void Menu::Update() {

        }

        void Menu::OnMouseMotion(const Event* e) {
            for (Entity* rep : Representations) {
                rep->GetComponent<Components::Transform>()->ScaleX = UnfocusedScale;
                rep->GetComponent<Components::Transform>()->ScaleY = UnfocusedScale;
            }

            LastActive = 0;

            const MouseMotionEvent* mouseEvent = static_cast<const MouseMotionEvent *>(e);
            Point position(mouseEvent->X, mouseEvent->Y);
            position = ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Renderer>(true)->ScreenToWorld(position);
            Entity* rep = GetRepresentationAtPosition(position);
            if (rep) {
                rep->GetComponent<Components::Transform>()->ScaleX = FocusedScale;
                rep->GetComponent<Components::Transform>()->ScaleY = FocusedScale;
                LastActive = rep;
            }
        }

        void Menu::OnMouseDown(const Event* e) {
            for (unsigned i = 0; i < Representations.size(); ++i) {
                if (Representations[i] == LastActive) {
                    Items[i]->Action();
                }
            }
        }

        void Menu::AddLoadLevel(std::string text, std::string stateName) {
            Items.push_back(new MenuItems::LoadLevel(text, stateName));
        }

        void Menu::Activate() {
            Active = true;
            State& currentState = ForLease->GameStateManager().CurrentState();

            Point position = Parent.GetComponent<Components::Transform>()->Position;
            
            for (MenuItem* item : Items) {
                std::stringstream name;
                name << "Menu " << Parent.GetName() << " item " << item->Text;
                Entity* rep = currentState.AddEntity(name.str());
                Representations.push_back(rep);
                rep->AddComponent(new Components::Transform(*rep, position, UnfocusedScale, UnfocusedScale));
                rep->AddComponent(new Components::Sprite(*rep));
                ForLease->Resources.LoadTexture("ButtonTemplate.png");
                Texture* texture = Texture::CreateTexture("ButtonTemplate.png");
                TextureRegion textureRegion(texture, 0, texture->GetWidth(), 0, texture->GetHeight());
                rep->GetComponent<Components::Sprite>(true)->SpriteSource.push_back(textureRegion);
                rep->GetComponent<Components::Sprite>(true)->AnimationActive = false;
                position += Spacing * UnfocusedScale * texture->GetHeight();
            }
        }

        void Menu::Deactivate() {
            for (Entity* rep : Representations) {
                ForLease->GameStateManager().CurrentState().DeleteEntity(rep->GetID());
            }
            Active = false;
        }

        Entity* Menu::GetRepresentationAtPosition(Point position, bool throwOnFail) {
            for (Entity* entity : Representations) {
                if (!entity->HasComponent(ComponentType::Transform)) continue;
                if (!entity->HasComponent(ComponentType::Sprite)) continue;

                Components::Transform* transform = entity->GetComponent<Components::Transform>(true);
                Components::Sprite* sprite = entity->GetComponent<Components::Sprite>(true);
                float scaleX = transform->ScaleX * sprite->SpriteSource[0].GetWidth();
                float scaleY = transform->ScaleY * sprite->SpriteSource[0].GetHeight();

                //std::cout
                //    << (position[0] > transform->Position[0] - scaleX)
                //    << (position[0] < transform->Position[0] + scaleX)
                //    << (position[1] > transform->Position[1] - scaleY)
                //    << (position[1] < transform->Position[1] + scaleY)
                //    << std::endl;

                //LevelComponents::Renderer* renderer = ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Renderer>();

                //renderer->DrawRectangle(transform->Position, scaleX, scaleY);
                //renderer->DrawRectangleFilled(position, 10, 10);
                //std::cout << position << std::endl;

                if (position[0] > transform->Position[0] - scaleX &&
                    position[0] < transform->Position[0] + scaleX &&
                    position[1] > transform->Position[1] - scaleY &&
                    position[1] < transform->Position[1] + scaleY)
                    return entity;
            }

            if (throwOnFail) {
                std::stringstream ss;
                ss << "No entity found at point " << position << ".";

                throw EntityNotFoundException(0, ss.str());
            }

            else return 0;
        }

        void Menu::Serialize(Serializer& root) {}
        void Menu::Deserialize(Serializer& root) {}

    } // Components

} // ForLeaseEngine