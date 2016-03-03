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
    AddLevelComponent(new LevelComponents::Physics(*this, Vector(0, -10)));
    AddLevelComponent(new LevelComponents::Collision(*this));
    AddLevelComponent(new LevelComponents::Light(*this, Color(0, 0, 0, 1)));


    Entity* background = AddEntity("Background");
    background->AddComponent(new Components::Transform(*background, 0, 0, 50, 50, 0, -20));
    background->AddComponent(new Components::Sprite(*background));
    background->GetComponent<Components::Sprite>(true)->SetSpriteSource("bg7.png");
    background->GetComponent<Components::Sprite>(true)->AnimationActive = false;

    Entity* logo = AddEntity("Logo");
    logo->AddComponent(new Components::Transform(*logo, Point(0, 0), 20, 20));
    logo->AddComponent(new Components::Sprite(*logo));
    logo->GetComponent<Components::Sprite>(true)->SetSpriteSource("DigiPenWhiteRough.png");
    logo->GetComponent<Components::Sprite>(true)->AnimationActive = false;

    Entity* light = AddEntity("Light");
    light->AddComponent(new Components::Transform(*light, Point(0, 25)));
    light->AddComponent(new Components::Light(*light));
    Components::Light* lightComp = light->GetComponent<Components::Light>();
    lightComp->LightColor = Color(1, 1, 1, 0);

    CurrentFadeState = FadingIn;


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

    Entity* light = GetEntityByName("Light");
    Components::Light* lightComp = light->GetComponent<Components::Light>();
    Color currColor = lightComp->LightColor;
    float currAlpha = currColor.GetA();

    LevelComponents::Light* lcLightComp = GetLevelComponent<LevelComponents::Light>();

    if (CurrentFadeState == FadeState::FadingIn) {
        currAlpha += (1 / FadeInTime) * dt;
        if (currAlpha >= 1) {
            currAlpha = 1;
            CurrentFadeState = FadeState::Hold;
        }
        lightComp->LightColor = Color(1, 1, 1, currAlpha);
        lcLightComp->AmbientLight = Color(0, 0, 0, 1 - (currAlpha * .5));
    } else if (CurrentFadeState == FadeState::Hold) {
        HoldTime -= dt;
        if (HoldTime <= 0)
            CurrentFadeState = FadeState::FadingOut;
        lcLightComp->AmbientLight = Color(0, 0, 0, 0.5);
    } else if (CurrentFadeState == FadeState::FadingOut) {
        currAlpha -= (1 / FadeOutTime) * dt;
        if (currAlpha <= 0) {
            currAlpha = 0;
            ForLease->GameStateManager().SetAction(ForLeaseEngine::Modules::StateAction::Next);
        }
        lightComp->LightColor = Color(1, 1, 1, currAlpha);
        lcLightComp->AmbientLight = Color(0, 0, 0, 1 - (currAlpha * 0.5));
    }

    for (FLE::Entity* entity : Entities) {
        entity->Update();
    }

    for (FLE::LevelComponent* levelComponent : LevelComponents) {
        levelComponent->Update(Entities);
    }

    LevelComponents::Renderer* renderer = ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Renderer>();

    ForLease->GameWindow->UpdateGameWindow();
}

void DigiPenLogo::Deinitialize() {
    DeleteAllEntities();
    DeleteAllLevelComponents();
}

void DigiPenLogo::Unload() {}
