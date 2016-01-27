/*!
    \file   LevelEditor.cpp
    \author Sam Montanari
    \date   12/2/15

    \brief
        Partial implementation of level editor class

    \see LevelEditor.h LevelEditorMenu.cpp LevelEditorObjectWindow.cpp

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "LevelEditor.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <map>
namespace ForLeaseEngine
{
    namespace LevelEditorGlobals
    {
         SDL_Window* window;
         Point       mousePos;

         LevelComponents::Renderer* render;
         LevelComponents::Physics*  levelPhysics;
         LevelComponents::Light*    levelLight;
         Vector gravity;

         Entity*                            selection;
         Components::Transform*             selTran;
         Components::Model*                 selModel;
         Components::Camera*                selCamera;
         Components::Light*                 selLight;
         Components::Collision*             selCollision;
         Components::Physics*               selPhysics;
         //Components::SoundEmitter*          selSound;
         Components::Sprite*                selSprite;
         Components::SpriteText*            selSprtxt;
         Components::CharacterController*   selController;
         Components::VisionCone*            selVision;
         Components::Menu*                  selMenu;
         Components::DragWithMouse*         selDrag;
         Components::ScaleWithKeyboard*     selScale;
         Components::TransformModeControls* selTMC;
         Components::EnemyAI*               selEnemyAI;
         Components::FadeWithDistance*      selFade;
         Components::ChangeLevelOnCollide*  selChange;
         Components::BackgroundMusic*       selMusic;


         Entity*                camera;
         Components::Transform* camTrans;
         Components::Camera*    camCamera;

         std::vector<std::string> meshNames;
         std::vector<std::string> componentNames;
         //std::vector<std::string> soundNames;
         std::vector<std::string> archetypeNames;
         std::vector<std::string> fontNames;
         std::vector<std::string> animationNames;
         std::vector<std::string> textureNames;

         std::map<std::string, ComponentType> reqMap;
         Serializer copyEntity;

         bool clickAdd   = false;
         bool newSelect  = false;
         bool moveMode   = false;
         bool showWindow = false;
         bool selMade    = false;
         bool archSpawn  = false;
         bool selMode    = true;
         bool toSave     = false;
         bool toLoad     = false;
         bool delobj     = false;
         bool copySet    = false;
         bool setLiked   = false;
         bool setHated   = false;
         bool setTarget  = false;
         bool walkSound  = false;
         bool jumpSound  = false;
         bool landSound  = false;
         bool walkAni    = false;
         bool jumpAni    = false;
         bool setName    = false;

         char entName[70];
         char spriteTextBuf[500];
         char statefile[70];
         char statename[70];
         char meshfile[70];
         char archetypefile[70];
         char soundfile[70];
         char fontfile[70];
         char enemyHateN[70];
         char enemyLikeN[70];
         char enemyHateS[70];
         char enemyLikeS[70];
         char animationfile[70];
         char spriteSource[70];
         char changeLevel[70];
         char changeObject[70];

         float spriteTextColor[4] = { 0 };
         float visionConeColor[4] = { 0 };
         float happyColor[4] = { 0 };
         float detectionColor[4] = { 0 };
         float noDetectionColor[4] = { 0 };
         float spriteColor[4] = { 0 };
         float meshColor[4] = { 0 };
         float lightColor[4] = { 0 };

         const char* archToSpawn = NULL;
         int lightRays = 100;
    }

    namespace leg = LevelEditorGlobals;

//    void PopulateMap()
//    {
//        std::vector<Component*> comps;
//        Entity dummy;
//        comps.push_back(new Components::BackgroundMusic(dummy));
//        comps.push_back(new Components::Camera(dummy));
//        comps.push_back(new Components::ChangeLevelOnCollide(dummy));
//        comps.push_back(new Components::Collision(dummy));
//        comps.push_back(new Components::DragWithMouse(dummy));
//        comps.push_back(new Components::FadeWithDistance(dummy));
//        comps.push_back(new Components::EnemyAI(dummy));
//        comps.push_back(new Components::Light(dummy));
//        comps.push_back(new Components::Model(dummy));
//        comps.push_back(new Components::Physics(dummy));
//        comps.push_back(new Components::CharacterController(dummy));
//        comps.push_back(new Components::ScaleWithKeyboard(dummy));
//        comps.push_back(new Components::SoundEmitter(dummy));
//        comps.push_back(new Components::Sprite(dummy));
//        comps.push_back(new Components::SpriteText(dummy));
//        comps.push_back(new Components::TransformModeControls(dummy));
//        comps.push_back(new Components::VisionCone(dummy));
//
//        for (int i = 0; i < comps.size(); i++)
//        {
//            leg::reqMap[leg::componentNames[i]] = comps[i]->GetRequired;
//        }
//    }

    void LevelEditor::Load()
    {
        leg::render = new LevelComponents::Renderer(*this);
        leg::camera = AddEntity("Level Camera");
        leg::camTrans = new Components::Transform(*leg::camera);
        leg::camCamera = new Components::Camera(*leg::camera, 0, 1, 50);
        leg::camera->AddComponent(leg::camTrans);
        leg::camera->AddComponent(leg::camCamera);
        leg::render->SetCamera(*leg::camera);
        leg::levelPhysics = new LevelComponents::Physics(*this);
        leg::levelLight = new LevelComponents::Light(*this);
        AddLevelComponent(leg::render);
        AddLevelComponent(leg::levelPhysics);
        AddLevelComponent(new LevelComponents::Menu(*this));
        AddLevelComponent(new LevelComponents::Collision(*this));
        AddLevelComponent(leg::levelLight);
        leg::gravity = leg::levelPhysics->GetGravity();
        leg::window = ForLease->GameWindow->DangerousGetRawWindow();
        strcpy(leg::statename, Name.c_str());
        LoadFiles();
        leg::componentNames.push_back("Background Music");
        leg::componentNames.push_back("Camera");
        leg::componentNames.push_back("Change Level on Collide");
        leg::componentNames.push_back("Collision");
        leg::componentNames.push_back("Drag with Mouse");
        leg::componentNames.push_back("Fade with Distance");
        leg::componentNames.push_back("Enemy AI");
        leg::componentNames.push_back("Light");
        leg::componentNames.push_back("Model");
        leg::componentNames.push_back("Physics");
        leg::componentNames.push_back("Player Controller");
        leg::componentNames.push_back("Scale with Keyboard");
        leg::componentNames.push_back("Sound");
        leg::componentNames.push_back("Sprite");
        leg::componentNames.push_back("Sprite Text");
        leg::componentNames.push_back("Transform Control");
        leg::componentNames.push_back("Vision Cone");
        //PopulateMap();
        Point moved(100000, 100000);
        Entity* ent = GetEntityByName("PauseMenu", true);
        Components::Transform* trans = ent->GetComponent<Components::Transform>();
        trans->Position = moved;
        ent = GetEntityByName("QuitConfirm", true);
        trans = ent->GetComponent<Components::Transform>();
        trans->Position = moved;
        ent = GetEntityByName("HowToConfirm", true);
        trans = ent->GetComponent<Components::Transform>();
        trans->Position = moved;
        ent = GetEntityByName("HowToScreen", true);
        trans = ent->GetComponent<Components::Transform>();
        trans->Position = moved;
    }

    void LevelEditor::Initialize()
    {
        ImGui_ImplSdl_Init(leg::window);
    }

    void LevelEditor::GetMouse(Point& v)
    {
        ImVec2 screen = ImGui::GetMousePos();
        v[0] = screen.x;
        v[1] = ForLease->GameWindow->GetYResolution() - screen.y;
        v = leg::render->ScreenToWorld(v);
    }

    void LevelEditor::Input()
    {
        double dt = ForLease->FrameRateController().GetDt();

        if (leg::moveMode && ImGui::IsMouseDown(1))
        {
            GetMouse(leg::mousePos);
            Point delta = leg::mousePos - leg::camTrans->Position;
            leg::camTrans->Position[0] += delta[0] * dt;
            leg::camTrans->Position[1] += delta[1] * dt;
        }

        else if (leg::clickAdd && ImGui::IsMouseClicked(1))
        {
            GetMouse(leg::mousePos);
            Entity* ent = AddEntity();
            ent->AddComponent(new Components::Transform(*ent, leg::mousePos[0], leg::mousePos[1]));
        }

        if (leg::setTarget && ImGui::IsMouseClicked(0) && !ImGui::IsMouseHoveringAnyWindow())
        {
            GetMouse(leg::mousePos);
            std::vector<Entity*> ents = GetEntitiesAtPosition(leg::mousePos);
            for (Entity* e : ents)
            {
                if (e != leg::camera)
                {
                    leg::selFade->TrackedEntityID = e->GetID();
                    leg::setTarget = false;
                }
            }
        }
        else if (leg::selMode && ImGui::IsMouseClicked(0) && !ImGui::IsMouseHoveringAnyWindow())
        {
            GetMouse(leg::mousePos);
            //std::vector<Entity*> ents = GetEntitiesAtPosition(leg::mousePos);
            leg::selection = GetEntityAtPosition(leg::mousePos);

            //for (unsigned i = 0; i < ents.size(); i++)
            //{
                if (leg::selection)
                {
                    //leg::selection = ents[i];
                    std::string name = leg::selection->GetName();
                    strcpy(leg::entName, name.c_str());
                    leg::selCamera     = leg::selection->GetComponent<Components::Camera>();
                    leg::selTran       = leg::selection->GetComponent<Components::Transform>();
                    leg::selMenu       = leg::selection->GetComponent<Components::Menu>();
                    leg::selVision     = leg::selection->GetComponent<Components::VisionCone>();
                    leg::selDrag       = leg::selection->GetComponent<Components::DragWithMouse>();
                    leg::selScale      = leg::selection->GetComponent<Components::ScaleWithKeyboard>();
                    leg::selTMC        = leg::selection->GetComponent<Components::TransformModeControls>();
                    leg::selModel      = leg::selection->GetComponent<Components::Model>();
                    leg::selCollision  = leg::selection->GetComponent<Components::Collision>();
                    leg::selPhysics    = leg::selection->GetComponent<Components::Physics>();
//                    leg::selSound      = leg::selection->GetComponent<Components::SoundEmitter>();
                    leg::selSprite     = leg::selection->GetComponent<Components::Sprite>();
                    leg::selSprtxt     = leg::selection->GetComponent<Components::SpriteText>();
                    leg::selController = leg::selection->GetComponent<Components::CharacterController>();
                    leg::selEnemyAI    = leg::selection->GetComponent<Components::EnemyAI>();
                    leg::selLight      = leg::selection->GetComponent<Components::Light>();
                    leg::selFade       = leg::selection->GetComponent<Components::FadeWithDistance>();
                    leg::selChange     = leg::selection->GetComponent<Components::ChangeLevelOnCollide>();
                    leg::selMusic      = leg::selection->GetComponent<Components::BackgroundMusic>();
                    if (leg::selSprtxt)
                    {
                        strcpy(leg::spriteTextBuf, leg::selSprtxt->Text.c_str());
                        leg::spriteTextColor[0] =  leg::selSprtxt->TextColor.GetR();
                        leg::spriteTextColor[1] =  leg::selSprtxt->TextColor.GetG();
                        leg::spriteTextColor[2] =  leg::selSprtxt->TextColor.GetB();
                        leg::spriteTextColor[3] =  leg::selSprtxt->TextColor.GetA();
                    }
                    if (leg::selVision)
                    {
                        leg::visionConeColor[0] = leg::selVision->IndicatorColor.GetR();
                        leg::visionConeColor[1] = leg::selVision->IndicatorColor.GetG();
                        leg::visionConeColor[2] = leg::selVision->IndicatorColor.GetB();
                        leg::visionConeColor[3] = leg::selVision->IndicatorColor.GetA();
                    }
                    if (leg::selEnemyAI)
                    {
                        strcpy(leg::enemyHateN, leg::selEnemyAI->HatedEntityName.c_str());
                        strcpy(leg::enemyLikeN, leg::selEnemyAI->LikedEntityName.c_str());
                        strcpy(leg::enemyHateS, leg::selEnemyAI->HatedSeenSound.c_str());
                        strcpy(leg::enemyLikeS, leg::selEnemyAI->LikedSeenSound.c_str());
                        leg::noDetectionColor[0] = leg::selEnemyAI->NoDetectionColor.GetR();
                        leg::noDetectionColor[1] = leg::selEnemyAI->NoDetectionColor.GetG();
                        leg::noDetectionColor[2] = leg::selEnemyAI->NoDetectionColor.GetB();
                        leg::noDetectionColor[3] = leg::selEnemyAI->NoDetectionColor.GetA();
                        leg::detectionColor[0]   = leg::selEnemyAI->DetectionColor.GetR();
                        leg::detectionColor[1]   = leg::selEnemyAI->DetectionColor.GetG();
                        leg::detectionColor[2]   = leg::selEnemyAI->DetectionColor.GetB();
                        leg::detectionColor[3]   = leg::selEnemyAI->DetectionColor.GetA();
                        leg::happyColor[0]       = leg::selEnemyAI->HappyColor.GetR();
                        leg::happyColor[1]       = leg::selEnemyAI->HappyColor.GetG();
                        leg::happyColor[2]       = leg::selEnemyAI->HappyColor.GetB();
                        leg::happyColor[3]       = leg::selEnemyAI->HappyColor.GetA();
                    }
                    if (leg::selLight)
                    {
                        leg::lightRays = leg::selLight->Rays;
                        leg::lightColor[0] = leg::selLight->DrawColor.GetR();
                        leg::lightColor[1] = leg::selLight->DrawColor.GetG();
                        leg::lightColor[2] = leg::selLight->DrawColor.GetB();
                        leg::lightColor[3] = leg::selLight->DrawColor.GetA();
                    }
                    if (leg::selSprite)
                    {
                        leg::spriteColor[0] = leg::selSprite->SpriteColor.GetR();
                        leg::spriteColor[1] = leg::selSprite->SpriteColor.GetG();
                        leg::spriteColor[2] = leg::selSprite->SpriteColor.GetB();
                        leg::spriteColor[3] = leg::selSprite->SpriteColor.GetA();
                    }
                    if (leg::selChange)
                    {
                        strcpy(leg::changeLevel, leg::selChange->LevelName.c_str());
                        strcpy(leg::changeObject, leg::selChange->TriggerObjectName.c_str());
                    }
                    leg::selMade = true;
                    //break;
                }
            //}
        }

        if (leg::selMade && ImGui::IsMouseDown(0) && ImGui::IsMouseDragging(0))
        {
            GetMouse(leg::mousePos);
            leg::selTran->Position = leg::mousePos;
        }

        if (ImGui::IsMouseReleased(0) || !leg::selMode)
            leg::selMade = false;

        if (leg::selection && ImGui::GetIO().KeyCtrl && ImGui::IsKeyPressed(Keys::C))
        {
            leg::selection->Serialize(leg::copyEntity);
            leg::copySet = true;
        }

        if (leg::copySet && ImGui::GetIO().KeyCtrl && ImGui::IsKeyPressed(Keys::V))
        {
            GetMouse(leg::mousePos);
            Entity* ent = AddEntity();
            ent->Deserialize(leg::copyEntity);
            Components::Transform* trans = ent->GetComponent<Components::Transform>();
            trans->Position = leg::mousePos;
        }

    }

    void LevelEditor::Update()
    {
        ForLease->OSInput.ProcessAllInputWithImgui();
        ImGui_ImplSdl_NewFrame(leg::window);
        DrawMainMenu();
        DrawMainWindow();
        if (leg::selection)
        {
            DrawObjectWindow();
        }

        if (leg::toSave)
        {
            Serializer root;
            Serialize(root);
            root.WriteFile(leg::statefile);
            leg::toSave = false;
        }

        if (leg::toLoad)
        {
            Serializer root;
            if (root.ReadFile(leg::statefile))
            {
                Deserialize(root);
                strcpy(leg::statename, GetName().c_str());
                LevelComponents::Menu* levelmenu = GetLevelComponent<LevelComponents::Menu>();
                LevelComponents::Collision* levelcol = GetLevelComponent<LevelComponents::Collision>();
                leg::levelPhysics = GetLevelComponent<LevelComponents::Physics>();
                leg::levelLight = GetLevelComponent<LevelComponents::Light>();
                leg::render = GetLevelComponent<LevelComponents::Renderer>();
                if (!leg::levelLight)
                {
                    leg::levelLight = new LevelComponents::Light(*this);
                    AddLevelComponent(leg::levelLight);
                }
                if (!leg::render)
                {
                    leg::render = new LevelComponents::Renderer(*this);
                    AddLevelComponent(leg::render);
                }
                leg::camera = GetEntityByName("Level Camera", true);
                leg::camTrans = leg::camera->GetComponent<Components::Transform>();
                leg::camCamera = leg::camera->GetComponent<Components::Camera>();
                leg::render->SetCamera(*leg::camera);
                if (!leg::levelPhysics)
                {
                    leg::levelPhysics = new LevelComponents::Physics(*this);
                    AddLevelComponent(leg::levelPhysics);
                }
                if (!levelcol)
                {
                    levelcol = new LevelComponents::Collision(*this);
                    AddLevelComponent(levelcol);
                }
                if (!levelmenu)
                {
                    levelmenu = new LevelComponents::Menu(*this);
                    AddLevelComponent(levelmenu);
                }
                leg::selection = NULL;
                Point moved(100000, 100000);
                Entity* ent = GetEntityByName("PauseMenu");
                Components::Transform* trans;
                if (ent != NULL)
                {
                    trans = ent->GetComponent<Components::Transform>();
                    trans->Position = moved;
                }

                ent = GetEntityByName("QuitConfirm");
                if (ent != NULL)
                {
                    trans = ent->GetComponent<Components::Transform>();
                    trans->Position = moved;
                }
                ent = GetEntityByName("HowToConfirm");
                if (ent != NULL)
                {
                    trans = ent->GetComponent<Components::Transform>();
                    trans->Position = moved;
                }
                ent = GetEntityByName("HowToScreen");
                if (ent != NULL)
                {
                    trans = ent->GetComponent<Components::Transform>();
                    trans->Position = moved;
                }
            }
            leg::toLoad = false;
        }

        if (leg::delobj)
        {
            DeleteEntity(leg::selection);
            leg::selection = NULL;
            leg::delobj = false;
        }

        if (leg::archToSpawn)
        {
            SpawnArchetype(leg::archToSpawn);
            leg::archToSpawn = NULL;
        }

        if (leg::setName)
            SetName(leg::statename);

        Input();
        leg::levelLight->Update(Entities);
        leg::render->Update(Entities);
        for(Entity* it : Entities)
        {
            if (it->HasComponent(ComponentType::VisionCone))
            {
                Components::VisionCone* vis = it->GetComponent<Components::VisionCone>();
                vis->Update();
            }
            if (it != leg::camera)
            {
                Components::Transform* tran  = it->GetComponent<Components::Transform>();
                Components::Model*     model = it->GetComponent<Components::Model>();
                leg::render->SetDrawingColor(1,1,1,1);
                if (tran != NULL && (!model || model->ModelMesh.empty()))
                {
                    leg::render->DrawRectangle(tran->Position, 2, 2, tran->Rotation);
                }
            }
        }
        if (leg::selection && leg::selFade)
        {
            Entity* ent = GetEntityByID(leg::selFade->TrackedEntityID);
            if (ent != NULL)
            {
                Components::Transform* trans = ent->GetComponent<Components::Transform>();
                leg::render->SetDrawingColor(Color(1, 0, 0));
                leg::render->DrawRectangle(trans->Position, leg::camCamera->Size / 25, leg::camCamera->Size / 25, trans->Rotation);
            }
        }
        ImGui::Render();
        ForLease->GameWindow->UpdateGameWindow();
    }

    void LevelEditor::Deinitialize()
    {
        ImGui_ImplSdl_Shutdown();
    }

    void LevelEditor::Unload()
    {
        SaveFiles();
    }

    void LevelEditor::SaveFiles()
    {
        Serializer root;
        root.WriteStringArray("Meshes", &(leg::meshNames[0]), leg::meshNames.size());
        root.WriteStringArray("Archetypes", &(leg::archetypeNames[0]), leg::archetypeNames.size());
 //       root.WriteStringArray("Sounds", &(leg::soundNames[0]), leg::soundNames.size());
        root.WriteStringArray("Animations", &(leg::animationNames[0]), leg::animationNames.size());
        root.WriteStringArray("Fonts", &(leg::fontNames[0]), leg::fontNames.size());
        root.WriteStringArray("Textures", &(leg::textureNames[0]), leg::textureNames.size());
        root.WriteFile("LevelEditorData.json");
    }

    void LevelEditor::LoadFiles()
    {
        Serializer root;
        if (root.ReadFile("LevelEditorData.json"))
        {
            root.ReadStringArray("Meshes", leg::meshNames);
            root.ReadStringArray("Archetypes", leg::archetypeNames);
   //         root.ReadStringArray("Sounds", leg::soundNames);
            root.ReadStringArray("Animations", leg::animationNames);
            root.ReadStringArray("Fonts", leg::fontNames);
            root.ReadStringArray("Textures", leg::textureNames);
            for (auto i : leg::meshNames)
                ForLease->Resources.LoadMesh(i);
//            for (auto i : leg::soundNames)
//                ForLease->Resources.LoadSound(i);
            for (auto i : leg::animationNames)
                ForLease->Resources.LoadMeshAnimation(i);
            for (auto i : leg::fontNames)
                ForLease->Resources.LoadFont(i);
            for (auto i : leg::textureNames)
                ForLease->Resources.LoadTexture(i);

            leg::meshNames = ForLease->Resources.GetLoadedMeshNames();
//            leg::soundNames = ForLease->Resources.GetLoadedSoundNames();
            leg::animationNames = ForLease->Resources.GetLoadedMeshAnimationNames();
            leg::fontNames = ForLease->Resources.GetLoadedFontNames();
            leg::textureNames = ForLease->Resources.GetLoadedTextureNames();
        }
    }
}

