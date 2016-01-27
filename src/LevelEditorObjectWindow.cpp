#include "LevelEditor.h"
#include "LevelEditorGlobals.h"
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
    namespace leg = LevelEditorGlobals;

    void LevelEditor::DrawObjectWindow()
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
//        if (leg::selMusic && ImGui::CollapsingHeader("Background Music"))
//        {
//            ImGui::Text("Current Sound: %s", leg::selMusic->MusicName.c_str());
//            static ImGuiTextFilter backSound;
//            backSound.Draw("Trigger Sound", 250);
//            ImGui::Text("Available Sounds");
//            ImGui::Separator();
//            ImGui::BeginChild("Sound", ImVec2(0, 100));
//            for (std::string s : leg::soundNames)
//            {
//                if (backSound.PassFilter(s.c_str()))
//                {
//                    if (ImGui::MenuItem(s.c_str()))
//                    {
//                        leg::selMusic->MusicName = s;
//                    }
//                }
//            }
//            ImGui::EndChild();
//            if (backSound.IsActive() && ImGui::IsKeyPressed(Keys::Return))
//            {
//                ForLease->Resources.LoadSound(backSound.InputBuf);
//                leg::soundNames = ForLease->Resources.GetLoadedSoundNames();
//            }
//            if (ImGui::Button("Remove Background Music"))
//            {
//                leg::selection->DeleteComponent(ComponentType::BackgroundMusic);
//                leg::selMusic = NULL;
//            }
//        }
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

//            ImGui::Text("Current Sound: %s", leg::selChange->TriggerSoundName.c_str());
//            static ImGuiTextFilter changeSound;
//            changeSound.Draw("Trigger Sound", 250);
//            ImGui::Text("Available Sounds");
//            ImGui::Separator();
//            ImGui::BeginChild("Sound", ImVec2(0, 100));
//            for (std::string s : leg::soundNames)
//            {
//                if (changeSound.PassFilter(s.c_str()))
//                {
//                    if (ImGui::MenuItem(s.c_str()))
//                    {
//                        leg::selChange->TriggerSoundName = s;
//                    }
//                }
//            }
//            ImGui::EndChild();
//            if (changeSound.IsActive() && ImGui::IsKeyPressed(Keys::Return))
//            {
//                ForLease->Resources.LoadSound(changeSound.InputBuf);
//                leg::soundNames = ForLease->Resources.GetLoadedSoundNames();
//            }
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
            ImGui::ColorEdit4("Happy Color", const_cast<float*>(leg::selEnemyAI->HappyColor.GetAll()));
            ImGui::ColorEdit4("Detection Color", const_cast<float*>(leg::selEnemyAI->DetectionColor.GetAll()));
            ImGui::ColorEdit4("No Detection Color", const_cast<float*>(leg::selEnemyAI->NoDetectionColor.GetAll()));
            ImGui::PopItemWidth();

            if (ImGui::InputText("Hated Entity Name", leg::enemyHateN, 70, ImGuiInputTextFlags_EnterReturnsTrue))
                leg::selEnemyAI->HatedEntityName = leg::enemyHateN;

            if (ImGui::InputText("Liked Entity Name", leg::enemyLikeN, 70, ImGuiInputTextFlags_EnterReturnsTrue))
                leg::selEnemyAI->LikedEntityName = leg::enemyLikeN;

