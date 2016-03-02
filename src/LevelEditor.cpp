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
#include <unordered_map>
namespace ForLeaseEngine
{
    namespace LevelEditorGlobals
    {
         SDL_Window* window;
         Point       mousePos;

         LevelComponents::Renderer*   render;
         LevelComponents::Physics*    levelPhysics;
         LevelComponents::Light*      levelLight;
         LevelComponents::Checkpoint* levelCheckpoint;
         Vector gravity;

         Entity*                             selection;
         Components::Transform*              selTran;
         Components::Model*                  selModel;
         Components::Camera*                 selCamera;
         Components::Light*                  selLight;
         Components::Collision*              selCollision;
         Components::Physics*                selPhysics;
         Components::SoundEmitter*           selSound;
         Components::Sprite*                 selSprite;
         Components::SpriteText*             selSprtxt;
         Components::CharacterController*    selController;
         Components::VisionCone*             selVision;
         Components::Menu*                   selMenu;
         Components::DragWithMouse*          selDrag;
         Components::ScaleWithKeyboard*      selScale;
         Components::TransformModeControls*  selTMC;
         Components::EnemyAI*                selEnemyAI;
         Components::FadeWithDistance*       selFade;
         Components::ChangeLevelOnCollide*   selChange;
         Components::BackgroundMusic*        selMusic;
         Components::ParticleColorAnimator*  selPartColor;
         Components::ParticleEmitter*        selPartEmitter;
         Components::ParticleSystem*         selPartSystem;
         Components::SimpleParticleDynamics* selPartDynamics;
         Components::Parallax*               selParallax;
         Components::Occluder*               selOccluder;
         Components::Checkpoint*             selCheckpoint;
         Components::Follow*                 selFollow;



         Entity*                levelCamera;
         Entity*                camera;
         Components::Transform* camTrans;
         Components::Camera*    camCamera;

         std::vector<std::string> meshNames;
         std::vector<std::string> componentNames;
         std::vector<std::string> soundNames;
         std::vector<std::string> archetypeNames;
         std::vector<std::string> fontNames;
         std::vector<std::string> animationNames;
         std::vector<std::string> textureNames;
         std::vector<std::string> keyCodes;
         std::unordered_map<std::string, ComponentType> reqMap;

         Serializer copyEntity;

         bool clickAdd   = false;
         bool newSelect  = false;
         bool moveMode   = false;
         bool selMade    = false;
         bool selMode    = true;
         bool delobj     = false;
         bool copySet    = false;
         bool setFade    = false;
         bool startUp    = true;
         bool levelSaved = false;
         bool setFollow  = false;

         char entName[128];
         char spriteTextBuf[512];
         char statefile[128];
         char statename[128];
         char meshfile[128];
         char archetypefile[128];
         char soundfile[128];
         char fontfile[128];
         char animationfile[128];
         char spriteSource[128];
         char changeLevel[128];
         char changeObject[128];
         char particleSource[128];
         char enemyHateName[128];
         char enemyLikeName[128];

         const char* archToSpawn = NULL;
         int eCount = 0;
         int maxParticles = 0;
         int particleBlend = 1;
         int modelBlend = 0;
         int lightBlend = 2;

         std::string blueprintDir = "blueprints/";
         std::string levelDir     = "levels/";
    }

    namespace leg = LevelEditorGlobals;

