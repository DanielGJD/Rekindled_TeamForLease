/*!
    \file   DigiPenLogo.cpp
    \author Sean McGeer
    \date   03/03/16

    \copyright ©Copyright 2016 DigiPen Institute of Technology, All Rights Reserved
*/

#include "DigiPenLogo.h"
#include "ComponentsInclude.h"
#include "ResourceManager.h"
#include "Vector.h"
#include "Face.h"
#include "Edge.h"
#include "Mesh.h"
#include "GameStateManager.h"
#include "Ray.h"
#include "MouseButtonEvent.h"
#include "Interpolation.h"

#include <iostream>
#include <string>

namespace FLE = ForLeaseEngine;
using namespace ForLeaseEngine;

DigiPenLogo::DigiPenLogo() : State("DigiPenLogo") {}

void DigiPenLogo::Load() {
}

void DigiPenLogo::Initialize() {
    FLE::LevelComponents::Renderer* renderer = new FLE::LevelComponents::Renderer(*this);
    FLE::Entity* camera = AddEntity("Camera");
    camera->AddComponent(new FLE::Components::Transform(*camera, FLE::Point(0, 0), 1, 1, 0));
    camera->AddComponent(new FLE::Components::Camera(*camera, 0, 1, 50));
    renderer->SetCamera(*camera);
    AddLevelComponent(renderer);
    AddLevelComponent(new LevelComponents::Light(*this, Color(0.0f, 0.0f, 0.0f, 1.0f)));
    //AddLevelComponent(new LevelComponents::Physics(*this, Vector(0, -10)));
    //AddLevelComponent(new LevelComponents::Collision(*this));


    //Entity* background = AddEntity("Background");
    //background->AddComponent(new Components::Transform(*background, 0, 0, 45, 25));
    //background->AddComponent(new Components::Sprite(*background));
    ////ForLease->Resources.LoadTexture("bg7.png");
    ////Texture* texture = Texture::CreateTexture("bg7.png");
    ////TextureRegion textureRegion(texture, 0, texture->GetWidth(), 0, texture->GetHeight());
    //background->GetComponent<Components::Sprite>(true)->SetSpriteSource("bg7_test2.png");
    //background->GetComponent<Components::Sprite>(true)->AnimationActive = false;

    //Entity* backgroundMask = AddEntity("BackgroundMask");
    //backgroundMask->AddComponent(new Components::Transform(*backgroundMask, 0, 0, 50, 50));
    //backgroundMask->AddComponent(new Components::Model(*backgroundMask, true, false, false, "1-1Block.json", "", Color(0.75f, 0.75f, 0.75f), FLE::MULTIPLY));

    Entity* background = AddEntity("Background");
    background->AddComponent(new Components::Transform(*background, 0, 0, 50, 50));
    background->AddComponent(new Components::Model(*background, true, false, false, "1-1Block.json", "", Color(0, 0, 0)));

    Entity* logo = AddEntity("Logo");
    logo->AddComponent(new Components::Transform(*logo, Point(0, 0), 20, 20));
    logo->AddComponent(new Components::Sprite(*logo));
    logo->GetComponent<Components::Sprite>(true)->SetSpriteSource("DigiPen_White_1024.png");
    //logo->GetComponent<Components::Sprite>(true)->SpriteColor = Color(1, 1, 1, 1);
    //logo->GetComponent<Components::Sprite>(true)->BlendingMode = ALPHA;
    logo->GetComponent<Components::Sprite>(true)->AnimationActive = false;

//    Entity* follow = SpawnArchetype(ForLease->Filesystem.AssetDirectory(Modules::Filesystem::AssetType::Blueprint) + "Wisp", Point(-25.0f, 0.0f), "MenuFollow");
//    follow->GetComponent<Components::Follow>()->Offset = Vector(-2.0f, 2.5f);
//    follow->GetComponent<Components::Light>()->Radius = 5.0f;
    //follow->GetComponent<Components::Follow>()->FollowEntityID = player->GetID();
    //follow->GetComponent<Components::Transform>()->Position = player->GetComponent<Components::Transform>()->Position + follow->GetComponent<Components::Follow>()->Offset;
//
//    Entity* light = AddEntity("Light");
//    light->AddComponent(new Components::Transform(*light, Point(-44,0)));
//    light->AddComponent(new Components::Light(*light, true, true, false, Vector(), Vector(1, 0)));
//    Components::Light* lightComp = light->GetComponent<Components::Light>();
//    lightComp->LightColor = Color(1, 1, 1, 1);

    CurrentFadeState = FadeState::FadingIn;

    ForLease->Dispatcher.Attach(NULL, this, "MouseButtonDown", &DigiPenLogo::OnMouseButtonEvent, NULL);


    //Entity* menu = AddEntity("Menu");
    //menu->AddComponent(new Components::Transform(*menu, 0, 0));
    //menu->AddComponent(new Components::Menu(*menu, Vector(0, -3)));
    //Components::Menu* menuComp = menu->GetComponent<Components::Menu>();
    //menuComp->AddItem(new MenuItems::NextLevel("ButtonPlay.png"));
    //menuComp->AddItem(new MenuItems::LoadLevel("ButtonHowTo.png", "HowToPlay"));
    //menuComp->AddItem(new MenuItems::ActivateAndDeactivate("ButtonQuit.png", "QuitConfirm", "Menu"));
    //menuComp->Activate();

    //Entity* quitConfirm = AddEntity("QuitConfirm");
    //quitConfirm->AddComponent(new Components::Transform(*quitConfirm));
    //quitConfirm->AddComponent(new Components::Menu(*quitConfirm));
    //Components::Menu* quitConfirmComp = quitConfirm->GetComponent<Components::Menu>();
    //quitConfirmComp->AddItem(new MenuItems::Quit("ButtonQuit.png"));
    //quitConfirmComp->AddItem(new MenuItems::ActivateAndDeactivate("ButtonCancel.png", "Menu", "QuitConfirm"));
}