//            ImGui::Checkbox("Liked Sound", &(leg::setLiked));
//            if (ImGui::IsItemHovered())
//            {
//                ImGui::BeginTooltip();
//                ImGui::Text("%s", (leg::selEnemyAI->LikedSeenSound.c_str()));
//                ImGui::EndTooltip();
//            }
//            ImGui::SameLine();
//            ImGui::Checkbox("Hated Sound", &(leg::setHated));
//            if (ImGui::IsItemHovered())
//            {
//                ImGui::BeginTooltip();
//                ImGui::Text("%s", (leg::selEnemyAI->HatedSeenSound.c_str()));
//                ImGui::EndTooltip();
//            }
//
//            static ImGuiTextFilter sounds;
//            sounds.Draw("Sound", 300);
//            ImGui::Text("Available Sounds");
//            ImGui::Separator();
//            ImGui::BeginChild("Sounds", ImVec2(0, 100));
//            for (std::string s : leg::soundNames)
//            {
//                if (sounds.PassFilter(s.c_str()))
//                {
//                    if (ImGui::MenuItem(s.c_str()))
//                    {
//                        if (leg::setHated)
//                            leg::selEnemyAI->HatedSeenSound = s;
//                        if (leg::setLiked)
//                            leg::selEnemyAI->LikedSeenSound = s;
//                    }
//                }
//            }
//            ImGui::EndChild();
//            if (sounds.IsActive() && ImGui::IsKeyPressed(Keys::Return))
//            {
//                ForLease->Resources.LoadSound(sounds.InputBuf);
//                leg::soundNames = ForLease->Resources.GetLoadedSoundNames();
//            }
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
            ImGui::Text("Current Target ID: %lu", leg::selFade->TrackedEntityID);
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
            ImGui::ColorEdit4("Light Color", const_cast<float*>(leg::selLight->DrawColor.GetAll()));

            if (ImGui::Button("Remove Light"))
            {
                leg::selection->DeleteComponent(ComponentType::Light);
                leg::selLight = NULL;
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
//            if (ImGui::CollapsingHeader("Edit Sound"))
//            {
//                ImGui::Checkbox("Walk Sound", &leg::walkSound);
//                ImGui::SameLine();
//                ImGui::Text(": %s", leg::selController->WalkSound.c_str());
//                ImGui::Checkbox("Jump Sound", &leg::jumpSound);
//                ImGui::SameLine();
//                ImGui::Text(": %s", leg::selController->JumpSound.c_str());
//                ImGui::Checkbox("Land Sound", &leg::landSound);
//                ImGui::SameLine();
//                ImGui::Text(": %s", leg::selController->LandSound.c_str());
//
//                static ImGuiTextFilter controllerSounds;
//                controllerSounds.Draw("Sound", 250);
//                ImGui::Text("Available Sounds");
//                ImGui::Separator();
//                ImGui::BeginChild("Sounds", ImVec2(0, 100));
//                for (std::string s : leg::soundNames)
//                {
//                    if (controllerSounds.PassFilter(s.c_str()))
//                    {
//                        if (ImGui::MenuItem(s.c_str()))
//                        {
//                            if (leg::walkSound)
//                                leg::selController->WalkSound = s;
//                            if (leg::landSound)
//                                leg::selController->LandSound = s;
//                            if (leg::jumpSound)
//                                leg::selController->JumpSound = s;
//                        }
//                    }
//                }
//                ImGui::EndChild();
//                if (controllerSounds.IsActive() && ImGui::IsKeyPressed(Keys::Return))
//                {
//                    ForLease->Resources.LoadSound(controllerSounds.InputBuf);
//                    leg::soundNames = ForLease->Resources.GetLoadedSoundNames();
//                }
//            }
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
//            static ImGuiTextFilter scaleSound;
//            scaleSound.Draw("Trigger Sound", 250);
//            ImGui::Text("Available Sounds");
//            ImGui::Separator();
//            ImGui::BeginChild("Sound", ImVec2(0, 100));
//            for (std::string s : leg::soundNames)
//            {
//                if (scaleSound.PassFilter(s.c_str()))
//                {
//                    if (ImGui::MenuItem(s.c_str()))
//                    {
//                        leg::selScale->ScaleSound = s;
//                    }
//                }
//            }
//            ImGui::EndChild();
//            if (scaleSound.IsActive() && ImGui::IsKeyPressed(Keys::Return))
//            {
//                ForLease->Resources.LoadSound(scaleSound.InputBuf);
//                leg::soundNames = ForLease->Resources.GetLoadedSoundNames();
//            }
            if (ImGui::Button("Remove Scale with Keyboard"))
            {
                leg::selection->DeleteComponent(ComponentType::ScaleWithKeyboard);
                leg::selScale = NULL;
            }
        }

//        if (leg::selSound && ImGui::CollapsingHeader("Sound Emitter"))
//        {
//            ImGui::Checkbox("Looping", &(leg::selSound->Looping));
//            ImGui::SameLine();
//            ImGui::PushItemWidth(75);
//            ImGui::InputInt("Loop Count", &(leg::selSound->LoopCount));
//            ImGui::InputFloat("Volume", &(leg::selSound->Volume));
//            ImGui::SameLine();
//            ImGui::InputFloat("Pitch", &(leg::selSound->Pitch));
//            ImGui::PopItemWidth();
//            if (ImGui::Button("Remove Sound Emitter"))
//            {
//                leg::selection->DeleteComponent(ComponentType::SoundEmitter);
//                leg::selSound = NULL;
//            }
//        }

        if (leg::selSprite && ImGui::CollapsingHeader("Sprite"))
        {
            ImGui::Checkbox("Visible", &(leg::selSprite->Visible));
            ImGui::Checkbox("Flip X", &(leg::selSprite->FlipX));
            ImGui::SameLine();
            ImGui::Checkbox("Flip Y", &(leg::selSprite->FlipY));
            ImGui::PushItemWidth(250);
            ImGui::ColorEdit4("Sprite Color", const_cast<float*>(leg::selSprite->SpriteColor.GetAll()));
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
            ImGui::ColorEdit4("Color", const_cast<float*>(leg::selSprtxt->TextColor.GetAll()));
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
//            ImGui::Text("Sound: %s", leg::selTMC->TransformModeSound.c_str());
//            static ImGuiTextFilter tmcSound;
//            tmcSound.Draw("Trigger Sound", 250);
//            ImGui::Text("Available Sounds");
//            ImGui::Separator();
//            ImGui::BeginChild("Sound", ImVec2(0, 100));
//            for (std::string s : leg::soundNames)
//            {
//                if (tmcSound.PassFilter(s.c_str()))
//                {
//                    if (ImGui::MenuItem(s.c_str()))
//                    {
//                        leg::selTMC->TransformModeSound = s;
//                    }
//                }
//            }
//            ImGui::EndChild();
//            if (tmcSound.IsActive() && ImGui::IsKeyPressed(Keys::Return))
//            {
//                ForLease->Resources.LoadSound(tmcSound.InputBuf);
//                leg::soundNames = ForLease->Resources.GetLoadedSoundNames();
//            }
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
}