    void PopulateMap()
    {
        std::vector<Component*> comps;
        Entity dummy;
        comps.push_back(new Components::BackgroundMusic(dummy));
        comps.push_back(new Components::Camera(dummy, 0, 0, 0));
        comps.push_back(new Components::ChangeLevelOnCollide(dummy));
        comps.push_back(new Components::Checkpoint(dummy));
        comps.push_back(new Components::Collision(dummy));
        comps.push_back(Components::DragWithMouse::Create(dummy));
        comps.push_back(new Components::FadeWithDistance(dummy));
        comps.push_back(new Components::Follow(dummy));
        comps.push_back(new Components::EnemyAI(dummy));
        comps.push_back(new Components::Light(dummy));
        comps.push_back(new Components::Model(dummy));
        comps.push_back(new Components::Occluder(dummy));
        comps.push_back(new Components::Parallax(dummy));
        comps.push_back(new Components::ParticleColorAnimator(dummy));
        comps.push_back(new Components::SimpleParticleDynamics(dummy));
        comps.push_back(new Components::ParticleEmitter(dummy));
        comps.push_back(new Components::ParticleSystem(dummy));
        comps.push_back(new Components::Physics(dummy));
        comps.push_back(Components::CharacterController::Create(dummy));
        comps.push_back(new Components::ScaleWithKeyboard(dummy));
        comps.push_back(new Components::SoundEmitter(dummy));
        comps.push_back(new Components::Sprite(dummy));
        comps.push_back(new Components::SpriteText(dummy, "Arial.fnt"));
        comps.push_back(new Components::TransformModeControls(dummy));
        comps.push_back(new Components::VisionCone(dummy));

        for (unsigned i = 0; i < comps.size(); i++)
        {
            leg::reqMap[leg::componentNames[i]] = comps[i]->GetRequired();
            delete comps[i];
        }
    }

