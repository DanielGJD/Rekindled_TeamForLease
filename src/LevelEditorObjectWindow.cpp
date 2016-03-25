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
#include <unordered_map>
namespace ForLeaseEngine
{
    namespace leg = LevelEditorGlobals;

    void LevelEditor::DrawObjectWindow()
    {
        float width = 100;
        ImGui::Begin("Object Editor");

        if (ImGui::InputText("Name##Object", leg::entName, 128, ImGuiInputTextFlags_EnterReturnsTrue))
            leg::selection->SetName(leg::entName);

        ImGui::InputText("Blueprint Name##Blueprint", leg::archetypefile, 128);
        if (ImGui::Button("Save Blueprint"))
        {
            std::string s = leg::blueprintDir + leg::archetypefile;
            leg::selection->CreateArchetype(s);
            if (std::find(leg::archetypeNames.begin(), leg::archetypeNames.end(), leg::archetypefile) == leg::archetypeNames.end())
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
                    if (ImGui::IsItemHovered())
                    {
                        std::string tooltip = RequiredToolTip(leg::componentNames[i]);
                        ImGui::BeginTooltip();
                        ImGui::Text("%s", tooltip.c_str());
                        ImGui::EndTooltip();
                    }
                }
            }
            ImGui::EndChild();
        }

        if (leg::selTran && ImGui::CollapsingHeader("Transform"))
        {
            ImGui::Checkbox("UI Layer##transform", &(leg::selTran->UILayer));
            ImGui::PushItemWidth(width);
            ImGui::InputFloat("X##TransformPos", &(leg::selTran->Position[0]));
            ImGui::SameLine();
            ImGui::InputFloat("Y##TransformPos", &(leg::selTran->Position[1]));
            ImGui::SameLine();
            ImGui::Text("Position");
            ImGui::DragFloat("X##TransformScale", &(leg::selTran->ScaleX), 0.01);
            ImGui::SameLine();
            ImGui::DragFloat("Y##TransformScale", &(leg::selTran->ScaleY), 0.01);
            ImGui::SameLine();
            ImGui::Text("Scale");
            ImGui::PopItemWidth();
            ImGui::SliderAngle("Rotation##Transform", &(leg::selTran->Rotation), 0, 360);
            ImGui::InputInt("Z Order##Transform", &(leg::selTran->ZOrder));
        }

        if (leg::selModel && ImGui::CollapsingHeader("Model"))
        {
            ImGui::Checkbox("Flip X##Model", &(leg::selModel->FlipX));
            ImGui::SameLine();
            ImGui::Checkbox("Flip Y##Model", &(leg::selModel->FlipY));
            ImGui::Combo("Blending Mode##model", &leg::modelBlend, "None\0Alpha\0Additive\0Multiply\0\0");
            ImGui::ColorEdit4("Color##model", const_cast<float*>(leg::selModel->ModelColor.GetAll()));
            leg::selModel->BlendingMode = static_cast<BlendMode>(leg::modelBlend);
            if (ImGui::TreeNode("Edit Mesh##model"))
            {
                static ImGuiTextFilter meshFilter;
                ImGui::Text("Current Mesh: %s", leg::selModel->ModelMesh.c_str());
                if (ImGui::Button("Clear Mesh"))
                    leg::selModel->ModelMesh = "";
                meshFilter.Draw("##Mesh", 250.0f);
                ImGui::Text("Available Meshes");
                ImGui::BeginChild("Meshes", ImVec2(0, 100), true);
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
                ImGui::TreePop();
            }

            if (ImGui::TreeNode("Edit Animation##model"))
            {
                ImGui::Checkbox("Active##Model", &(leg::selModel->AnimationActive));
                ImGui::Checkbox("Looping##Model", &(leg::selModel->Looping));
                ImGui::InputFloat("Frame Rate##Model", &(leg::selModel->FrameRate));
                ImGui::Text("Current Animation: %s", leg::selModel->GetAnimation().c_str());
                if (ImGui::Button("Clear Animation##model"))
                    leg::selModel->SetAnimation("");
                static ImGuiTextFilter animations;
                animations.Draw("##Animation", 250);
                ImGui::Text("Available Animations");
                ImGui::BeginChild("Animations", ImVec2(0, 100), true);
                for (std::string s : leg::animationNames)
                {
                    if (animations.PassFilter(s.c_str()))
                    {
                        if (ImGui::MenuItem(s.c_str()))
                            leg::selModel->SetAnimation(s);
                    }
                }
                ImGui::EndChild();
                ImGui::TreePop();
            }

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
            backSound.Draw("##background", 250);
            ImGui::Text("Available Sounds");
            ImGui::BeginChild("Sound", ImVec2(0, 100), true);
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
            if (ImGui::Button("Remove Background Music"))
            {
                leg::selection->DeleteComponent(ComponentType::BackgroundMusic);
                leg::selMusic = NULL;
            }
        }
        if (leg::selCamera && ImGui::CollapsingHeader("Camera"))
        {
            ImGui::InputFloat("Size##Camera", &(leg::selCamera->Size));
            if (ImGui::Button("Set Camera"))
                leg::levelCamera = leg::selection;

            if (ImGui::Button("Remove Camera"))
            {
                leg::selection->DeleteComponent(ComponentType::Camera);
                leg::selCamera = NULL;
            }
        }
        if (leg::selChange && ImGui::CollapsingHeader("Change Level on Collide"))
        {
            ImGui::PushItemWidth(width * 2);
            ImGui::Checkbox("Active##CLoC", &(leg::selChange->Active));
            ImGui::InputFloat("On Switch Time Scale##CLoC", &(leg::selChange->OnSwitchTimeScale));
            ImGui::InputFloat("Fade Out Time##CLoC", &(leg::selChange->FadeOutTime));
            ImGui::Text("Level Name: %s", leg::selChange->LevelName.c_str());
            if (ImGui::InputText("##CLoC", leg::changeLevel, 128, ImGuiInputTextFlags_EnterReturnsTrue))
                leg::selChange->LevelName = leg::changeLevel;
            ImGui::Text("Trigger Object Name: %s", leg::selChange->TriggerObjectName.c_str());
            if (ImGui::InputText("##CLoCObject", leg::changeObject, 128, ImGuiInputTextFlags_EnterReturnsTrue))
                leg::selChange->TriggerObjectName = leg::changeObject;

            ImGui::PopItemWidth();
            ImGui::Text("Current Sound: %s", leg::selChange->TriggerSoundName.c_str());
            static ImGuiTextFilter changeSound;
            changeSound.Draw("##clocsound", 250);
            ImGui::Text("Available Sounds");
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

            if (ImGui::Button("Clear Sound##cloc"))
                leg::selChange->TriggerSoundName = "";

            if (ImGui::Button("Remove Change on Collide"))
            {
                leg::selection->DeleteComponent(ComponentType::ChangeLevelOnCollide);
                leg::selChange = NULL;
            }
        }
        if (leg::selCheckpoint && ImGui::CollapsingHeader("Checkpoint"))
        {
            if (ImGui::Button("Remove Checkpoint"))
            {
                leg::selection->DeleteComponent(ComponentType::Checkpoint);
                leg::selCheckpoint = NULL;
            }
        }
        if (leg::selCollision && ImGui::CollapsingHeader("Collision"))
        {
            ImGui::PushItemWidth(width);
            ImGui::DragFloat("Width##Collision", &(leg::selCollision->Width), 0.05);
            ImGui::SameLine();
            ImGui::DragFloat("Height##Collision", &(leg::selCollision->Height), 0.05);
            ImGui::PopItemWidth();
            ImGui::Checkbox("Resolve Collision##Collision", &(leg::selCollision->ResolveCollisions));

            if (ImGui::Button("Remove Collision"))
            {
                leg::selection->DeleteComponent(ComponentType::Collision);
                leg::selCollision = NULL;
            }
        }

        if (leg::selDrag && ImGui::CollapsingHeader("Drag with Mouse"))
        {
            ImGui::Checkbox("Active##DWM", &(leg::selDrag->Active));
            if (ImGui::Button("Remove Drag With Mouse"))
            {
                leg::selection->DeleteComponent(ComponentType::DragWithMouse);
                leg::selDrag = NULL;
            }
        }

        if (leg::selEnemyAI && ImGui::CollapsingHeader("Enemy AI"))
        {
            ImGui::PushItemWidth(width * 2);
            ImGui::Checkbox("Active##EnemyAI", &(leg::selEnemyAI->Active));
            ImGui::InputFloat("Detection Delay##EnemyAI", &(leg::selEnemyAI->DetectionDelay));
            ImGui::ColorEdit4("Happy Color##EnemyAI", const_cast<float*>(leg::selEnemyAI->HappyColor.GetAll()));
            ImGui::ColorEdit4("Detection Color##EnemyAI", const_cast<float*>(leg::selEnemyAI->DetectionColor.GetAll()));
            ImGui::ColorEdit4("No Detection Color##EnemyAI", const_cast<float*>(leg::selEnemyAI->NoDetectionColor.GetAll()));

            if (ImGui::InputText("Hated Entity Name##EnemyAI", leg::enemyHateName, 128, ImGuiInputTextFlags_EnterReturnsTrue))
                leg::selEnemyAI->HatedEntityName = leg::enemyHateName;

            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                ImGui::Text("Press ENTER to set");
                ImGui::EndTooltip();
            }

            if (ImGui::InputText("Liked Entity Name##EnemyAI", leg::enemyLikeName, 128, ImGuiInputTextFlags_EnterReturnsTrue))
                leg::selEnemyAI->LikedEntityName = leg::enemyLikeName;

            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                ImGui::Text("Press ENTER to set");
                ImGui::EndTooltip();
            }

            ImGui::PopItemWidth();
            if (ImGui::TreeNode("Edit Sound##enemyAI"))
            {
                static int selButton = 0;
                ImGui::RadioButton("Hated Sound##enemyAI", &selButton, 0);
                ImGui::SameLine();
                ImGui::Text(": %s", leg::selEnemyAI->HatedSeenSound.c_str());
                ImGui::RadioButton("Liked Sound##enemyAI", &selButton, 1);
                ImGui::SameLine();
                ImGui::Text(": %s", leg::selEnemyAI->LikedSeenSound.c_str());

                static ImGuiTextFilter sounds;
                sounds.Draw("##enemyAI", 300);
                ImGui::Text("Available Sounds");
                ImGui::BeginChild("Sounds", ImVec2(0, 100), true);
                for (std::string s : leg::soundNames)
                {
                    if (sounds.PassFilter(s.c_str()))
                    {
                        if (ImGui::MenuItem(s.c_str()))
                        {
                            switch (selButton)
                            {
                            case 0:
                                leg::selEnemyAI->HatedSeenSound = s;
                                break;
                            case 1:
                                leg::selEnemyAI->LikedSeenSound = s;
                                break;
                            }
                        }
                    }
                }
                ImGui::EndChild();

                if (ImGui::Button("Clear Sound##enemyAI"))
                {
                    switch (selButton)
                    {
                    case 0:
                        leg::selEnemyAI->HatedSeenSound = "";
                        break;
                    case 1:
                        leg::selEnemyAI->LikedSeenSound = "";
                        break;
                    }
                }

                ImGui::TreePop();
            }

            if (ImGui::Button("Remove Enemy AI"))
            {
                leg::selection->DeleteComponent(ComponentType::EnemyAI);
                leg::selEnemyAI = NULL;
            }
        }
        if (leg::selFade && ImGui::CollapsingHeader("Fade with Distance"))
        {
            ImGui::PushItemWidth(width * 2);
            ImGui::Checkbox("Active##FWD", &(leg::selFade->Active));
            ImGui::Checkbox("Fade with X##FWD", &(leg::selFade->XDirection));
            ImGui::Checkbox("Fade with Y##FWD", &(leg::selFade->YDirection));
            ImGui::DragFloat("Begin Distance##FWD", &(leg::selFade->FadeBeginDistance), 0.05, 0);
            ImGui::DragFloat("End Distance##FWD", &(leg::selFade->FadeEndDistance), 0.05, 0);
            ImGui::Checkbox("Select Target##FWD", &leg::setFade);
            ImGui::PopItemWidth();
            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                ImGui::Text("Once this is checked, click the entity you want to select");
                ImGui::EndTooltip();
            }

            ImGui::Text("Current Target ID: %lu", leg::selFade->TrackedEntityID);
            if (ImGui::Button("Remove Fade with Distance"))
            {
                leg::setFade = false;
                leg::selection->DeleteComponent(ComponentType::FadeWithDistance);
                leg::selFade = NULL;
            }
        }
        if (leg::selFollow && ImGui::CollapsingHeader("Follow"))
        {
            ImGui::Checkbox("Active##follow", &(leg::selFollow->Active));
            ImGui::InputFloat("Begin Distance##follow", &(leg::selFollow->FollowBeginDistance));
            ImGui::InputFloat("End Distance##follow", &(leg::selFollow->FollowEndDistance));
            ImGui::PushItemWidth(width);
            ImGui::InputFloat("X##follow", &(leg::selFollow->Offset[0]));
            ImGui::SameLine();
            ImGui::InputFloat("Y##follow", &(leg::selFollow->Offset[1]));
            ImGui::SameLine();
            ImGui::Text("Offset");
            ImGui::PopItemWidth();
            ImGui::Checkbox("Follow Target ID: ", &leg::setFollow);
            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                ImGui::Text("Once this is checked, click the entity you want to select");
                ImGui::EndTooltip();
            }
            ImGui::SameLine();
            ImGui::Text("%lu", leg::selFollow->FollowEntityID);
            if (ImGui::Button("Remove Follow"))
            {
                leg::setFollow = false;
                leg::selection->DeleteComponent(ComponentType::Follow);
                leg::selFollow = NULL;
            }
        }
        if (leg::selHealth && ImGui::CollapsingHeader("Health"))
        {
            ImGui::InputFloat("Max Health", &(leg::selHealth->MaxHealth));
            ImGui::InputFloat("Damage Scale", &(leg::selHealth->DamageScale));
            ImGui::InputFloat("Regen Scale", &(leg::selHealth->RegenScale));

            if (ImGui::Button("Remove Health"))
            {
                leg::selection->DeleteComponent(ComponentType::Health);
                leg::selHealth = NULL;
            }
        }
        if (leg::selLight && ImGui::CollapsingHeader("Light"))
        {
            ImGui::Checkbox("Active##Light", &(leg::selLight->Active));
            ImGui::Checkbox("Visible##Light", &(leg::selLight->Visible));
            ImGui::Checkbox("Draw Outline##Light", &(leg::selLight->DrawOutline));
            ImGui::Checkbox("Angle = 360", &(leg::selLight->PointLight));
            ImGui::Combo("Blending Mode##light", &leg::lightBlend, "None\0Alpha\0Additive\0Multiply\0\0");
            leg::selLight->LightMode = static_cast<BlendMode>(leg::lightBlend);
            ImGui::SliderAngle("Angle##Light", &(leg::selLight->Angle), 0, 360);
            ImGui::SliderAngle("Direction##light", &leg::lightAngle, 0, 360);
            leg::selLight->Direction[0] = cos(leg::lightAngle);
            leg::selLight->Direction[1] = sin(leg::lightAngle);
            ImGui::DragFloat("Radius##light", &(leg::selLight->Radius), 0.01);
            ImGui::PushItemWidth(width);
            ImGui::DragFloat("X##LightOffset", &(leg::selLight->Offset[0]), 0.05);
            ImGui::SameLine();
            ImGui::DragFloat("Y##LightOffset", &(leg::selLight->Offset[1]), 0.05);
            ImGui::SameLine();
            ImGui::Text("Offset");
            ImGui::PopItemWidth();
            ImGui::ColorEdit4("Light Color", const_cast<float*>(leg::selLight->LightColor.GetAll()));
            if (ImGui::TreeNode("Edit Texture##light"))
            {
                ImGui::Text("Current Texture: %s", leg::selLight->LightTexture.c_str());

                static ImGuiTextFilter textures;
                textures.Draw("##light", 300);
                ImGui::Text("Available Textures");
                ImGui::BeginChild("textures", ImVec2(0, 100), true);
                for (std::string s : leg::textureNames)
                {
                    if (textures.PassFilter(s.c_str()))
                    {
                        if (ImGui::MenuItem(s.c_str()))
                        {
                            leg::selLight->LightTexture = s;
                        }
                    }
                }
                ImGui::EndChild();

                if (ImGui::Button("Clear Texture##light"))
                {
                    leg::selLight->LightTexture = "";
                }

                ImGui::TreePop();
            }
            if (ImGui::Button("Remove Light"))
            {
                leg::selection->DeleteComponent(ComponentType::Light);
                leg::selLight = NULL;
            }
        }
        if (leg::selOccluder && ImGui::CollapsingHeader("Occluder"))
        {
            ImGui::Checkbox("Blocks Light##occluder", &(leg::selOccluder->BlocksLight));
            ImGui::Checkbox("Blocks Vision##occluder", &(leg::selOccluder->BlocksVision));

            if (ImGui::Button("Remove Occluder"))
            {
                leg::selection->DeleteComponent(ComponentType::Occluder);
                leg::selOccluder = NULL;
            }
        }

        if (leg::selOwl && ImGui::CollapsingHeader("OwlAI"))
        {
            ImGui::InputFloat("Blink Time##OwlAI", &(leg::selOwl->BlinkTime));
            ImGui::InputFloat("Watch Time##OwlAI", &(leg::selOwl->WatchTime));
            ImGui::SliderAngle("Direction 1##owlAI", &leg::owlDir1, 0, 360);
            leg::selOwl->Direction1[0] = cos(leg::owlDir1);
            leg::selOwl->Direction1[1] = sin(leg::owlDir1);
            leg::selVision->Direction = leg::selOwl->Direction1;
            ImGui::SliderAngle("Direction 2##owlAI", &leg::owlDir2, 0, 360);
            leg::selOwl->Direction2[0] = cos(leg::owlDir2);
            leg::selOwl->Direction2[1] = sin(leg::owlDir2);

            if (ImGui::Button("Remove OwlAI"))
            {
                leg::selection->DeleteComponent(ComponentType::OwlAI);
                leg::selOwl = NULL;
            }
        }

        if (leg::selPace && ImGui::CollapsingHeader("PaceAI"))
        {
            ImGui::InputFloat("Speed##pace", &(leg::selPace->PaceSpeed));
            ImGui::InputFloat("Distance##pace", &(leg::selPace->MaxPaceDistance));
            ImGui::InputFloat("Pause Timer##pace", &(leg::selPace->PauseTimer));
            ImGui::InputFloat("Detection Delay##pace", &(leg::selPace->DetectionDelay));
            ImGui::InputFloat("Resume Delay##pace", &(leg::selPace->ResumeTime));
            ImGui::RadioButton("Horizontal", &(leg::selPace->Direction), 0);
            ImGui::RadioButton("Vertical", &(leg::selPace->Direction), 1);

            if (ImGui::Button("Remove PaceAI"))
            {
                leg::selection->DeleteComponent(ComponentType::EnemyPace);
                leg::selPace = NULL;
            }
        }
        if (leg::selParallax && ImGui::CollapsingHeader("Parallax"))
        {
            ImGui::Checkbox("Active", &(leg::selParallax->Active));
            ImGui::Checkbox("Repeating", &(leg::selParallax->Repeating));

            if (ImGui::Button("Remove Parallax"))
            {
                leg::selection->DeleteComponent(ComponentType::Parallax);
                leg::selParallax = NULL;
            }
        }
        if (leg::selPartColor && ImGui::CollapsingHeader("Particle Color"))
        {
            ImGui::Checkbox("Active##PartColor", &(leg::selPartColor->Active));
            ImGui::Text("Colors");
            ImGui::Separator();
            ImGui::BeginChild("Color List", ImVec2(0, 100));
            for(unsigned i = 0; i < leg::selPartColor->GetColorCount(); i++)
            {
                char s[32];
                float * col = const_cast<float*>(leg::selPartColor->GetColor(i).GetAll());
                ImVec4 col2(col[0], col[1], col[2], col[3]);
                sprintf(s, "Remove##Color%u", i+1);
                ImGui::ColorButton(col2);
                ImGui::SameLine();
                if (ImGui::Button(s))
                    leg::selPartColor->RemoveColor(i);
            }
            ImGui::EndChild();

            static float newColor[4] = {1, 1, 1, 1};
            Color c;
            ImGui::PushItemWidth(250);
            ImGui::ColorEdit4("New Color##PartColor", newColor);
            ImGui::PopItemWidth();
            if (ImGui::Button("Add Color##PartColor"))
            {
                c.SetAll(newColor);
                leg::selPartColor->AddColor(c);
            }

            if (ImGui::Button("Remove Particle Color"))
            {
                leg::selection->DeleteComponent(ComponentType::ParticleColorAnimator);
                leg::selPartColor = NULL;
            }
        }
        if (leg::selPartDynamics && ImGui::CollapsingHeader("Particle Dynamics"))
        {
            ImGui::Checkbox("Active##PartDynamics", &(leg::selPartDynamics->Active));
            ImGui::PushItemWidth(width);
            ImGui::InputFloat("X##PartDynamics", &(leg::selPartDynamics->Force[0]));
            ImGui::SameLine();
            ImGui::InputFloat("Y##PartDynamics", &(leg::selPartDynamics->Force[1]));
            ImGui::SameLine();
            ImGui::Text("Force");
            ImGui::PopItemWidth();
            ImGui::DragFloat("Torque##PartDynamics", &(leg::selPartDynamics->Torque), 0.01);
            ImGui::DragFloat("Growth##PartDynamics", &(leg::selPartDynamics->Growth), 0.01);
            ImGui::DragFloat("Drag##PartDynamics", &(leg::selPartDynamics->Drag), 0.01);

            if (ImGui::Button("Remove Particle Dynamics"))
            {
                leg::selection->DeleteComponent(ComponentType::SimpleParticleDynamics);
                leg::selPartDynamics = NULL;
            }
        }

        if (leg::selPartEmitter && ImGui::CollapsingHeader("Particle Emitter"))
        {
            ImGui::Checkbox("Active##PartEmitter", &(leg::selPartEmitter->Active));
            if (ImGui::InputInt("Emit Count##PartEmitter", &leg::eCount, 1, 1, ImGuiInputTextFlags_EnterReturnsTrue) && leg::eCount > 0)
                leg::selPartEmitter->EmitCount = leg::eCount;

            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                ImGui::Text("Press ENTER to set");
                ImGui::EndTooltip();
            }

            ImGui::PushItemWidth(width * 2);
            ImGui::DragFloat("Emit Rate##PartEmitter", &(leg::selPartEmitter->EmitRate), 0.01);
            ImGui::DragFloat("Emit Random##PartEmitter", &(leg::selPartEmitter->EmitRandom), 0.01);
            ImGui::DragFloat("Size##PartEmitter", &(leg::selPartEmitter->Size), 0.01);
            ImGui::DragFloat("Size Random##PartEmitter", &(leg::selPartEmitter->SizeRandom), 0.01);
            ImGui::DragFloat("Life##PartEmitter", &(leg::selPartEmitter->Life), 0.01);
            ImGui::DragFloat("Life Random##PartEmitter", &(leg::selPartEmitter->LifeRandom), 0.01);
            ImGui::DragFloat("Rotation##PartEmitter", &(leg::selPartEmitter->Rotation), 0.01);
            ImGui::DragFloat("Rotation Random##PartEmitter", &(leg::selPartEmitter->RotationRandom), 0.01);
            ImGui::PushItemWidth(width);
            ImGui::DragFloat("X##PartEmmiterVel", &(leg::selPartEmitter->Velocity[0]), 0.01);
            ImGui::SameLine();
            ImGui::DragFloat("Y##PartEmmiterVel", &(leg::selPartEmitter->Velocity[1]), 0.01);
            ImGui::SameLine();
            ImGui::Text("Velocity");
            ImGui::DragFloat("X##PartEmmiterVelRand", &(leg::selPartEmitter->VelocityRandom[0]), 0.01);
            ImGui::SameLine();
            ImGui::DragFloat("Y##PartEmmiterVelRand", &(leg::selPartEmitter->VelocityRandom[1]), 0.01);
            ImGui::SameLine();
            ImGui::Text("Random Velocity");
            ImGui::DragFloat("X##PartEmmiterSize", &(leg::selPartEmitter->EmitterSize[0]), 0.01);
            ImGui::SameLine();
            ImGui::DragFloat("Y##PartEmmiterSize", &(leg::selPartEmitter->EmitterSize[1]), 0.01);
            ImGui::SameLine();
            ImGui::Text("Emitter Size");
            ImGui::DragFloat("X##PartEmmiteroffset", &(leg::selPartEmitter->Offset[0]), 0.01);
            ImGui::SameLine();
            ImGui::DragFloat("Y##PartEmmiteroffset", &(leg::selPartEmitter->Offset[1]), 0.01);
            ImGui::SameLine();
            ImGui::Text("Offset");
            ImGui::PopItemWidth();
            ImGui::DragFloat("Rotational Velocity##PartEmitter", &(leg::selPartEmitter->RotationalVelocity), 0.01);
            ImGui::DragFloat("Rotational Vel Rand##PartEmitter", &(leg::selPartEmitter->RotationalVelocityRandom), 0.01);
            ImGui::PopItemWidth();

            if (ImGui::Button("Remove Particle Emitter"))
            {
                leg::selection->DeleteComponent(ComponentType::ParticleEmitter);
                leg::selPartEmitter = NULL;
            }
        }

        if (leg::selPartSystem && ImGui::CollapsingHeader("Particle System"))
        {
            ImGui::Checkbox("Visible##PartSystem", &(leg::selPartSystem->Visible));
            ImGui::PushItemWidth(width * 2);
            ImGui::ColorEdit4("Color##PartSystem", const_cast<float*>(leg::selPartSystem->StartingColor.GetAll()));

            if (ImGui::InputInt("Max Number of Particles##PartSystem", &leg::maxParticles, 1, 1, ImGuiInputTextFlags_EnterReturnsTrue) && leg::maxParticles > 0)
                leg::selPartSystem->MaxParticles = leg::maxParticles;

            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                ImGui::Text("Press ENTER to set");
                ImGui::EndTooltip();
            }

            ImGui::Combo("Blending Mode##partsystem", &leg::particleBlend, "None\0Alpha\0Additive\0Multiply\0\0");
            leg::selPartSystem->BlendingMode = static_cast<BlendMode>(leg::particleBlend);

            ImGui::PopItemWidth();
            ImGui::PushItemWidth(width);
            ImGui::InputFloat("X##PartSystemSize", &(leg::selPartSystem->SystemSize[0]));
            ImGui::SameLine();
            ImGui::InputFloat("Y##PartSystemSize", &(leg::selPartSystem->SystemSize[1]));
            ImGui::SameLine();
            ImGui::Text("System Size");
            ImGui::PopItemWidth();
            ImGui::Text("Current Sprite Source: %s", leg::selPartSystem->SpriteSource.c_str());
            static ImGuiTextFilter partTextures;
            partTextures.Draw("##PartSystem", 250);
            ImGui::Text("Available Sprite Sources");
            ImGui::Separator();
            ImGui::BeginChild("ParticleTextures", ImVec2(0,100));
            for (std::string s : leg::textureNames)
            {
                if (partTextures.PassFilter(s.c_str()))
                {
                    if (ImGui::MenuItem(s.c_str()))
                    {
                        leg::selPartSystem->SpriteSource = s;
                    }
                }
            }
            ImGui::EndChild();
            if (ImGui::Button("Remove Particle System"))
            {
                leg::selection->DeleteComponent(ComponentType::ParticleSystem);
                leg::selPartSystem = NULL;
            }
        }

        if (leg::selPhysics && ImGui::CollapsingHeader("Physics"))
        {
            ImGui::Checkbox("Unaffected by Time Scale##Physics", &(leg::selPhysics->UnaffectedByTimeScaling));
            ImGui::Checkbox("Unaffected by Gravity##Physics", &(leg::selPhysics->UnaffectedByGravity));
            ImGui::InputFloat("Mass##Physics", &(leg::selPhysics->Mass));
            if (ImGui::Button("Remove Physics"))
            {
                leg::selection->DeleteComponent(ComponentType::Physics);
                leg::selPhysics = NULL;
            }
        }

        if (leg::selController && ImGui::CollapsingHeader("Player Controller"))
        {
            ImGui::InputFloat("Jump Speed##Player", &(leg::selController->JumpSpeed));
            ImGui::InputFloat("Acceleration##Player", &(leg::selController->Acceleration));
            ImGui::InputFloat("Drag##Player", &(leg::selController->Drag));
            if (ImGui::TreeNode("Edit Controls##Player"))
            {
                static int selButton = 0;
                ImGui::RadioButton("Jump Key: ##player", &selButton, 0);
                ImGui::SameLine();
                ImGui::Text("%d", leg::selController->JumpKey);
                ImGui::RadioButton("Left Key: ##player", &selButton, 1);
                ImGui::SameLine();
                ImGui::Text("%d", leg::selController->LeftKey);
                ImGui::RadioButton("Right Key: ##player", &selButton, 2);
                ImGui::SameLine();
                ImGui::Text("%d", leg::selController->RightKey);

                static ImGuiTextFilter keyFilter;
                keyFilter.Draw("##player", 250);
                ImGui::Text("Available Keys");
                ImGui::BeginChild("PlayerKeys", ImVec2(0, 100), true);
                for (std::string s : leg::keyCodes)
                {
                    if (keyFilter.PassFilter(s.c_str()))
                    {
                        if (ImGui::MenuItem(s.c_str()))
                        {
                            switch (selButton)
                            {
                            case 0:
                                leg::selController->JumpKey = Keys::GetKeyFromString(s);
                                break;
                            case 1:
                                leg::selController->LeftKey = Keys::GetKeyFromString(s);
                                break;
                            case 2:
                                leg::selController->RightKey = Keys::GetKeyFromString(s);
                                break;
                            }
                        }
                    }
                }
                ImGui::EndChild();
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Edit Sound##controller"))
            {
                static int selButton = 0;
                ImGui::RadioButton("Walk Sound", &selButton, 0);
                ImGui::SameLine();
                ImGui::Text(": %s", leg::selController->WalkSound.c_str());
                ImGui::RadioButton("Jump Sound", &selButton, 1);
                ImGui::SameLine();
                ImGui::Text(": %s", leg::selController->JumpSound.c_str());
                ImGui::RadioButton("Land Sound", &selButton, 2);
                ImGui::SameLine();
                ImGui::Text(": %s", leg::selController->LandSound.c_str());

                static ImGuiTextFilter controllerSounds;
                controllerSounds.Draw("##controllersounds", 250);
                ImGui::Text("Available Sounds");
                ImGui::BeginChild("Sounds", ImVec2(0, 100), true);
                for (std::string s : leg::soundNames)
                {
                    if (controllerSounds.PassFilter(s.c_str()))
                    {
                        if (ImGui::MenuItem(s.c_str()))
                        {
                            switch (selButton)
                            {
                            case 0:
                                leg::selController->WalkSound = s;
                                break;
                            case 1:
                                leg::selController->JumpSound = s;
                                break;
                            case 2:
                                leg::selController->LandSound = s;
                                break;
                            }
                        }
                    }
                }
                ImGui::EndChild();

                if (ImGui::Button("Clear Sound##controller"))
                {
                    switch (selButton)
                    {
                    case 0:
                        leg::selController->WalkSound = "";
                        break;
                    case 1:
                        leg::selController->JumpSound = "";
                        break;
                    case 2:
                        leg::selController->LandSound = "";
                        break;
                    }
                }

                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Edit Animation##player"))
            {
                static int selButton = 0;
                ImGui::RadioButton("Walk Animation##Player", &selButton, 0);
                ImGui::SameLine();
                ImGui::Text(": %s", leg::selController->WalkAnimation.c_str());
                ImGui::RadioButton("Jump Animation##Player", &selButton, 1);
                ImGui::SameLine();
                ImGui::Text(": %s", leg::selController->JumpAnimation.c_str());
                static ImGuiTextFilter controllerAni;
                controllerAni.Draw("PlayerAnimation", 250);
                ImGui::Text("Available Animations");
                ImGui::BeginChild("PlayerAni", ImVec2(0, 100), true);
                for (std::string s : leg::animationNames)
                {
                    if (controllerAni.PassFilter(s.c_str()))
                    {
                        if(ImGui::MenuItem(s.c_str()))
                        {
                            switch (selButton)
                            {
                            case 0:
                                leg::selController->WalkAnimation = s;
                                break;
                            case 1:
                                leg::selController->JumpAnimation = s;
                                break;
                            }
                        }
                    }
                }
                ImGui::EndChild();
                if (ImGui::Button("Clear Animation##player"))
                {
                    switch (selButton)
                    {
                    case 0:
                        leg::selController->WalkAnimation = "";
                        break;
                    case 1:
                        leg::selController->JumpAnimation = "";
                        break;
                    }
                }
                ImGui::TreePop();
            }
            if (ImGui::Button("Remove Controller"))
            {
                leg::selection->DeleteComponent(ComponentType::PlayerController);
                leg::selController = NULL;
            }
        }

        if (leg::selScale && ImGui::CollapsingHeader("Scale with Keyboard"))
        {
            ImGui::PushItemWidth(width * 2);
            ImGui::Checkbox("Active##SWK", &(leg::selScale->Active));
            ImGui::InputFloat("Scale Speed##SWK", &(leg::selScale->ScaleSpeed));
            ImGui::InputFloat("Max X##SWK", &(leg::selScale->MaxXScale));
            ImGui::InputFloat("Max Y##SWK", &(leg::selScale->MaxYScale));
            ImGui::DragFloat("Scale##SWK", &(leg::selScale->Scale), 0.01, 0, 1);
            if (ImGui::TreeNode("Edit Controls##swk"))
            {
                static int selButton = 0;
                ImGui::RadioButton("Scale Down", &selButton, 0);
                ImGui::SameLine();
                ImGui::Text(": %d", leg::selScale->ScaleDownKey);
                ImGui::RadioButton("Scale Up", &selButton, 1);
                ImGui::SameLine();
                ImGui::Text(": %d", leg::selScale->ScaleUpKey);
                static ImGuiTextFilter keyFilter;
                keyFilter.Draw("##swk", 250);
                ImGui::Text("Available Keys");
                ImGui::BeginChild("swkkeys", ImVec2(0, 100), true);
                for (std::string s : leg::keyCodes)
                {
                    if (keyFilter.PassFilter(s.c_str()))
                    {
                        if (ImGui::MenuItem(s.c_str()))
                        {
                            switch (selButton)
                            {
                            case 0:
                                leg::selScale->ScaleDownKey = Keys::GetKeyFromString(s);
                                break;
                            case 1:
                                leg::selScale->ScaleUpKey = Keys::GetKeyFromString(s);
                                break;
                            }
                        }
                    }
                }
                ImGui::EndChild();
                ImGui::TreePop();
            }
            ImGui::PopItemWidth();
            ImGui::Text("Scale Sound: %s", leg::selScale->ScaleSound.c_str());
            static ImGuiTextFilter scaleSound;
            scaleSound.Draw("##swksound", 250);
            ImGui::Text("Available Sounds");
            ImGui::BeginChild("Sound", ImVec2(0, 100), true);
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

            if (ImGui::Button("Clear Sound##swk"))
                leg::selScale->ScaleSound= "";

            if (ImGui::Button("Remove Scale with Keyboard"))
            {
                leg::selection->DeleteComponent(ComponentType::ScaleWithKeyboard);
                leg::selScale = NULL;
            }
        }

        if (leg::selSound && ImGui::CollapsingHeader("Sound Emitter"))
        {
            if (ImGui::Button("Remove Sound Emitter"))
            {
                leg::selection->DeleteComponent(ComponentType::SoundEmitter);
                leg::selSound = NULL;
            }
        }

        if (leg::selSprite && ImGui::CollapsingHeader("Sprite"))
        {
            ImGui::Checkbox("Visible##sprite", &(leg::selSprite->Visible));
            ImGui::Checkbox("Flip X##sprite", &(leg::selSprite->FlipX));
            ImGui::SameLine();
            ImGui::Checkbox("Flip Y##sprite", &(leg::selSprite->FlipY));
            ImGui::PushItemWidth(250);
            ImGui::ColorEdit4("Color##sprite", const_cast<float*>(leg::selSprite->SpriteColor.GetAll()));
            ImGui::Text("Current Sprite Source: %s", leg::selSprite->GetSourceName().c_str());
            static ImGuiTextFilter textures;
            textures.Draw("##Sprite", 300);
            ImGui::Text("Available Sprite Sources");
            ImGui::Separator();
            ImGui::BeginChild("SpriteTextures", ImVec2(0,100));
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

            if (ImGui::Button("Remove Sprite"))
            {
                leg::selection->DeleteComponent(ComponentType::Sprite);
                leg::selSprite = NULL;
            }
        }

        if (leg::selSprtxt && ImGui::CollapsingHeader("Sprite Text"))
        {
            if (ImGui::InputTextMultiline("Text##sprtxt", leg::spriteTextBuf, 512, ImVec2(), ImGuiInputTextFlags_EnterReturnsTrue))
                leg::selSprtxt->Text = leg::spriteTextBuf;

            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                ImGui::Text("Press CTRL + ENTER to set");
                ImGui::EndTooltip();
            }
            ImGui::PushItemWidth(width * 2);
            ImGui::ColorEdit4("Color##sprtxt", const_cast<float*>(leg::selSprtxt->TextColor.GetAll()));
            ImGui::PopItemWidth();

            if (ImGui::Button("Remove Sprite Text"))
            {
                leg::selection->DeleteComponent(ComponentType::SpriteText);
                leg::selSprtxt = NULL;
            }
        }

        if (leg::selTMC && ImGui::CollapsingHeader("Transform Control"))
        {
            ImGui::PushItemWidth(width * 2);
            ImGui::InputFloat("Normal Speed##TMC", &(leg::selTMC->NormalSpeed));
            ImGui::InputFloat("Slow Motion Speed##TMC", &(leg::selTMC->SlowMotionSpeed));
            ImGui::InputFloat("Influence Radius##TMC", &(leg::selTMC->InfluenceRadius));
            ImGui::PopItemWidth();
            if (ImGui::TreeNode("Edit Controls##tmc"))
            {
                ImGui::Text("Toggle Key: %d", leg::selTMC->ModeToggleKey);
                static ImGuiTextFilter keyFilter;
                keyFilter.Draw("##tmckey", 250);
                ImGui::Text("Available Keys");
                ImGui::BeginChild("tmckeys", ImVec2(0, 100), true);
                for (std::string s : leg::keyCodes)
                {
                    if (keyFilter.PassFilter(s.c_str()))
                    {
                        if (ImGui::MenuItem(s.c_str()))
                        {
                            leg::selTMC->ModeToggleKey = Keys::GetKeyFromString(s);
                        }
                    }
                }
                ImGui::EndChild();
                ImGui::TreePop();
            }
            ImGui::Text("Sound: %s", leg::selTMC->TransformModeSound.c_str());
            static ImGuiTextFilter tmcSound;
            tmcSound.Draw("##tmcsound", 250);
            ImGui::Text("Available Sounds");
            ImGui::BeginChild("Sound", ImVec2(0, 100), true);
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

            if (ImGui::Button("Clear Sound##tmc"))
                leg::selTMC->TransformModeSound = "";

            if (ImGui::Button("Remove Transform Control"))
            {
                leg::selection->DeleteComponent(ComponentType::TransformModeControls);
                leg::selTMC = NULL;
            }
        }

        if (leg::selVision && ImGui::CollapsingHeader("Vision Cone"))
        {
            ImGui::Checkbox("Active##VisionCone", &(leg::selVision->Active));
            ImGui::SameLine();
            ImGui::Checkbox("Draw Outline##VisionCone", &(leg::selVision->DrawOutline));
            ImGui::SameLine();
            ImGui::Checkbox("Visible##VisionCone", &(leg::selVision->Visible));
            ImGui::SliderAngle("Angle##VisionCone", &(leg::selVision->Angle), 0, 180);
            ImGui::SliderAngle("Direction##VisionCone", &leg::visionAngle, 0, 360);
            leg::selVision->Direction[0] = cos(leg::visionAngle);
            leg::selVision->Direction[1] = sin(leg::visionAngle);
            ImGui::PushItemWidth(width);
            ImGui::DragFloat("X##visionoffset", &(leg::selVision->Offset[0]), 0.01);
            ImGui::SameLine();
            ImGui::DragFloat("Y##visionoffset", &(leg::selVision->Offset[1]), 0.01);
            ImGui::Text("Offset");
            ImGui::PopItemWidth();
            ImGui::DragFloat("Radius##VisionCone", &(leg::selVision->Radius), 0.01);
            ImGui::PushItemWidth(200);
            ImGui::ColorEdit4("Indicator Color##VisionCone", const_cast<float*>(leg::selVision->IndicatorColor.GetAll()));
            ImGui::PopItemWidth();

            if (ImGui::Button("Remove Vision Cone"))
            {
                leg::selection->DeleteComponent(ComponentType::VisionCone);
                leg::selVision = NULL;
            }
        }

        ImGui::Separator();
        if (ImGui::Button("Set Checkpoint Trigger"))
            leg::levelCheckpoint->TriggerEntityID = leg::selection->GetID();
        if (ImGui::Button("Delete Object"))
            leg::delobj = true;

        ImGui::End();
    }
}