void DigiPenLogo::Update() {

    ForLease->OSInput.ProcessAllInput();

    float dt = ForLease->FrameRateController().GetDt();

//    Entity* light = GetEntityByName("Light");
//    Components::Light* lightComp = light->GetComponent<Components::Light>();
//    Color currColor = lightComp->LightColor;
//    float currAlpha = currColor.GetA();

    LevelComponents::Light* lcLightComp = GetLevelComponent<LevelComponents::Light>();
    float currAlpha = lcLightComp->AmbientLight.GetA();

    if (CurrentFadeState == FadeState::FadingIn) {
        currAlpha -= (1 / FadeInTime) * dt;
        if (currAlpha <= 0) {
            currAlpha = 0;
            CurrentFadeState = FadeState::Hold;
        }
        lcLightComp->AmbientLight = Color(0, 0, 0, currAlpha);
    } else if (CurrentFadeState == FadeState::Hold) {
        HoldTime -= dt;
        if (HoldTime <= 0)
            CurrentFadeState = FadeState::FadingOut;
        lcLightComp->AmbientLight = Color(0,0,0,0);
    } else if (CurrentFadeState == FadeState::FadingOut) {
        currAlpha += (1 / FadeOutTime) * dt;
        if (currAlpha >= 1) {
            currAlpha = 1;
            ForLease->GameStateManager().SetAction(ForLeaseEngine::Modules::StateAction::Next);
        }
        lcLightComp->AmbientLight = Color(0,0,0,currAlpha);
    }

//    if (CurrentFadeState == FadeState::FadingIn) {
//        currAlpha += (1 / FadeInTime) * dt;
//        if (currAlpha >= 1) {
//            currAlpha = 1;
//            CurrentFadeState = FadeState::Hold;
//        }
//        lightComp->LightColor = Color(1, 1, 1, currAlpha);
//        lcLightComp->AmbientLight = Color(0, 0, 0, 1 - (currAlpha * .5));
//    } else if (CurrentFadeState == FadeState::Hold) {
//        HoldTime -= dt;
//        if (HoldTime <= 0)
//            CurrentFadeState = FadeState::FadingOut;
//        lcLightComp->AmbientLight = Color(0, 0, 0, 0.5);
//    } else if (CurrentFadeState == FadeState::FadingOut) {
//        currAlpha -= (1 / FadeOutTime) * dt;
//        if (currAlpha <= 0) {
//            currAlpha = 0;
//            ForLease->GameStateManager().SetAction(ForLeaseEngine::Modules::StateAction::Next);
//        }
//        lightComp->LightColor = Color(1, 1, 1, currAlpha);
//        lcLightComp->AmbientLight = Color(0, 0, 0, 1 - (currAlpha * 0.5));
//    }

    for (FLE::Entity* entity : Entities) {
        entity->Update();
    }

    for (FLE::LevelComponent* levelComponent : LevelComponents) {
        levelComponent->Update(Entities);
    }

//    LevelComponents::Renderer* renderer = ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Renderer>();

    ForLease->GameWindow->UpdateGameWindow();
}

void DigiPenLogo::Deinitialize() {
    DeleteAllEntities();
    DeleteAllLevelComponents();
    ForLease->Dispatcher.Detach(this, "MouseButtonDown");
}

void DigiPenLogo::Unload() {}

void DigiPenLogo::OnMouseButtonEvent(const ForLeaseEngine::Event* e) {
    const MouseButtonEvent* mouse_e = reinterpret_cast<const MouseButtonEvent*>(e);
    if (mouse_e->Button == 1) ForLease->GameStateManager().SetAction(ForLeaseEngine::Modules::StateAction::Next);
}