    void LevelEditor::Load()
    {
        leg::render = new LevelComponents::Renderer(*this);
        leg::camera = AddEntity("LevelEditorCamera");
        leg::camTrans = new Components::Transform(*leg::camera);
        leg::camCamera = new Components::Camera(*leg::camera, 0, 1, 50);
        leg::camera->AddComponent(leg::camTrans);
        leg::camera->AddComponent(leg::camCamera);
        leg::levelCamera = leg::camera;
        leg::render->SetCamera(*leg::camera);
        leg::levelPhysics = new LevelComponents::Physics(*this);
        Serializer root;
        leg::levelCheckpoint = new LevelComponents::Checkpoint(*this, root);
        AddLevelComponent(leg::render);
        AddLevelComponent(leg::levelPhysics);
        AddLevelComponent(new LevelComponents::Menu(*this));
        AddLevelComponent(new LevelComponents::Collision(*this));
        AddLevelComponent(leg::levelCheckpoint);
        leg::gravity = leg::levelPhysics->GetGravity();
        leg::window = ForLease->GameWindow->DangerousGetRawWindow();
        strcpy(leg::statename, Name.c_str());
        if (leg::startUp)
        {
            Keys::InitKeymap();
            leg::keyCodes = Keys::GetKeyStrings();
            std::sort(leg::keyCodes.begin(), leg::keyCodes.end());
            LoadFiles();
            leg::componentNames.push_back("Background Music");
            leg::componentNames.push_back("Camera");
            leg::componentNames.push_back("Change Level on Collide");
            leg::componentNames.push_back("Checkpoint");
            leg::componentNames.push_back("Collision");
            leg::componentNames.push_back("Drag with Mouse");
            leg::componentNames.push_back("Fade with Distance");
            leg::componentNames.push_back("Follow");
            leg::componentNames.push_back("Enemy AI");
            leg::componentNames.push_back("Light");
            leg::componentNames.push_back("Model");
            leg::componentNames.push_back("Occluder");
            leg::componentNames.push_back("Parallax");
            leg::componentNames.push_back("Particle Color");
            leg::componentNames.push_back("Particle Dynamics");
            leg::componentNames.push_back("Particle Emitter");
            leg::componentNames.push_back("Particle System");
            leg::componentNames.push_back("Physics");
            leg::componentNames.push_back("Player Controller");
            leg::componentNames.push_back("Scale with Keyboard");
            leg::componentNames.push_back("Sound");
            leg::componentNames.push_back("Sprite");
            leg::componentNames.push_back("Sprite Text");
            leg::componentNames.push_back("Transform Control");
            leg::componentNames.push_back("Vision Cone");
            leg::startUp = false;
        }

        PopulateMap();
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

    void LevelEditor::MakeSelection()
    {
        std::string name = leg::selection->GetName();
        strcpy(leg::entName, name.c_str());
        leg::selCamera       = leg::selection->GetComponent<Components::Camera>();
        leg::selTran         = leg::selection->GetComponent<Components::Transform>();
        leg::selMenu         = leg::selection->GetComponent<Components::Menu>();
        leg::selVision       = leg::selection->GetComponent<Components::VisionCone>();
        leg::selDrag         = leg::selection->GetComponent<Components::DragWithMouse>();
        leg::selScale        = leg::selection->GetComponent<Components::ScaleWithKeyboard>();
        leg::selTMC          = leg::selection->GetComponent<Components::TransformModeControls>();
        leg::selModel        = leg::selection->GetComponent<Components::Model>();
        leg::selCollision    = leg::selection->GetComponent<Components::Collision>();
        leg::selPhysics      = leg::selection->GetComponent<Components::Physics>();
        leg::selSound        = leg::selection->GetComponent<Components::SoundEmitter>();
        leg::selSprite       = leg::selection->GetComponent<Components::Sprite>();
        leg::selSprtxt       = leg::selection->GetComponent<Components::SpriteText>();
        leg::selController   = leg::selection->GetComponent<Components::CharacterController>();
        leg::selEnemyAI      = leg::selection->GetComponent<Components::EnemyAI>();
        leg::selLight        = leg::selection->GetComponent<Components::Light>();
        leg::selFade         = leg::selection->GetComponent<Components::FadeWithDistance>();
        leg::selChange       = leg::selection->GetComponent<Components::ChangeLevelOnCollide>();
        leg::selMusic        = leg::selection->GetComponent<Components::BackgroundMusic>();
        leg::selPartColor    = leg::selection->GetComponent<Components::ParticleColorAnimator>();
        leg::selPartEmitter  = leg::selection->GetComponent<Components::ParticleEmitter>();
        leg::selPartSystem   = leg::selection->GetComponent<Components::ParticleSystem>();
        leg::selPartDynamics = leg::selection->GetComponent<Components::SimpleParticleDynamics>();
        leg::selParallax     = leg::selection->GetComponent<Components::Parallax>();
        leg::selOccluder     = leg::selection->GetComponent<Components::Occluder>();

        if (leg::selSprtxt)
            strcpy(leg::spriteTextBuf, leg::selSprtxt->Text.c_str());

        if (leg::selChange)
        {
            strcpy(leg::changeLevel, leg::selChange->LevelName.c_str());
            strcpy(leg::changeObject, leg::selChange->TriggerObjectName.c_str());
        }

        if (leg::selModel)
            leg::modelBlend = leg::selModel->BlendingMode;

        if (leg::selPartEmitter)
            leg::eCount = leg::selPartEmitter->EmitCount;

        if (leg::selPartSystem)
        {
            leg::maxParticles = leg::selPartSystem->MaxParticles;
            leg::particleBlend = leg::selPartSystem->BlendingMode;
        }

        if (leg::selEnemyAI)
        {
            strcpy(leg::enemyHateName, leg::selEnemyAI->HatedEntityName.c_str());
            strcpy(leg::enemyLikeName, leg::selEnemyAI->LikedEntityName.c_str());
        }
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

        else if (leg::clickAdd && ImGui::IsMouseClicked(1) && !ImGui::IsMouseHoveringAnyWindow())
        {
            GetMouse(leg::mousePos);
            Entity* ent = AddEntity();
            ent->AddComponent(new Components::Transform(*ent, leg::mousePos[0], leg::mousePos[1]));
        }

        if (leg::setFade && ImGui::IsMouseClicked(0) && !ImGui::IsMouseHoveringAnyWindow())
        {
            GetMouse(leg::mousePos);
            Entity* ent = GetEntityAtPosition(leg::mousePos);
            if (ent)
            {
                leg::selFade->TrackedEntityID = ent->GetID();
                leg::setFade = false;
            }
        }

        else if (leg::setFollow && ImGui::IsMouseClicked(0) && !ImGui::IsMouseHoveringAnyWindow())
        {
            GetMouse(leg::mousePos);
            Entity* ent = GetEntityAtPosition(leg::mousePos);
            if (ent)
            {
                leg::selFollow->FollowEntityID = ent->GetID();
                leg::setFollow = false;
            }
        }

        else if (leg::selMode && ImGui::IsMouseClicked(0) && !ImGui::IsMouseHoveringAnyWindow())
        {
            GetMouse(leg::mousePos);
            leg::selection = GetEntityAtPosition(leg::mousePos);

            if (leg::selection)
            {
                MakeSelection();
                leg::selMade = true;
            }
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
            leg::copyEntity.Clear();
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

        if (leg::selection && ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Delete)))
        {
            leg::delobj = true;
        }

    }

