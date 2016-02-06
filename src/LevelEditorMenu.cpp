#include "LevelEditor.h"
#include "LevelEditorGlobals.h"
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "Utilities.h"
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

    void LevelEditor::DrawMainMenu()
    {
        ImGui::BeginMainMenuBar();
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::BeginMenu("Save"))
            {
                if (ImGui::InputText("File Name", leg::statefile, 70, ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    ImGui::OpenPopup("Are you sure");
                }

                if (ImGui::BeginPopupModal("Are you sure", NULL, ImGuiWindowFlags_AlwaysAutoResize))
                {
                    ImGui::Text("Are you sure you want to overwrite %s?\n", leg::statefile);
                    if (ImGui::Button("Yes"))
                    {
                        leg::toSave = true;
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Cancel"))
                    {
                        ImGui::CloseCurrentPopup();
                    }
                    ImGui::EndPopup();
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

//                if (ImGui::BeginMenu("Sound"))
//                {
//                    if (ImGui::InputText("File Name", leg::soundfile, 70, ImGuiInputTextFlags_EnterReturnsTrue))
//                    {
//                        ForLease->Resources.LoadSound(leg::soundfile);
//                        leg::soundNames = ForLease->Resources.GetLoadedSoundNames();
//                    }
//
//                    ImGui::EndMenu();
//                }

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

    void LevelEditor::DrawMainWindow()
    {
        ImGui::Begin("Level Editor");

        if (ImGui::Button("Test Level"))
        {
            ImGui::OpenPopup("Test Level");
        }

        if (ImGui::BeginPopupModal("Test Level", NULL, ImGuiWindowFlags_AlwaysAutoResize))
        {
            static char filename[64];
            ImGui::Text("Input the name of the level file you wish to test.\nThe file will be saved/overwritten.");
            ImGui::InputText("File Name", filename, 64);
            if (ImGui::Button("Launch"))
            {
                SpawnNewLevelProcess(filename);
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
        if (ImGui::InputText("Level Name", leg::statename, 70, ImGuiInputTextFlags_EnterReturnsTrue))
            leg::setName = true;

        if (leg::levelLight)
        {
            if (ImGui::Button("Remove Light Component"))
            {
                DeleteLevelComponent(leg::levelLight);
                leg::levelLight = 0;
            }
        }
        else
        {
            if (ImGui::Button("Add Light Component"))
            {
                leg::levelLight = new LevelComponents::Light(*this);
                AddLevelComponent(leg::levelLight);
            }
        }

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

    void LevelEditor::AddComponent(const std::string& component)
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
        if (!(component.compare("Particle Color")) && !leg::selPartColor)
        {
            leg::selPartColor = new Components::ParticleColorAnimator(*leg::selection);
            if (!leg::selection->AddComponent(leg::selPartColor))
                leg::selPartColor = NULL;
            return;
        }
        if (!(component.compare("Particle Dynamics")) && !leg::selPartDynamics)
        {
            leg::selPartDynamics = new Components::SimpleParticleDynamics(*leg::selection);
            if (!leg::selection->AddComponent(leg::selPartDynamics))
                leg::selPartDynamics = NULL;
            return;
        }
        if (!(component.compare("Particle Emitter")) && !leg::selPartEmitter)
        {
            leg::selPartEmitter = new Components::ParticleEmitter(*leg::selection);
            if (!leg::selection->AddComponent(leg::selPartEmitter))
                leg::selPartEmitter = NULL;
            return;
        }
        if (!(component.compare("Particle System")) && !leg::selPartSystem)
        {
            leg::selPartSystem = new Components::ParticleSystem(*leg::selection);
            if (!leg::selection->AddComponent(leg::selPartSystem))
                leg::selPartSystem = NULL;
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
//        if (!(component.compare("Sound")) && !leg::selSound)
//        {
//            leg::selSound = new Components::SoundEmitter(*leg::selection);
//            if (!leg::selection->AddComponent(leg::selSound))
//                leg::selSound = NULL;
//            return;
//        }
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


}
