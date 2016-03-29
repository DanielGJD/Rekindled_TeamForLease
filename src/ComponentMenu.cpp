/*!
    \file   ComponentMenu.cpp
    \author Sean McGeer
    \date   11/20/15

    \brief
        Implements the menu component.

    \see ComponentMenu.h

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "ComponentMenu.h"
#include "Engine.h"
#include "State.h"
#include "SoundEmitter.h"
#include <sstream>

namespace ForLeaseEngine {

    namespace Components {

        Menu::Menu(Entity& owner, Vector spacing, bool active, float unfocusedScale, float focusedScale)
            : Component(owner, ComponentType::Transform), UnfocusedScale(unfocusedScale),
              FocusedScale(focusedScale), Spacing(spacing), Active(active), LastActive(0) {
            if (Active) Activate();



        }

        Menu::~Menu() {
            ForLease->Dispatcher.Detach(this, "MouseMotion");
            ForLease->Dispatcher.Detach(this, "MouseButtonDown");
        }

        void Menu::Update() {

        }

        void Menu::OnMouseMotion(const Event* e) {
            for (Entity* rep : Representations) {
                Components::Sprite* sprite = rep->GetComponent<Components::Sprite>(true);
                rep->GetComponent<Components::Transform>()->ScaleX = UnfocusedScale * sprite->SpriteSource.GetWidth() / sprite->SpriteSource.GetHeight();
                rep->GetComponent<Components::Transform>()->ScaleY = UnfocusedScale;
            }

            LastActive = 0;

            const MouseMotionEvent* mouseEvent = static_cast<const MouseMotionEvent *>(e);
            Point position(mouseEvent->X, mouseEvent->Y);
            position = ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Renderer>(true)->ScreenToWorld(position);
            Entity* rep = GetRepresentationAtPosition(position);
            if (rep) {
                Components::Sprite* sprite = rep->GetComponent<Components::Sprite>(true);
                rep->GetComponent<Components::Transform>()->ScaleX = FocusedScale * sprite->SpriteSource.GetWidth() / sprite->SpriteSource.GetHeight();
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

        void Menu::AddItem(MenuItem* item) {
            Items.push_back(item);
            std::cout << Parent.GetName() << " added an item: " << item->Image << std::endl;
        }

        void Menu::AddLoadLevel(std::string image, std::string stateName) {
            Items.push_back(new MenuItems::LoadLevel(image, stateName));
        }

        void Menu::Activate() {
            ForLease->Dispatcher.Attach(NULL, this, "MouseMotion", &Menu::OnMouseMotion);
            ForLease->Dispatcher.Attach(NULL, this, "MouseButtonDown", &Menu::OnMouseDown);

            Active = true;
            State& currentState = ForLease->GameStateManager().CurrentState();

            Point position = Parent.GetComponent<Components::Transform>()->Position;

            for (MenuItem* item : Items) {
                std::stringstream name;
                name << "Menu " << Parent.GetName() << " item " << item->Image;
                std::cout << "Activate: " << name.str() << std::endl;
                Entity* rep = currentState.AddEntity(name.str());
                rep->IncludeInSerialize = false;
                Representations.push_back(rep);
                rep->AddComponent(new Components::Transform(*rep, position, UnfocusedScale, UnfocusedScale, 0, 500));
                rep->AddComponent(new Components::Sprite(*rep));

                //ForLease->Resources.LoadTexture(item->Image);
                //Texture* texture = Texture::CreateTexture(item->Image);
                //TextureRegion textureRegion(texture, 0, texture->GetWidth(), 0, texture->GetHeight());
                //rep->GetComponent<Components::Sprite>(true)->SpriteSource.push_back(textureRegion);

                ///////////////// Chris hacked in some crazy magic here //////////////////////
                //Texture* texture = ForLease->Resources.GetTexture(item->Image);

                //////////////////////////////////////////////////////////////////////////////

                rep->GetComponent<Components::Sprite>(true)->AnimationActive = false;
                Components::Sprite* sprite = rep->GetComponent<Components::Sprite>(true);
                sprite->SetSpriteSource(item->Image);
                rep->GetComponent<Components::Transform>(true)->ScaleX = UnfocusedScale * sprite->SpriteSource.GetWidth() / sprite->SpriteSource.GetHeight();
                position += Spacing * FocusedScale;
                rep->AddComponent(new Components::BackgroundMusic(*rep, "Menu2"));
            }


            std::cout << Parent.GetName() << " activated." << std::endl;
        }

        void Menu::Deactivate() {
            if (Active) {
                ForLease->Dispatcher.Detach(this, "MouseMotion");
                ForLease->Dispatcher.Detach(this, "MouseButtonDown");

                for (Entity* rep : Representations)
                    ForLease->GameStateManager().CurrentState().DeleteEntity(rep->GetName());

                Representations.clear();
                Active = false;
            }
        }

        Entity* Menu::GetRepresentationAtPosition(Point position, bool throwOnFail) {
            for (Entity* entity : Representations) {
                if (!entity->HasComponent(ComponentType::Transform)) continue;
                if (!entity->HasComponent(ComponentType::Sprite)) continue;

                Components::Transform* transform = entity->GetComponent<Components::Transform>(true);
                Components::Sprite* sprite = entity->GetComponent<Components::Sprite>(true);
                float scaleX = transform->ScaleX * sprite->SpriteSource.GetWidth() / sprite->SpriteSource.GetHeight();
                float scaleY = transform->ScaleY;

                ///////////////////////////// More Chris hax /////////////////////////
                //float scaleX = transform->ScaleX;
                //float scaleY = transform->ScaleY;
                //////////////////////////////////////////////////////////////////////

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

        void Menu::Serialize(Serializer& root) {
            root.WriteUint("Type", static_cast<unsigned>(Type));
            Serializer menu = root.GetChild("Menu");
            menu.WriteVec("Spacing", Spacing);
            menu.WriteFloat("UnfocusedScale", UnfocusedScale);
            menu.WriteFloat("FocusedScale", FocusedScale);
            menu.WriteBool("Active", Active);
            ArraySerializer jsonItems(menu);
            jsonItems = menu.GetChild("Items");
            for (MenuItem* item : Items) {
                Serializer itemSerializer;
                item->Serialize(itemSerializer);
                jsonItems.Append(itemSerializer);
            }
            menu.Append(jsonItems, "Items");
            menu.WriteUint("Type", static_cast<unsigned>(Type));
            root.Append(menu, "Menu");
        }

        void Menu::Deserialize(Serializer& root) {
            Serializer menu = root.GetChild("Menu");
            menu.ReadVec("Spacing", Spacing);
            menu.ReadFloat("UnfocusedScale", UnfocusedScale);
            menu.ReadFloat("FocusedScale", FocusedScale);
            menu.ReadBool("Active", Active);
            ArraySerializer jsonItems(menu);
            jsonItems = menu.GetChild("Items");
            for (unsigned i = 0; i < jsonItems.Size(); ++i) {
                Serializer itemSerializer = jsonItems[i];
                AddItem(DeserializeItem(itemSerializer));
            }

            if (Active) Activate();
        }

        MenuItem* Menu::DeserializeItem(Serializer& root) {
            unsigned type;
            std::string image;
            root.ReadUint("Type", type);
            root.ReadString("Image", image);

            MenuItem* item = 0;

            switch (static_cast<MenuItemType>(type)) {
                case MenuItemType::LoadLevel:
                    item = new MenuItems::LoadLevel("", "");
                    break;
                case MenuItemType::NextLevel:
                    item = new MenuItems::NextLevel("");
                    break;
                case MenuItemType::Quit:
                    item = new MenuItems::Quit("");
                    break;
                case MenuItemType::ActivateOther:
                    item = new MenuItems::ActivateOther("", "");
                    break;
                case MenuItemType::ActivateAndDeactivate:
                    item = new MenuItems::ActivateAndDeactivate("", "", "");
                    break;
                case MenuItemType::ResumeGame:
                    item = new MenuItems::ResumeGame("");
                    break;
                case MenuItemType::ActDeactVisible:
                    item = new MenuItems::ActivateAndDeactivateAndMakeVisible("", "", "", "");
                    break;
                case MenuItemType::ActDeactInvisible:
                    item = new MenuItems::ActivateAndDeactivateAndMakeInvisible("", "", "", "");
                    break;
                default:
                    break;
            }

            item->Deserialize(root);

            return item;
        }

    } // Components

} // ForLeaseEngine