    void LevelEditor::SaveLevel(std::string level)
    {
        std::string file = leg::levelDir + level;
        if (leg::levelCamera && leg::levelCamera->HasComponent(ComponentType::Camera))
            leg::render->SetCamera(*leg::levelCamera);
        Serializer root;
        Serialize(root);
        root.WriteFile(file);
        leg::render->SetCamera(*leg::camera);
        leg::levelSaved = true;
    }

    void LevelEditor::LoadLevel(std::string level)
    {
        Serializer root;
        std::string file = leg::levelDir + level;
        if (root.ReadFile(file))
        {
            Deserialize(root);
            strcpy(leg::statename, GetName().c_str());
            LevelComponents::Menu* levelmenu = GetLevelComponent<LevelComponents::Menu>();
            LevelComponents::Collision* levelcol = GetLevelComponent<LevelComponents::Collision>();
            leg::levelPhysics = GetLevelComponent<LevelComponents::Physics>();
            leg::levelLight = GetLevelComponent<LevelComponents::Light>();
            leg::render = GetLevelComponent<LevelComponents::Renderer>();
            leg::levelLight = GetLevelComponent<LevelComponents::Light>();
            if (!leg::render)
            {
                leg::render = new LevelComponents::Renderer(*this);
                AddLevelComponent(leg::render);
            }
            unsigned long camID = leg::render->GetCameraID();
            leg::levelCamera = GetEntityByID(camID);
            leg::camera = GetEntityByName("LevelEditorCamera", true);
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
        }
    }

    void LevelEditor::SpawnBluePrint(std::string blueprint)
    {
        std::string file = leg::blueprintDir + blueprint;
        SpawnArchetype(file, leg::camTrans->Position);
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


        if (leg::delobj)
        {
            DeleteEntity(leg::selection);
            if (leg::levelCamera == leg::selection)
                leg::levelCamera = leg::camera;
            leg::selection = NULL;
            leg::delobj = false;
        }

        Input();

        if (leg::levelLight)
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


        if (leg::selection && leg::selPartEmitter && leg::selPartColor && leg::selPartDynamics && leg::selPartSystem)
        {
            leg::selPartEmitter->Update();
            leg::selPartColor->Update();
            leg::selPartSystem->Update();
            leg::selPartDynamics->Update();
        }

        if (leg::selection)
        {
            if (leg::selection->HasComponent(ComponentType::Collision))
            {
                leg::render->SetDrawingColor(Color(0, 0, 1));
                leg::selection->GetComponent<Components::Collision>()->DebugDraw();
            }
            else
            {
                leg::render->SetDrawingColor(Color(1, 0, 0));
                leg::render->DrawRectangle(leg::selTran->Position, leg::selTran->ScaleX * 2, leg::selTran->ScaleY * 2, leg::selTran->Rotation);

            }
            if (leg::selFade)
            {
                Entity* ent = GetEntityByID(leg::selFade->TrackedEntityID);
                if (ent != NULL)
                {
                    Components::Transform* trans = ent->GetComponent<Components::Transform>();
                    leg::render->SetDrawingColor(Color(1, 0, 1));
                    leg::render->DrawRectangle(trans->Position, trans->ScaleX * 2, trans->ScaleY * 2, trans->Rotation);
                }
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
        //SaveFiles();
    }

    void LevelEditor::LoadFiles()
    {
        leg::archetypeNames = ForLease->Filesystem.GetAllFilesInFolder(leg::blueprintDir);
        ForLease->Filesystem.LoadAllAssets();
        leg::meshNames = ForLease->Resources.GetLoadedMeshNames();
        leg::soundNames = ForLease->sound->GetName();
        leg::animationNames = ForLease->Resources.GetLoadedMeshAnimationNames();
        leg::fontNames = ForLease->Resources.GetLoadedFontNames();
        leg::textureNames = ForLease->Resources.GetLoadedTextureNames();
    }
}

