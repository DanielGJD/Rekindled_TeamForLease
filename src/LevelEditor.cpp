/*!
    \file   LevelEditor.cpp
    \author Sam Montanari
    \date   12/2/15

    \brief
        Implementation of level editor class

    \see LevelEditor.h

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "LevelEditor.h"
#include "Face.h"
#include "Edge.h"
#include "Vector.h"
#include "KeyboardEvent.h"
#include "Event.h"
#include "Keys.h"
#include "Vector.h"
#include "Texture.h"
#include "TextureRegion.h"
#include "Mesh.h"
#include "BmFont.h"
#include "Font.h"
#include "UpdateEvent.h"
#include "GL/gl.h"
#include "ComponentsInclude.h"
#include "LevelComponentsInclude.h"
#include "Timer.h"
#include "Serialize.h"
#include "MouseButtonEvent.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "GameStateManager.h"
#include <iostream>
#include <cmath>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
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
        Components::SoundEmitter*          selSound;
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
        std::vector<std::string> soundNames;
        std::vector<std::string> archetypeNames;
        std::vector<std::string> fontNames;
        std::vector<std::string> animationNames;
        std::vector<std::string> textureNames;

        std::string copyfile = "copyentity.json";

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
        int lightRays =100;
    }

    namespace leg = LevelEditorGlobals;

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

    static void DrawMainMenu()
    {
        ImGui::BeginMainMenuBar();
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::BeginMenu("Save"))
            {
                if (ImGui::InputText("File Name", leg::statefile, 70, ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    leg::toSave = true;
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Load"))
            {
                if (ImGui::BeginMenu("Level"))
                {
                    if (ImGui::InputText("File Name", leg::statefile, 70, ImGuiInputTextFlags_EnterReturnsTrue))
                    {
                        leg::toLoad = true;
                    }

                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Archetype"))
                {
                    if (ImGui::InputText("File Name", leg::archetypefile, 70, ImGuiInputTextFlags_EnterReturnsTrue))
                    {
                        leg::archetypeNames.push_back(leg::archetypefile);
                    }

                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Mesh"))
                {
                    if (ImGui::InputText("File Name", leg::meshfile, 70, ImGuiInputTextFlags_EnterReturnsTrue))
                    {
                        ForLease->Resources.LoadMesh(leg::meshfile);
                        leg::meshNames = ForLease->Resources.GetLoadedMeshNames();
                    }

                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Animation"))
                {
                    if (ImGui::InputText("File Name", leg::animationfile, 70, ImGuiInputTextFlags_EnterReturnsTrue))
                    {
                        ForLease->Resources.LoadMeshAnimation(leg::animationfile);
                        leg::animationNames = ForLease->Resources.GetLoadedMeshAnimationNames();
                    }
                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Sound"))
                {
                    if (ImGui::InputText("File Name", leg::soundfile, 70, ImGuiInputTextFlags_EnterReturnsTrue))
                    {
                        ForLease->Resources.LoadSound(leg::soundfile);
                        leg::soundNames = ForLease->Resources.GetLoadedSoundNames();
                    }

                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Font"))
                {
                    if (ImGui::InputText("File Name", leg::fontfile, 70, ImGuiInputTextFlags_EnterReturnsTrue))
                    {
                        ForLease->Resources.LoadFont(leg::fontfile);
                        leg::fontNames = ForLease->Resources.GetLoadedFontNames();
                    }

                    ImGui::EndMenu();
                }

                ImGui::EndMenu();
            }

            ImGui::EndMenu();
        }

        ImGui::Checkbox("Translate/Select", &leg::selMode);
        ImGui::SameLine();
        ImGui::Checkbox("Move Camera", &leg::moveMode);
        ImGui::SameLine();
        ImGui::Checkbox("Place Objects", &leg::clickAdd);
        ImGui::EndMainMenuBar();
    }

    static void DrawMainWindow()
    {
        ImGui::Begin("Level Editor");

        if (ImGui::InputText("Level Name", leg::statename, 70, ImGuiInputTextFlags_EnterReturnsTrue))
            leg::setName = true;

        if (ImGui::CollapsingHeader("Camera"))
        {
            ImGui::Text("Position");
            ImGui::PushItemWidth(80);
            ImGui::InputFloat("x", &(leg::camTrans->Position[0]), 0, 0, 3);
            ImGui::SameLine();
            ImGui::InputFloat("y", &(leg::camTrans->Position[1]), 0, 0, 3);
            ImGui::InputFloat("Size", &(leg::camCamera->Size), 0, 0, 0);
            ImGui::PopItemWidth();
        }

        if (ImGui::CollapsingHeader("Physics"))
        {
            ImGui::Text("Gravity");
            ImGui::PushItemWidth(80);
            ImGui::InputFloat("x", &(leg::gravity[0]), 0, 0, 3);
            ImGui::SameLine();
            ImGui::InputFloat("y", &(leg::gravity[1]), 0, 0, 3);
            ImGui::SameLine();
            if (ImGui::Button("Set"))
                leg::levelPhysics->SetGravity(leg::gravity);

            ImGui::PopItemWidth();
        }

        if (ImGui::CollapsingHeader("Spawn Archetype"))
        {
            static ImGuiTextFilter archFilter;
            archFilter.Draw("Archetype", 500);
            ImGui::Text("Available Archetypes");
            ImGui::Separator();
            ImGui::BeginChild("Archetypes", ImVec2(0, 100));
            for (unsigned i = 0; i < leg::archetypeNames.size(); i++)
            {
                if (archFilter.PassFilter(leg::archetypeNames[i].c_str()))
                {
                    if (ImGui::Button(leg::archetypeNames[i].c_str()))
                    {
                        leg::archToSpawn = leg::archetypeNames[i].c_str();
                    }
                }
            }
            ImGui::EndChild();

            if (archFilter.IsActive() && ImGui::IsKeyPressed(Keys::Return))
            {
                std::string s = archFilter.InputBuf;
                Serializer root;
                if (std::find(leg::archetypeNames.begin(), leg::archetypeNames.end(), s) == leg::archetypeNames.end() && root.ReadFile(s))
                {
                    leg::archetypeNames.push_back(s);
                }
            }
        }

        ImGui::End();
    }

    static void AddComponent(const std::string& component)
    {
        if (!(component.compare("Background Music")) && !leg::selMusic)
        {
            leg::selMusic = new Components::BackgroundMusic(*leg::selection);
            if (!leg::selection->AddComponent(leg::selMusic))
                leg::selMusic = NULL;
            return;
        }
        if (!(component.compare("Camera")) && !leg::selCamera)
        {
            leg::selCamera = new Components::Camera(*leg::selection, 0, 1, 50);
            if (!leg::selection->AddComponent(leg::selCamera))
                leg::selCamera = NULL;
            return;
        }
        if (!(component.compare("Change Level on Collide")) && !leg::selChange)
        {
            leg::selChange = new Components::ChangeLevelOnCollide(*leg::selection);
            if (!leg::selection->AddComponent(leg::selChange))
                leg::selChange = NULL;
            return;
        }
        if (!(component.compare("Collision")) && !leg::selCollision)
        {
            leg::selCollision = new Components::Collision(*leg::selection);
            if (!leg::selection->AddComponent(leg::selCollision))
                leg::selCollision = NULL;
            return;
        }
        if (!(component.compare("Drag with Mouse")) && !leg::selDrag)
        {
            leg::selDrag = Components::DragWithMouse::Create(*leg::selection);
            if (!leg::selection->AddComponent(leg::selDrag))
                leg::selDrag = NULL;
            return;
        }
        if (!(component.compare("Enemy AI")) && !leg::selEnemyAI)
        {
            leg::selEnemyAI = Components::EnemyAI::Create(*leg::selection);
            if (!leg::selection->AddComponent(leg::selEnemyAI))
                leg::selEnemyAI = NULL;
        }
        if (!(component.compare("Fade with Distance")) && !leg::selFade)
        {
            leg::selFade = new Components::FadeWithDistance(*leg::selection);
            if (!leg::selection->AddComponent(leg::selFade))
                leg::selFade = NULL;
            return;
        }
        if (!(component.compare("Light")) && !leg::selLight)
        {
            leg::selLight = new Components::Light(*leg::selection);
            if (!leg::selection->AddComponent(leg::selLight))
                leg::selLight = NULL;
            return;
        }
        if (!(component.compare("Model")) && !leg::selModel)
        {
            leg::selModel = new Components::Model(*leg::selection);
            if (!leg::selection->AddComponent(leg::selModel))
                leg::selModel = NULL;
            return;
        }
        if (!(component.compare("Physics")) && !leg::selPhysics)
        {
            leg::selPhysics = new Components::Physics(*leg::selection);
            if (!leg::selection->AddComponent(leg::selPhysics))
                leg::selPhysics = NULL;
            return;
        }
        if (!(component.compare("Player Controller")) && !leg::selController)
        {
            leg::selController = Components::CharacterController::Create(*leg::selection);
            if (!leg::selection->AddComponent(leg::selController))
                leg::selController = NULL;
            return;
        }
        if (!(component.compare("Scale with Keyboard")) && !leg::selScale)
        {
            leg::selScale = Components::ScaleWithKeyboard::Create(*leg::selection);
            if (!leg::selection->AddComponent(leg::selScale))
                leg::selScale = NULL;
            return;
        }
        if (!(component.compare("Sound")) && !leg::selSound)
        {
            leg::selSound = new Components::SoundEmitter(*leg::selection);
            if (!leg::selection->AddComponent(leg::selSound))
                leg::selSound = NULL;
            return;
        }
        if (!(component.compare("Sprite")) && !leg::selSprite)
        {
            leg::selSprite = new Components::Sprite(*leg::selection);
            if (!leg::selection->AddComponent(leg::selSprite))
                leg::selSprite = NULL;
            return;
        }
        if (!(component.compare("Sprite Text")) && !leg::selSprtxt)
        {
            leg::selSprtxt = new Components::SpriteText(*leg::selection, "Arial.fnt");
            if (!leg::selection->AddComponent(leg::selSprtxt))
                leg::selSprtxt = NULL;
            return;
        }
        if (!(component.compare("Transform Control")) && !leg::selTMC)
        {
            leg::selTMC = new Components::TransformModeControls(*leg::selection);
            if (!leg::selection->AddComponent(leg::selTMC))
                leg::selTMC = NULL;

            return;
        }
        if (!(component.compare("Vision Cone")) && !leg::selVision)
        {
            leg::selVision = new Components::VisionCone(*leg::selection);
            if (!leg::selection->AddComponent(leg::selVision))
                leg::selVision = NULL;
        }
    }

    static void DrawObjectWindow()
    {
        leg::render->SetDrawingColor(Color(1,1,1));
        leg::render->DrawRectangle(leg::selTran->Position, leg::selTran->ScaleX, leg::selTran->ScaleY, leg::selTran->Rotation);
        ImGui::Begin("Object Editor");
        ImGui::PushItemWidth(80);

        if (ImGui::InputText("Name", leg::entName, 70, ImGuiInputTextFlags_EnterReturnsTrue))
            leg::selection->SetName(leg::entName);

        ImGui::InputText("File Name", leg::archetypefile, 70);
        ImGui::SameLine();
        if (ImGui::Button("Save Archetype"))
        {
            std::string s = leg::archetypefile;
            leg::selection->CreateArchetype(leg::archetypefile);
            if (std::find(leg::archetypeNames.begin(), leg::archetypeNames.end(), s) == leg::archetypeNames.end())
            {
                leg::archetypeNames.push_back(leg::archetypefile);
            }
        }


        if (ImGui::CollapsingHeader("Add Component"))
        {
            static ImGuiTextFilter compFilter;
            compFilter.Draw("Components", 500);
            ImGui::Text("Available Components");
            ImGui::Separator();
            ImGui::BeginChild("Components", ImVec2(0, 100));
            for (unsigned i = 0; i < leg::componentNames.size(); i++)
            {
                if (compFilter.PassFilter(leg::componentNames[i].c_str()))
                {
                    if (ImGui::MenuItem(leg::componentNames[i].c_str()))
                    {
                        AddComponent(leg::componentNames[i]);
                    }
                }
            }
            ImGui::EndChild();
        }

        if (leg::selTran && ImGui::CollapsingHeader("Transform"))
        {
            ImGui::PushItemWidth(100);
            ImGui::InputFloat("x      ", &(leg::selTran->Position[0]), 0.5, 1);
            ImGui::SameLine();
            ImGui::InputFloat("y      ", &(leg::selTran->Position[1]), 0.5, 1);
            ImGui::PopItemWidth();
            ImGui::DragFloat("rotation", &(leg::selTran->Rotation), 0.05, 0, 44.0 / 7);
            ImGui::DragFloat("x scale", &(leg::selTran->ScaleX), 0.25);
            ImGui::SameLine();
            ImGui::DragFloat("y scale", &(leg::selTran->ScaleY), 0.25);
            ImGui::InputInt("Z Order", &(leg::selTran->ZOrder));
        }

        if (leg::selModel && ImGui::CollapsingHeader("Model"))
        {
            ImGui::Checkbox("Flip X", &(leg::selModel->FlipX));
            ImGui::SameLine();
            ImGui::Checkbox("Flip Y", &(leg::selModel->FlipY));
            ImGui::Indent();
            if (ImGui::CollapsingHeader("Change Mesh"))
            {
                static ImGuiTextFilter meshFilter;
                meshFilter.Draw("Mesh", 250.0f);
                ImGui::Text("Available Meshes");
                ImGui::Separator();
                ImGui::BeginChild("Meshes", ImVec2(0, 100), false, ImGuiWindowFlags_HorizontalScrollbar);
                for(unsigned i = 0; i < leg::meshNames.size(); i++)
                {
                    if(meshFilter.PassFilter((leg::meshNames[i]).c_str()))
                    {
                        if (ImGui::MenuItem((leg::meshNames[i]).c_str()))
                        {
                            leg::selModel->ModelMesh = leg::meshNames[i];
                        }
                    }
                }
                ImGui::EndChild();

                if (meshFilter.IsActive() && ImGui::IsKeyPressed(Keys::Return))
                {
                    ForLease->Resources.LoadMesh(meshFilter.InputBuf);
                    leg::meshNames = ForLease->Resources.GetLoadedMeshNames();
                }

            }

            if (ImGui::CollapsingHeader("Edit Animation"))
            {
                ImGui::Checkbox("Active", &(leg::selModel->AnimationActive));
                ImGui::Checkbox("Looping", &(leg::selModel->Looping));
                ImGui::InputFloat("Frame Rate", &(leg::selModel->FrameRate));
                ImGui::Text("Current Animation: %s", leg::selModel->GetAnimation().c_str());
                static ImGuiTextFilter animations;
                animations.Draw("Animation", 300);
                ImGui::Text("Available Animations");
                ImGui::Separator();
                ImGui::BeginChild("Animations", ImVec2(0, 100));
                for (std::string s : leg::animationNames)
                {
                    if (animations.PassFilter(s.c_str()))
                    {
                        if (ImGui::MenuItem(s.c_str()))
                            leg::selModel->SetAnimation(s);
                    }
                }
                if (animations.IsActive() && ImGui::IsKeyPressed(Keys::Return))
                {
                    ForLease->Resources.LoadMeshAnimation(animations.InputBuf);
                    leg::animationNames = ForLease->Resources.GetLoadedMeshAnimationNames();
                }
                ImGui::EndChild();
            }
            ImGui::Unindent();

            if (ImGui::Button("Remove Model"))
            {
                leg::selection->DeleteComponent(ComponentType::Model);
                leg::selModel = NULL;
            }
        }
        if (leg::selMusic && ImGui::CollapsingHeader("Background Music"))
        {
            ImGui::Text("Current Sound: %s", leg::selMusic->MusicName.c_str());
            static ImGuiTextFilter backSound;
            backSound.Draw("Trigger Sound", 250);
            ImGui::Text("Available Sounds");
            ImGui::Separator();
            ImGui::BeginChild("Sound", ImVec2(0, 100));
            for (std::string s : leg::soundNames)
            {
                if (backSound.PassFilter(s.c_str()))
                {
                    if (ImGui::MenuItem(s.c_str()))
                    {
                        leg::selMusic->MusicName = s;
                    }
                }
            }
            ImGui::EndChild();
            if (backSound.IsActive() && ImGui::IsKeyPressed(Keys::Return))
            {
                ForLease->Resources.LoadSound(backSound.InputBuf);
                leg::soundNames = ForLease->Resources.GetLoadedSoundNames();
            }
            if (ImGui::Button("Remove Background Music"))
            {
                leg::selection->DeleteComponent(ComponentType::BackgroundMusic);
                leg::selMusic = NULL;
            }
        }
        if (leg::selCamera && ImGui::CollapsingHeader("Camera"))
        {
            ImGui::InputFloat("Size", &(leg::selCamera->Size));
            if (ImGui::Button("Set Camera"))
                leg::render->SetCamera(*leg::selection);

            if (ImGui::Button("Remove Camera"))
            {
                leg::selection->DeleteComponent(ComponentType::Camera);
                leg::selCamera = NULL;
            }
        }
        if (leg::selChange && ImGui::CollapsingHeader("Change Level on Collide"))
        {
            ImGui::Checkbox("Active", &(leg::selChange->Active));
            ImGui::InputFloat("On Switch Time Scale", &(leg::selChange->OnSwitchTimeScale));
            ImGui::InputFloat("Fade Out Time", &(leg::selChange->FadeOutTime));
            ImGui::Text("Level Name: %s", leg::selChange->LevelName.c_str());
            if (ImGui::InputText("", leg::changeLevel, 70, ImGuiInputTextFlags_EnterReturnsTrue))
                leg::selChange->LevelName = leg::changeLevel;
            ImGui::Text("Trigger Object Name: %s", leg::selChange->TriggerObjectName.c_str());
            if (ImGui::InputText(" ", leg::changeObject, 70, ImGuiInputTextFlags_EnterReturnsTrue))
                leg::selChange->TriggerObjectName = leg::changeObject;

            ImGui::Text("Current Sound: %s", leg::selChange->TriggerSoundName.c_str());
            static ImGuiTextFilter changeSound;
            changeSound.Draw("Trigger Sound", 250);
            ImGui::Text("Available Sounds");
            ImGui::Separator();
            ImGui::BeginChild("Sound", ImVec2(0, 100));
            for (std::string s : leg::soundNames)
            {
                if (changeSound.PassFilter(s.c_str()))
                {
                    if (ImGui::MenuItem(s.c_str()))
                    {
                        leg::selChange->TriggerSoundName = s;
                    }
                }
            }
            ImGui::EndChild();
            if (changeSound.IsActive() && ImGui::IsKeyPressed(Keys::Return))
            {
                ForLease->Resources.LoadSound(changeSound.InputBuf);
                leg::soundNames = ForLease->Resources.GetLoadedSoundNames();
            }
            if (ImGui::Button("Remove Change on Collide"))
            {
                leg::selection->DeleteComponent(ComponentType::ChangeLevelOnCollide);
                leg::selChange = NULL;
            }
        }
        if (leg::selCollision && ImGui::CollapsingHeader("Collision"))
        {
            ImGui::PushItemWidth(100);
            ImGui::DragFloat("Width", &(leg::selCollision->Width), 0.05);
            ImGui::SameLine();
            ImGui::DragFloat("Height", &(leg::selCollision->Height), 0.05);
            ImGui::PopItemWidth();
            leg::render->DrawRectangle(leg::selTran->Position, leg::selCollision->Width, leg::selCollision->Height, leg::selTran->Rotation);
            ImGui::Checkbox("Resolve Collision", &(leg::selCollision->ResolveCollisions));

            if (ImGui::Button("Remove Collision"))
            {
                leg::selection->DeleteComponent(ComponentType::Collision);
                leg::selCollision = NULL;
            }
        }

        if (leg::selDrag && ImGui::CollapsingHeader("Drag with Mouse"))
        {
            ImGui::Checkbox("Active", &(leg::selDrag->Active));
            if (ImGui::Button("Remove Drag With Mouse"))
            {
                leg::selection->DeleteComponent(ComponentType::DragWithMouse);
                leg::selDrag = NULL;
            }
        }

        if (leg::selEnemyAI && ImGui::CollapsingHeader("Enemy AI"))
        {
            ImGui::Checkbox("Active", &(leg::selEnemyAI->Active));
            ImGui::InputFloat("Detection Delay", &(leg::selEnemyAI->DetectionDelay));
            ImGui::PushItemWidth(250);
            ImGui::ColorEdit4("Happy Color", leg::happyColor);
            ImGui::ColorEdit4("Detection Color", leg::detectionColor);
            ImGui::ColorEdit4("No Detection Color", leg::noDetectionColor);
            ImGui::PopItemWidth();
            if (ImGui::Button("Set Happy"))
            {
                leg::selEnemyAI->HappyColor.SetAll(leg::happyColor[0],
                                                   leg::happyColor[1],
                                                   leg::happyColor[2],
                                                   leg::happyColor[3]);
            }
            ImGui::SameLine();
            if (ImGui::Button("Set Detection"))
            {
                leg::selEnemyAI->DetectionColor.SetAll(leg::detectionColor[0],
                                                       leg::detectionColor[1],
                                                       leg::detectionColor[2],
                                                       leg::detectionColor[3]);
            }

            ImGui::SameLine();
            if (ImGui::Button("Set No Detection"))
            {
                leg::selEnemyAI->NoDetectionColor.SetAll(leg::noDetectionColor[0],
                                                         leg::noDetectionColor[1],
                                                         leg::noDetectionColor[2],
                                                         leg::noDetectionColor[3]);
            }

            if (ImGui::InputText("Hated Entity Name", leg::enemyHateN, 70, ImGuiInputTextFlags_EnterReturnsTrue))
                leg::selEnemyAI->HatedEntityName = leg::enemyHateN;

            if (ImGui::InputText("Liked Entity Name", leg::enemyLikeN, 70, ImGuiInputTextFlags_EnterReturnsTrue))
                leg::selEnemyAI->LikedEntityName = leg::enemyLikeN;

            ImGui::Checkbox("Liked Sound", &(leg::setLiked));
            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                ImGui::Text("%s", (leg::selEnemyAI->LikedSeenSound.c_str()));
                ImGui::EndTooltip();
            }
            ImGui::SameLine();
            ImGui::Checkbox("Hated Sound", &(leg::setHated));
            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                ImGui::Text("%s", (leg::selEnemyAI->HatedSeenSound.c_str()));
                ImGui::EndTooltip();
            }

            static ImGuiTextFilter sounds;
            sounds.Draw("Sound", 300);
            ImGui::Text("Available Sounds");
            ImGui::Separator();
            ImGui::BeginChild("Sounds", ImVec2(0, 100));
            for (std::string s : leg::soundNames)
            {
                if (sounds.PassFilter(s.c_str()))
                {
                    if (ImGui::MenuItem(s.c_str()))
                    {
                        if (leg::setHated)
                            leg::selEnemyAI->HatedSeenSound = s;
                        if (leg::setLiked)
                            leg::selEnemyAI->LikedSeenSound = s;
                    }
                }
            }
            ImGui::EndChild();
            if (sounds.IsActive() && ImGui::IsKeyPressed(Keys::Return))
            {
                ForLease->Resources.LoadSound(sounds.InputBuf);
                leg::soundNames = ForLease->Resources.GetLoadedSoundNames();
            }
            if (ImGui::Button("Remove Enemy AI"))
            {
                leg::selection->DeleteComponent(ComponentType::EnemyAI);
                leg::selEnemyAI = NULL;
            }
        }
        if (leg::selFade && ImGui::CollapsingHeader("Fade with Distance"))
        {
            ImGui::Checkbox("Active", &(leg::selFade->Active));
            ImGui::Checkbox("X Direction", &(leg::selFade->XDirection));
            ImGui::Checkbox("Y Direction", &(leg::selFade->YDirection));
            ImGui::DragFloat("Begin Distance", &(leg::selFade->FadeBeginDistance), 0.05, 0);
            ImGui::DragFloat("End Distance", &(leg::selFade->FadeEndDistance), 0.05, 0);
            ImGui::Checkbox("Select Target", &leg::setTarget);
            ImGui::SameLine();
            ImGui::Text("Current Target ID: %u", (unsigned)leg::selFade->TrackedEntityID);
            if (ImGui::Button("Remove Fade"))
            {
                leg::selection->DeleteComponent(ComponentType::FadeWithDistance);
                leg::selFade = NULL;
            }
        }
        if (leg::selLight && ImGui::CollapsingHeader("Light"))
        {
            ImGui::InputInt("Rays", &leg::lightRays, 1, 5);
            if (leg::lightRays >= 0)
                leg::selLight->Rays = leg::lightRays;
            ImGui::DragFloat("Sweep", &(leg::selLight->Sweep), 0.05);
            ImGui::Text("Direction");
            ImGui::SameLine();
            ImGui::DragFloat("x", &(leg::selLight->Direction[0]), 0.05);
            ImGui::SameLine();
            ImGui::DragFloat("y", &(leg::selLight->Direction[1]), 0.05);
            ImGui::PushItemWidth(250);
            ImGui::ColorEdit4("Light Color", leg::lightColor);
            if (ImGui::Button("Set Light Color"))
            {
                leg::selLight->DrawColor.SetAll(leg::lightColor[0],
                                                leg::lightColor[1],
                                                leg::lightColor[2],
                                                leg::lightColor[3]);
            }


            if (ImGui::Button("Remove Light"))
            {
                leg::selection->DeleteComponent(ComponentType::Light);
                leg::selLight = NULL;
            }
        }
        if (leg::selMenu && ImGui::CollapsingHeader("Menu"))
        {
            ImGui::InputFloat("Focused Scale", &(leg::selMenu->FocusedScale));
            ImGui::SameLine();
            ImGui::InputFloat("Unfocused Scale", &(leg::selMenu->UnfocusedScale));
            ImGui::InputFloat("Spacing X", &(leg::selMenu->Spacing[0]));
            ImGui::SameLine();
            ImGui::InputFloat("Spacing Y", &(leg::selMenu->Spacing[1]));

            if (ImGui::Button("Remove"))
            {
                leg::selection->DeleteComponent(ComponentType::Menu);
                leg::selMenu = NULL;
            }
        }

        if (leg::selPhysics && ImGui::CollapsingHeader("Physics"))
        {
            ImGui::InputFloat("Mass", &(leg::selPhysics->Mass));
            if (ImGui::Button("Remove Physics"))
            {
                leg::selection->DeleteComponent(ComponentType::Physics);
                leg::selPhysics = NULL;
            }
        }

        if (leg::selController && ImGui::CollapsingHeader("Player Controller"))
        {
            ImGui::PushItemWidth(75);
            ImGui::InputInt("Jump Key", &(leg::selController->JumpKey));
            ImGui::PopItemWidth();
            ImGui::InputInt("Move Left Key", &(leg::selController->LeftKey));
            ImGui::InputInt("Move Right Key", &(leg::selController->RightKey));
            ImGui::InputFloat("Jump Speed", &(leg::selController->JumpSpeed));
            ImGui::InputFloat("Move Speed", &(leg::selController->MoveSpeed));
            ImGui::Indent();
            if (ImGui::CollapsingHeader("Edit Sound"))
            {
                ImGui::Checkbox("Walk Sound", &leg::walkSound);
                ImGui::SameLine();
                ImGui::Text(": %s", leg::selController->WalkSound.c_str());
                ImGui::Checkbox("Jump Sound", &leg::jumpSound);
                ImGui::SameLine();
                ImGui::Text(": %s", leg::selController->WalkSound.c_str());
                ImGui::Checkbox("Land Sound", &leg::landSound);
                ImGui::SameLine();
                ImGui::Text(": %s", leg::selController->WalkSound.c_str());

                static ImGuiTextFilter controllerSounds;
                controllerSounds.Draw("Sound", 250);
                ImGui::Text("Available Sounds");
                ImGui::Separator();
                ImGui::BeginChild("Sounds", ImVec2(0, 100));
                for (std::string s : leg::soundNames)
                {
                    if (controllerSounds.PassFilter(s.c_str()))
                    {
                        if (ImGui::MenuItem(s.c_str()))
                        {
                            if (leg::walkSound)
                                leg::selController->WalkSound = s;
                            if (leg::landSound)
                                leg::selController->LandSound = s;
                            if (leg::jumpSound)
                                leg::selController->JumpSound = s;
                        }
                    }
                }
                ImGui::EndChild();
                if (controllerSounds.IsActive() && ImGui::IsKeyPressed(Keys::Return))
                {
                    ForLease->Resources.LoadSound(controllerSounds.InputBuf);
                    leg::soundNames = ForLease->Resources.GetLoadedSoundNames();
                }
            }
            if (ImGui::CollapsingHeader("Edit Animation"))
            {
                ImGui::Checkbox("Walk Animation", &leg::walkAni);
                ImGui::SameLine();
                ImGui::Text(": %s", leg::selController->WalkAnimation.c_str());
                ImGui::Checkbox("Jump Animation", &leg::jumpAni);
                ImGui::SameLine();
                ImGui::Text(": %s", leg::selController->JumpAnimation.c_str());
                static ImGuiTextFilter controllerAni;
                controllerAni.Draw("Animation", 250);
                ImGui::Text("Available Animations");
                ImGui::Separator();
                ImGui::BeginChild("Ani", ImVec2(0, 100));
                for (std::string s : leg::animationNames)
                {
                    if (controllerAni.PassFilter(s.c_str()))
                    {
                        if(ImGui::MenuItem(s.c_str()))
                        {
                            if (leg::walkAni)
                                leg::selController->WalkAnimation = s;
                            if (leg::jumpAni)
                                leg::selController->JumpAnimation = s;
                        }
                    }
                }
                ImGui::EndChild();
                if (controllerAni.IsActive() && ImGui::IsKeyPressed(Keys::Return))
                {
                    ForLease->Resources.LoadMeshAnimation(controllerAni.InputBuf);
                    leg::animationNames = ForLease->Resources.GetLoadedMeshAnimationNames();
                }
            }
            ImGui::Unindent();
            if (ImGui::Button("Remove Controller"))
            {
                leg::selection->DeleteComponent(ComponentType::PlayerController);
                leg::selController = NULL;
            }
        }

        if (leg::selScale && ImGui::CollapsingHeader("Scale with Keyboard"))
        {
            ImGui::Checkbox("Active", &(leg::selScale->Active));
            ImGui::InputFloat("Scale Speed", &(leg::selScale->ScaleSpeed));
            ImGui::InputInt("X up Key", &(leg::selScale->ScaleXUpKey));
            ImGui::SameLine();
            ImGui::InputInt("X down Key", &(leg::selScale->ScaleXDownKey));
            ImGui::InputInt("Y up Key", &(leg::selScale->ScaleYUpKey));
            ImGui::SameLine();
            ImGui::InputInt("Y down Key", &(leg::selScale->ScaleYDownKey));
            ImGui::InputFloat("Min X", &(leg::selScale->MinXScale));
            ImGui::SameLine();
            ImGui::InputFloat("Max X", &(leg::selScale->MaxXScale));
            ImGui::InputFloat("Min Y", &(leg::selScale->MinYScale));
            ImGui::SameLine();
            ImGui::InputFloat("Max Y", &(leg::selScale->MaxYScale));
            ImGui::Text("Scale Sound: %s", leg::selScale->ScaleSound.c_str());
            static ImGuiTextFilter scaleSound;
            scaleSound.Draw("Trigger Sound", 250);
            ImGui::Text("Available Sounds");
            ImGui::Separator();
            ImGui::BeginChild("Sound", ImVec2(0, 100));
            for (std::string s : leg::soundNames)
            {
                if (scaleSound.PassFilter(s.c_str()))
                {
                    if (ImGui::MenuItem(s.c_str()))
                    {
                        leg::selScale->ScaleSound = s;
                    }
                }
            }
            ImGui::EndChild();
            if (scaleSound.IsActive() && ImGui::IsKeyPressed(Keys::Return))
            {
                ForLease->Resources.LoadSound(scaleSound.InputBuf);
                leg::soundNames = ForLease->Resources.GetLoadedSoundNames();
            }
            if (ImGui::Button("Remove Scale with Keyboard"))
            {
                leg::selection->DeleteComponent(ComponentType::ScaleWithKeyboard);
                leg::selScale = NULL;
            }
        }

        if (leg::selSound && ImGui::CollapsingHeader("Sound Emitter"))
        {
            ImGui::Checkbox("Looping", &(leg::selSound->Looping));
            ImGui::SameLine();
            ImGui::PushItemWidth(75);
            ImGui::InputInt("Loop Count", &(leg::selSound->LoopCount));
            ImGui::InputFloat("Volume", &(leg::selSound->Volume));
            ImGui::SameLine();
            ImGui::InputFloat("Pitch", &(leg::selSound->Pitch));
            ImGui::PopItemWidth();
            if (ImGui::Button("Remove Sound Emitter"))
            {
                leg::selection->DeleteComponent(ComponentType::SoundEmitter);
                leg::selSound = NULL;
            }
        }

        if (leg::selSprite && ImGui::CollapsingHeader("Sprite"))
        {
            ImGui::Checkbox("Visible", &(leg::selSprite->Visible));
            ImGui::Checkbox("Flip X", &(leg::selSprite->FlipX));
            ImGui::SameLine();
            ImGui::Checkbox("Flip Y", &(leg::selSprite->FlipY));
            ImGui::PushItemWidth(250);
            ImGui::ColorEdit4("Sprite Color", leg::spriteColor);
            if (ImGui::Button("Set Sprite Color"))
            {
                leg::selSprite->SpriteColor.SetAll(leg::spriteColor[0],
                                                   leg::spriteColor[1],
                                                   leg::spriteColor[2],
                                                   leg::spriteColor[3]);
            }
            ImGui::Text("Current Sprite Source: %s", leg::selSprite->GetSourceName().c_str());
            static ImGuiTextFilter textures;
            textures.Draw("", 300);
            ImGui::Text("Available Sprite Sources");
            ImGui::Separator();
            ImGui::BeginChild("textures", ImVec2(0,100));
            for (std::string s : leg::textureNames)
            {
                if (textures.PassFilter(s.c_str()))
                {
                    if (ImGui::MenuItem(s.c_str()))
                    {
                        leg::selSprite->SetSpriteSource(s);
                    }
                }
            }
            ImGui::EndChild();
            if (textures.IsActive() && ImGui::IsKeyPressed(Keys::Return))
            {
                ForLease->Resources.LoadTexture(textures.InputBuf);
                leg::textureNames = ForLease->Resources.GetLoadedTextureNames();
            }

            if (ImGui::Button("Remove Sprite"))
            {
                leg::selection->DeleteComponent(ComponentType::Sprite);
                leg::selSprite = NULL;
            }
        }

        if (leg::selSprtxt && ImGui::CollapsingHeader("Sprite Text"))
        {
            ImGui::InputText("Text", leg::spriteTextBuf, 500);
            leg::selSprtxt->Text = leg::spriteTextBuf;
            ImGui::PushItemWidth(300);
            ImGui::ColorEdit4("Color", leg::spriteTextColor);
            if (ImGui::Button("Set Text Color"))
            {
                leg::selSprtxt->TextColor.SetAll(leg::spriteTextColor[0],
                                                 leg::spriteTextColor[1],
                                                 leg::spriteTextColor[2],
                                                 leg::spriteTextColor[3]);
            }

            ImGui::PopItemWidth();

            if (ImGui::Button("Remove Sprite Text"))
            {
                leg::selection->DeleteComponent(ComponentType::SpriteText);
                leg::selSprtxt = NULL;
            }
        }

        if (leg::selTMC && ImGui::CollapsingHeader("Transform Control"))
        {
            ImGui::PushItemWidth(200);
            ImGui::InputInt("Toggle Key", &(leg::selTMC->ModeToggleKey));
            ImGui::InputFloat("Normal Speed", &(leg::selTMC->NormalSpeed));
            ImGui::InputFloat("Slow Motion Speed", &(leg::selTMC->SlowMotionSpeed));
            ImGui::PopItemWidth();
            ImGui::Text("Sound: %s", leg::selTMC->TransformModeSound.c_str());
            static ImGuiTextFilter tmcSound;
            tmcSound.Draw("Trigger Sound", 250);
            ImGui::Text("Available Sounds");
            ImGui::Separator();
            ImGui::BeginChild("Sound", ImVec2(0, 100));
            for (std::string s : leg::soundNames)
            {
                if (tmcSound.PassFilter(s.c_str()))
                {
                    if (ImGui::MenuItem(s.c_str()))
                    {
                        leg::selTMC->TransformModeSound = s;
                    }
                }
            }
            ImGui::EndChild();
            if (tmcSound.IsActive() && ImGui::IsKeyPressed(Keys::Return))
            {
                ForLease->Resources.LoadSound(tmcSound.InputBuf);
                leg::soundNames = ForLease->Resources.GetLoadedSoundNames();
            }
            if (ImGui::Button("Remove Transform Control"))
            {
                leg::selection->DeleteComponent(ComponentType::TransformModeControls);
                leg::selTMC = NULL;
            }
        }

        if (leg::selVision && ImGui::CollapsingHeader("Vision Cone"))
        {
            ImGui::Checkbox("Active", &(leg::selVision->Active));
            ImGui::SameLine();
            ImGui::Checkbox("Draw Outline", &(leg::selVision->DrawOutline));
            ImGui::SameLine();
            ImGui::Checkbox("Visible", &(leg::selVision->Visible));
            ImGui::DragFloat("Angle", &(leg::selVision->Angle), 0.05);
            ImGui::DragFloat("Direction X", &(leg::selVision->Direction[0]), 0.01);
            ImGui::SameLine();
            ImGui::DragFloat("Direction Y", &(leg::selVision->Direction[1]), 0.01);
            ImGui::DragFloat("Radius", &(leg::selVision->Radius), 0.25);
            ImGui::PushItemWidth(250);
            ImGui::ColorEdit4("Indicator Color", leg::visionConeColor);
            ImGui::PopItemWidth();
            if (ImGui::Button("Set Indicator Color"))
            {
                leg::selVision->IndicatorColor.SetAll(leg::visionConeColor[0],
                                                      leg::visionConeColor[1],
                                                      leg::visionConeColor[2],
                                                      leg::visionConeColor[3]);
            }

            if (ImGui::Button("Remove Vision Cone"))
            {
                leg::selection->DeleteComponent(ComponentType::VisionCone);
                leg::selVision = NULL;
            }
        }

        ImGui::Separator();
        if (ImGui::Button("Delete Object"))
            leg::delobj = true;

        ImGui::End();
    }

    static void GetMouse(Point& v)
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
            std::vector<Entity*> ents = GetEntitiesAtPosition(leg::mousePos);
            leg::selection = NULL;

            for (unsigned i = 0; i < ents.size(); i++)
            {
                if (ents[i] != leg::camera)
                {
                    leg::selection = ents[i];
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
                    leg::selSound      = leg::selection->GetComponent<Components::SoundEmitter>();
                    leg::selSprite     = leg::selection->GetComponent<Components::Sprite>();
                    leg::selSprtxt     = leg::selection->GetComponent<Components::SpriteText>();
                    leg::selController = leg::selection->GetComponent<Components::CharacterController>();
                    leg::selEnemyAI    = leg::selection->GetComponent<Components::EnemyAI>();
                    leg::selLight      = leg::selection->GetComponent<Components::Light>();
                    leg::selFade       = leg::selection->GetComponent<Components::FadeWithDistance>();
                    leg::selChange     = leg::selection->GetComponent<Components::ChangeLevelOnCollide>();
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
                    break;
                }
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
            leg::selection->CreateArchetype(leg::copyfile);
            leg::copySet = true;
        }

        if (leg::copySet && ImGui::GetIO().KeyCtrl && ImGui::IsKeyPressed(Keys::V))
        {
            GetMouse(leg::mousePos);
            SpawnArchetype(leg::copyfile, leg::mousePos);
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
        root.WriteStringArray("Sounds", &(leg::soundNames[0]), leg::soundNames.size());
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
            root.ReadStringArray("Sounds", leg::soundNames);
            root.ReadStringArray("Animations", leg::animationNames);
            root.ReadStringArray("Fonts", leg::fontNames);
            root.ReadStringArray("Textures", leg::textureNames);
            for (auto i : leg::meshNames)
                ForLease->Resources.LoadMesh(i);
            for (auto i : leg::soundNames)
                ForLease->Resources.LoadSound(i);
            for (auto i : leg::animationNames)
                ForLease->Resources.LoadMeshAnimation(i);
            for (auto i : leg::fontNames)
                ForLease->Resources.LoadFont(i);
            for (auto i : leg::textureNames)
                ForLease->Resources.LoadTexture(i);

            leg::meshNames = ForLease->Resources.GetLoadedMeshNames();
            leg::soundNames = ForLease->Resources.GetLoadedSoundNames();
            leg::animationNames = ForLease->Resources.GetLoadedMeshAnimationNames();
            leg::fontNames = ForLease->Resources.GetLoadedFontNames();
            leg::textureNames = ForLease->Resources.GetLoadedTextureNames();
        }
    }
}

