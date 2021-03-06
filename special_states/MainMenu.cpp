/*!
    \file   MainMenu.cpp
    \author Sean McGeer
    \date   11/23/15

    \copyright ęCopyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "MainMenu.h"
#include "ComponentsInclude.h"
#include "ResourceManager.h"
#include "Vector.h"
#include "Face.h"
#include "Edge.h"
#include "Mesh.h"
#include "GameStateManager.h"
#include "Ray.h"
#include "SoundEmitter.h"

#include <iostream>
#include <string>

namespace FLE = ForLeaseEngine;
using namespace ForLeaseEngine;

MainMenu::MainMenu() : State("MainMenu") , MainMenuBGM("woody title 7.5") {}

void MainMenu::Load() {
}

void MainMenu::Initialize() {
    FLE::LevelComponents::Renderer* renderer = new FLE::LevelComponents::Renderer(*this);
    FLE::Entity* camera = AddEntity("Camera");
    camera->AddComponent(new FLE::Components::Transform(*camera, FLE::Point(0, 0), 1, 1, 0));
    camera->AddComponent(new FLE::Components::Camera(*camera, 0, 1, 50));
    renderer->SetCamera(*camera);
    AddLevelComponent(renderer);
    AddLevelComponent(new LevelComponents::Light(*this, Color(0.5f, 0.5f, 0.5f)));
    //AddLevelComponent(new LevelComponents::Physics(*this, Vector(0, -10)));
    //AddLevelComponent(new LevelComponents::Collision(*this));


    Entity* background = AddEntity("Background");
    background->AddComponent(new Components::Transform(*background, 0, 0, 45, 25));
    background->AddComponent(new Components::Sprite(*background));
    //ForLease->Resources.LoadTexture("bg7.png");
    //Texture* texture = Texture::CreateTexture("bg7.png");
    //TextureRegion textureRegion(texture, 0, texture->GetWidth(), 0, texture->GetHeight());
    background->GetComponent<Components::Sprite>(true)->SetSpriteSource("bg7_test2.png");
    background->GetComponent<Components::Sprite>(true)->AnimationActive = false;

    Entity* backgroundMask = AddEntity("BackgroundMask");
    backgroundMask->AddComponent(new Components::Transform(*backgroundMask, 0, 0, 50, 50));
    backgroundMask->AddComponent(new Components::Model(*backgroundMask, true, false, false, "1-1Block.json", "", Color(0.75f, 0.75f, 0.75f), FLE::MULTIPLY));


    //SoundEmitter* emitter = background.GetComponent<SoundEmitter>();
//    if(emitter)
//    {
//        emitter->SetVolume(1.0f, MainMenuBGM);
//        emitter->StopEvent(MainMenuBGM);
//        emitter->PlayEvent(MainMenuBGM);
//        std::cout<< "MENU2 EMITTER HERE" << std::endl;
//    }
//    if(!emitter)
//    {
//        std::cout << "no menu sound" <<std::endl;
//    }
   // background->GetComponent<Components::SoundEmitter>(true)->SetVolume(1.0f, "Menu2");
    //background->GetComponent<Components::SoundEmitter>(true)->StopEvent("Menu2");
    //background->GetComponent<Components::SoundEmitter>(true)->PlayEvent("Menu2");
    //ForLease->Resources.LoadTexture("bg7.png");
    //Texture* texture = Texture::CreateTexture("bg7.png");
    //TextureRegion textureRegion(texture, 0, texture->GetWidth(), 0, texture->GetHeight());
    //background->GetComponent<Components::Sprite>(true)->SetSpriteSource("bg7.png");
    //background->GetComponent<Components::Sprite>(true)->AnimationActive = false;

    Entity* player = AddEntity("Player");
    player->AddComponent(new Components::Transform(*player, Point(25.0f, -11.0f), 15, 15));
    Components::Model* model = new Components::Model(*player, true, false, true, "foxSit.json");
    model->SetAnimation("foxSitAni.json");
    model->AnimationActive = true;
    model->Looping = true;
    model->FrameRate = 3.0f;
    player->AddComponent(model);

    Entity* logo = AddEntity("Logo");
    logo->AddComponent(new Components::Transform(*logo, Point(0, 10), 26, 26));
    logo->AddComponent(new Components::Sprite(*logo));
    logo->GetComponent<Components::Sprite>(true)->SetSpriteSource("title1b.png");
    logo->GetComponent<Components::Sprite>(true)->AnimationActive = false;

    Entity* creditsPic = AddEntity("CreditsPic");
    creditsPic->AddComponent(new Components::Transform(*creditsPic, Point(0, 0), 30.0f, 30.0f, 0.0f));
    creditsPic->AddComponent(new Components::Sprite(*creditsPic, "Credits.png", false));

    Entity* follow = SpawnArchetype(ForLease->Filesystem.AssetDirectory(Modules::Filesystem::AssetType::Blueprint) + "Wisp", Point(-35.0f, -6.0f), "MenuFollow");
    follow->GetComponent<Components::Follow>()->Offset = Vector(-1.5f, -1.0f);
    follow->GetComponent<Components::Follow>()->Speed = 4;
    follow->GetComponent<Components::Light>()->Radius = 5.0f;

    Entity* l1 = AddEntity("Light1");
    l1->AddComponent(new Components::Transform(*l1, Point(-15.0f, 10.0f)));
    l1->AddComponent(new Components::Light(*l1, true, true, false, Vector(), Vector(-1,0), 2 * PI, Color(0.5f, 0.3f, 0.0f, 1.0f), ADDITIVE, 15.0f, "LinearFalloff.png", true));

    Entity* l2 = AddEntity("Light2");
    l2->AddComponent(new Components::Transform(*l2, Point(0.0f, 9.0f)));
    l2->AddComponent(new Components::Light(*l2, true, true, false, Vector(), Vector(-1, 0), 2 * PI, Color(0.5f, 0.5f, 0.0f, 1.0f), ADDITIVE, 13.0f, "LinearFalloff.png", true));

    Entity* l3 = AddEntity("Light3");
    l3->AddComponent(new Components::Transform(*l3, Point(15.0f, 10.0f)));
    l3->AddComponent(new Components::Light(*l3, true, true, false, Vector(), Vector(-1, 0), 2 * PI, Color(0.5f, 0.5f, 0.3f, 1.0f), ADDITIVE, 13.0f, "LinearFalloff.png", true));

    Entity* menu = AddEntity("Menu");
    menu->AddComponent(new Components::Transform(*menu, Point(-35.0f, -6.0f)));
    menu->AddComponent(new Components::Menu(*menu, Vector(0, -1), true, 2.0f, 2.5f, "Liberation_Serif.fnt", "MenuFollow", Color(1, 1, 1), Color(1, 1, 0)));
    Components::Menu* menuComp = menu->GetComponent<Components::Menu>();
    menuComp->AddItem(new MenuItems::NextLevel("Play"));
    menuComp->AddItem(new MenuItems::LoadLevel("How to Play", "HowToPlay"));
    menuComp->AddItem(new MenuItems::ActivateAndDeactivate("Options", "OptionsMenu", "Menu"));
    menuComp->AddItem(new MenuItems::ActivateAndDeactivateAndMakeVisibleAndMakeInvisible("Credits", "CreditsMenu", "Menu", "CreditsPic", "Logo"));
    menuComp->AddItem(new MenuItems::ActivateAndDeactivate("Quit", "QuitConfirm", "Menu"));
    menuComp->Activate();

    Entity* opMenu = AddEntity("OptionsMenu");
    opMenu->AddComponent(new Components::Transform(*opMenu, Point(-35.0f, -6.0f)));
    opMenu->AddComponent(new Components::Menu(*opMenu, Vector(0, -1), false, 2.0f, 2.5f, "Liberation_Serif.fnt", "MenuFollow", Color(1, 1, 1), Color(1, 1, 0)));
    Components::Menu* opMenuComp = opMenu->GetComponent<Components::Menu>();
    opMenuComp->AddItem(new OptionMenuItems::Resolution());
    opMenuComp->AddItem(new OptionMenuItems::Fullscreen());
    //opMenuComp->AddItem(new OptionMenuItems::Volume());
    opMenuComp->AddItem(new OptionMenuItems::ToggleAudio());
    opMenuComp->AddItem(new OptionMenuItems::FinalAccept("OptionsMenu"));
    opMenuComp->AddItem(new MenuItems::ActivateAndDeactivate("Back", "Menu", "OptionsMenu"));

    Entity* quitConfirm = AddEntity("QuitConfirm");
    quitConfirm->AddComponent(new Components::Transform(*quitConfirm, Point(-35.0f, -13.0f)));
    quitConfirm->AddComponent(new Components::Menu(*quitConfirm, Vector(0, -1), false, 2.0f, 2.5f, "Liberation_Serif.fnt", "MenuFollow", Color(1, 1, 1), Color(1, 1, 0)));
    Components::Menu* quitConfirmComp = quitConfirm->GetComponent<Components::Menu>();
    quitConfirmComp->AddItem(new MenuItems::Quit("Quit"));
    quitConfirmComp->AddItem(new MenuItems::ActivateAndDeactivate("Cancel", "Menu", "QuitConfirm"));

    Entity* creditsMenu = AddEntity("CreditsMenu");
    creditsMenu->AddComponent(new Components::Transform(*creditsMenu, Point(-35.0f, -13.0f)));
    creditsMenu->AddComponent(new Components::Menu(*creditsMenu, Vector(0, -1), false, 2.0f, 2.5f, "Liberation_Serif.fnt", "MenuFollow", Color(1, 1, 1), Color(1, 1, 0)));
    Components::Menu* creditsMenuComp = creditsMenu->GetComponent<Components::Menu>();
    //creditsMenuComp->AddItem(new MenuItems::Quit("Quit"));
    creditsMenuComp->AddItem(new MenuItems::ActivateAndDeactivateAndMakeVisibleAndMakeInvisible("Back", "Menu", "CreditsMenu", "Logo", "CreditsPic"));

    Entity* bgMusic = AddEntity("BackgroundMusic");
    bgMusic->AddComponent(new Components::BackgroundMusic(*bgMusic, "woody title 7.5"));
}

void MainMenu::Update() {
    ForLease->sound->Update();

    ForLease->OSInput.ProcessAllInput();

    for (FLE::Entity* entity : Entities) {
        entity->Update();
    }

    Entity* mainMenu = GetEntityByName("Menu");
    Entity* optionsMenu = GetEntityByName("OptionsMenu");
    Entity* quitMenu = GetEntityByName("QuitConfirm");
    if (mainMenu->GetComponent<Components::Menu>()->Active
        || optionsMenu->GetComponent<Components::Menu>()->Active
        || quitMenu->GetComponent<Components::Menu>()->Active) {
        GetEntityByName("Light1")->GetComponent<Components::Light>()->Active = true;
        GetEntityByName("Light2")->GetComponent<Components::Light>()->Active = true;
        GetEntityByName("Light3")->GetComponent<Components::Light>()->Active = true;
    } else {
        GetEntityByName("Light1")->GetComponent<Components::Light>()->Active = false;
        GetEntityByName("Light2")->GetComponent<Components::Light>()->Active = false;
        GetEntityByName("Light3")->GetComponent<Components::Light>()->Active = false;
    }

    for (FLE::LevelComponent* levelComponent : LevelComponents) {
        levelComponent->Update(Entities);
    }

    LevelComponents::Renderer* renderer = ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Renderer>();

    ForLease->GameWindow->UpdateGameWindow();
}

void MainMenu::Deinitialize() {
    DeleteAllEntities();
    DeleteAllLevelComponents();
}

void MainMenu::Unload() {}
