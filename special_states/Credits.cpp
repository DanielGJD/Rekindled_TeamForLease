/*!
    \file   Credits.cpp
    \author Sean McGeer
    \date   4/05/15

    \copyright ©Copyright 2016 DigiPen Institute of Technology, All Rights Reserved
*/

#include "Credits.h"
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

Credits::Credits() : State("Credits"), MainMenuBGM("Menu2") {}

void Credits::Load() {
}

void Credits::Initialize() {
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

    Components::SoundEmitter* emitter = background->GetComponent<Components::SoundEmitter>();
    //SoundEmitter* emitter = background.GetComponent<SoundEmitter>();
    if(emitter)
    {
        emitter->SetVolume(1.0f, MainMenuBGM);
        emitter->StopEvent(MainMenuBGM);
        emitter->PlayEvent(MainMenuBGM);
        std::cout<< "MENU2 EMITTER HERE" << std::endl;
    }
    if(!emitter)
    {
        std::cout << "no menu sound" <<std::endl;
    }
   // background->GetComponent<Components::SoundEmitter>(true)->SetVolume(1.0f, "Menu2");
    //background->GetComponent<Components::SoundEmitter>(true)->StopEvent("Menu2");
    //background->GetComponent<Components::SoundEmitter>(true)->PlayEvent("Menu2");
    //ForLease->Resources.LoadTexture("bg7.png");
    //Texture* texture = Texture::CreateTexture("bg7.png");
    //TextureRegion textureRegion(texture, 0, texture->GetWidth(), 0, texture->GetHeight());
    //background->GetComponent<Components::Sprite>(true)->SetSpriteSource("bg7.png");
    //background->GetComponent<Components::Sprite>(true)->AnimationActive = false;

    Entity* player = AddEntity("Player");
    player->AddComponent(new Components::Transform(*player, Point(15, -15), 15, 15));
    Components::Model* model = new Components::Model(*player, true, false, true, "fox.json");
    model->SetAnimation("a");
    model->AnimationActive = true;
    model->Looping = true;
    model->FrameRate = 4;
    player->AddComponent(model);

//    Entity* logo = AddEntity("Logo");
//    logo->AddComponent(new Components::Transform(*logo, Point(0, 15), 30, 30));
//    logo->AddComponent(new Components::Sprite(*logo));
//    logo->GetComponent<Components::Sprite>(true)->SetSpriteSource("Title.png");
//    logo->GetComponent<Components::Sprite>(true)->AnimationActive = false;

    Entity* follow = SpawnArchetype(ForLease->Filesystem.AssetDirectory(Modules::Filesystem::AssetType::Blueprint) + "Wisp", Point(-40.0f, -20.0f), "MenuFollow");
    follow->GetComponent<Components::Follow>()->Offset = Vector(-1.5f, -1.0f);
    follow->GetComponent<Components::Light>()->Radius = 5.0f;

    Entity* copyright = AddEntity("Copyright");
    copyright->AddComponent(new Components::Transform(*copyright, Point(-40.0f, 22.0f), 2.0f, 2.0f));
    copyright->AddComponent(new Components::SpriteText(*copyright, "Liberation_Serif.fnt", "Copyright 2015-2016 DigiPen Institute of Technology, All Rights Reserved", Color(1,1,1)));


    Entity* dpHeading = AddEntity("DPHeading");
    dpHeading->AddComponent(new Components::Transform(*dpHeading, Point(-40.0f, 19.0f), 2.0f, 2.0f));
    dpHeading->AddComponent(new Components::SpriteText(*dpHeading, "Liberation_Serif.fnt", "Digipen Faculty", Color(0.5, 0.5, 0.5)));
    Entity* dpCredits = AddEntity("DPCredits");
    dpCredits->AddComponent(new Components::Transform(*dpCredits, Point(-40.0f, 17.0f), 1.5f, 1.5f));
    dpCredits->AddComponent(new Components::SpriteText(*dpCredits, "Liberation_Serif.fnt"));
    std::stringstream ss;
    ss  << "President.......Claude Comair\n"
        << "Instructor......Ellen Beeman\n"
        << "Instructor......Stephen Beeman\n"
        << "Instructor......Ben Ellinger\n"
        << "Instructor......Rachel Rutherford\n";
    dpCredits->GetComponent<Components::SpriteText>()->Text = ss.str();


    Entity* teamHeading = AddEntity("TeamHeading");
    teamHeading->AddComponent(new Components::Transform(*teamHeading, Point(-40.0f, 9.0f), 2.0f, 2.0f));
    teamHeading->AddComponent(new Components::SpriteText(*teamHeading, "Liberation_Serif.fnt", "Team For Lease", Color(0.5, 0.5, 0.5)));
    Entity* teamCredits = AddEntity("TeamCredits");
    teamCredits->AddComponent(new Components::Transform(*teamCredits, Point(-40.0f, 7.0f), 1.5f, 1.5f));
    teamCredits->AddComponent(new Components::SpriteText(*teamCredits, "Liberation_Serif.fnt"));
    std::stringstream ss1;
    ss1 << "Jiangdi Gou\n"
        << "Kaila Harris\n"
        << "Chris Hudson\n"
        << "Andrew Langley\n"
        << "Sean McGeer\n"
        << "Sam Montanari\n"
        << "Josh Painter\n";
    teamCredits->GetComponent<Components::SpriteText>()->Text = ss1.str();

    Entity* addHeading = AddEntity("AdditionalHeading");
    addHeading->AddComponent(new Components::Transform(*addHeading, Point(-40.0f, -3.5f), 2.0f, 2.0f));
    addHeading->AddComponent(new Components::SpriteText(*addHeading, "Liberation_Serif.fnt", "Additional Contributors", Color(0.5, 0.5, 0.5)));
    Entity* addCredits = AddEntity("AdditionalCredits");
    addCredits->AddComponent(new Components::Transform(*addCredits, Point(-40.0f, -5.5f), 1.5f, 1.5f));
    addCredits->AddComponent(new Components::SpriteText(*addCredits, "Liberation_Serif.fnt"));
    std::stringstream ss2;
    ss2 << "Background Art...Alex Beavin\n"
        << "Audio............Eric Bergman\n";
    addCredits->GetComponent<Components::SpriteText>()->Text = ss2.str();

    Entity* menu = AddEntity("Menu");
    menu->AddComponent(new Components::Transform(*menu, Point(-40.0f, -20.0f)));
    menu->AddComponent(new Components::Menu(*menu, Vector(0, -1), true, 2.0f, 2.5f, "Liberation_Serif.fnt", "MenuFollow", Color(1, 1, 1), Color(1, 1, 0)));
    Components::Menu* menuComp = menu->GetComponent<Components::Menu>();
    //menuComp->AddItem(new MenuItems::NextLevel("Play"));
    //menuComp->AddItem(new MenuItems::ActivateAndDeactivate("Options", "OptionsMenu", "Menu"));
    menuComp->AddItem(new MenuItems::LoadLevel("Main Menu", "MainMenu"));
    //menuComp->AddItem(new MenuItems::ActivateAndDeactivate("Quit", "QuitConfirm", "Menu"));
    menuComp->Activate();

    //Entity* opMenu = AddEntity("OptionsMenu");
    //opMenu->AddComponent(new Components::Transform(*opMenu, Point(-40.0f, -10.0f)));
    //opMenu->AddComponent(new Components::Menu(*opMenu, Vector(0, -1), false, 2.0f, 2.5f, "Liberation_Serif.fnt", "MenuFollow", Color(1, 1, 1), Color(1, 1, 0)));
    //Components::Menu* opMenuComp = opMenu->GetComponent<Components::Menu>();
    //opMenuComp->AddItem(new OptionMenuItems::Resolution());
    //opMenuComp->AddItem(new OptionMenuItems::Fullscreen());
    //opMenuComp->AddItem(new OptionMenuItems::Volume());
    //opMenuComp->AddItem(new OptionMenuItems::FinalAccept("OptionsMenu"));
    //opMenuComp->AddItem(new MenuItems::ActivateAndDeactivate("Back", "Menu", "OptionsMenu"));

    //Entity* quitConfirm = AddEntity("QuitConfirm");
    //quitConfirm->AddComponent(new Components::Transform(*quitConfirm, Point(-40.0f, -10.0f)));
    //quitConfirm->AddComponent(new Components::Menu(*quitConfirm, Vector(0, -1), false, 2.0f, 2.5f, "Liberation_Serif.fnt", "MenuFollow", Color(1, 1, 1), Color(1, 1, 0)));
    //Components::Menu* quitConfirmComp = quitConfirm->GetComponent<Components::Menu>();
    //quitConfirmComp->AddItem(new MenuItems::Quit("Quit"));
    //quitConfirmComp->AddItem(new MenuItems::ActivateAndDeactivate("Cancel", "Menu", "QuitConfirm"));
}

void Credits::Update() {

    ForLease->OSInput.ProcessAllInput();

    for (FLE::Entity* entity : Entities) {
        entity->Update();
    }

    for (FLE::LevelComponent* levelComponent : LevelComponents) {
        levelComponent->Update(Entities);
    }

    LevelComponents::Renderer* renderer = ForLease->GameStateManager().CurrentState().GetLevelComponent<LevelComponents::Renderer>();

    ForLease->GameWindow->UpdateGameWindow();
}

void Credits::Deinitialize() {
    DeleteAllEntities();
    DeleteAllLevelComponents();
}

void Credits::Unload() {}
