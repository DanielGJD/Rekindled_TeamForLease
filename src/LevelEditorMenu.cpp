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
#include <unordered_map>
namespace ForLeaseEngine
{
    namespace leg = LevelEditorGlobals;

    void LevelEditor::DrawMainMenu()
    {
        ImGui::BeginMainMenuBar();
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::Button("Save"))
            {
                ImGui::OpenPopup("Save");
            }

            if (ImGui::BeginPopupModal("Save", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::InputText("File Name", leg::statefile, 128);
                ImGui::Text("Warning: File will get overwritten if it already exists");
                if (ImGui::Button("Save"))
                {
                    SaveLevel(leg::statefile);
                    ImGui::CloseCurrentPopup();
                }
                ImGui::SameLine();
                if (ImGui::Button("Cancel"))
                {
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }

            if (ImGui::Button("New Level"))
            {
                ImGui::OpenPopup("new level");
            }

            if (ImGui::BeginPopupModal("new level", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text("Warning: All unsaved progress will be lost");
                if (ImGui::Button("Continue"))
                {
                    DeleteAllLevelComponents();
                    DeleteAllEntities();
                    leg::selection = NULL;
                    Load();
                    ImGui::CloseCurrentPopup();
                    leg::levelSaved = false;
                }
                ImGui::SameLine();
                if (ImGui::Button("Cancel"))
                {
                    ImGui::CloseCurrentPopup();
                }
                ImGui::EndPopup();
            }

            if (ImGui::Button("Test Level"))
            {
                ImGui::OpenPopup("Test Level");
            }

            if (ImGui::BeginMenu("Load Level"))
            {
                if (ImGui::InputText("File Name", leg::statefile, 128, ImGuiInputTextFlags_EnterReturnsTrue))
                {
                    ImGui::OpenPopup("Load");
                }

                if (ImGui::BeginPopupModal("Load", NULL, ImGuiWindowFlags_AlwaysAutoResize))
                {
                    ImGui::Text("Any unsaved progress will be lost!");
                    if (ImGui::Button("Continue"))
                    {
                        LoadLevel(leg::statefile);
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

            if (ImGui::BeginPopupModal("Test Level", NULL, ImGuiWindowFlags_AlwaysAutoResize))
            {
                static bool debug = false;
                if (!leg::levelSaved)
                {
                    ImGui::Text("This level will be saved with this filename");
                    ImGui::InputText("Filename", leg::statefile, 128);
                }

                ImGui::Text("Warning: %s will be overwritten!", leg::statefile);
                ImGui::Checkbox("Draw Collision Boxes", &debug);
                if (ImGui::Button("Launch"))
                {
                    SaveLevel(leg::statefile);
                    std::string file = leg::levelDir + leg::statefile;
                    SpawnNewLevelProcess(file, debug);
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

        ImGui::Checkbox("Translate/Select", &leg::selMode);
        ImGui::SameLine();
        ImGui::Checkbox("Move Camera", &leg::moveMode);
        ImGui::SameLine();
        ImGui::Checkbox("Place Objects", &leg::clickAdd);
        ImGui::SameLine();
        if (ImGui::Button("Reload Assets"))
            LoadFiles();
        ImGui::EndMainMenuBar();
    }

    void LevelEditor::DrawMainWindow()
    {
        ImGui::Begin("Level Editor");

        if (ImGui::InputText("Level Name", leg::statename, 128, ImGuiInputTextFlags_EnterReturnsTrue))
            SetName(leg::statename);

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
            ImGui::InputFloat("x##levelcam", &(leg::camTrans->Position[0]), 0, 0, 3);
            ImGui::SameLine();
            ImGui::InputFloat("y##levelcam", &(leg::camTrans->Position[1]), 0, 0, 3);
            ImGui::InputFloat("Size##levelcam", &(leg::camCamera->Size), 0, 0, 0);
            ImGui::PopItemWidth();
        }

        if (ImGui::CollapsingHeader("Physics"))
        {
            ImGui::Text("Gravity");
            ImGui::PushItemWidth(80);
            ImGui::InputFloat("x##levelphy", &(leg::gravity[0]), 0, 0, 3);
            ImGui::SameLine();
            ImGui::InputFloat("y##levelphy", &(leg::gravity[1]), 0, 0, 3);
            ImGui::SameLine();
            if (ImGui::Button("Set##levelphy"))
                leg::levelPhysics->SetGravity(leg::gravity);

            ImGui::PopItemWidth();
        }

        if (leg::levelLight && ImGui::CollapsingHeader("Light"))
        {
            ImGui::ColorEdit4("Ambient Light", const_cast<float*>(leg::levelLight->AmbientLight.GetAll()));
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
                        SpawnBluePrint(leg::archetypeNames[i]);
                    }
                }
            }
            ImGui::EndChild();
        }

        if (ImGui::CollapsingHeader("Object List"))
        {
            std::vector<std::string> entityNames;
            for (Entity* e : Entities)
                entityNames.push_back(e->GetName());

            static ImGuiTextFilter objectFilter;
            objectFilter.Draw("Name##objects", 200);
            ImGui::BeginChild("Objects##objects", ImVec2(0, 150), true);
            for (std::string s : entityNames)
            {
                if (objectFilter.PassFilter(s.c_str()))
                {
                    if (ImGui::MenuItem(s.c_str()))
                    {
                        leg::selection = GetEntityByName(s);
                        if (leg::selection)
                        {
                            MakeSelection();
                            leg::camTrans->Position[0] = leg::selTran->Position[0];
                            leg::camTrans->Position[1] = leg::selTran->Position[1];
                        }
                    }
                }
            }

            entityNames.clear();
            ImGui::EndChild();
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
        if (!(component.compare("Checkpoint")) && !leg::selCheckpoint)
        {
            leg::selCheckpoint = Components::Checkpoint::Create(*leg::selection);
            if (!leg::selection->AddComponent(leg::selCheckpoint))
                leg::selCheckpoint = NULL;
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
        if (!(component.compare("Follow")) && !leg::selFollow)
        {
            leg::selFollow = new Components::Follow(*leg::selection);
            if (!leg::selection->AddComponent(leg::selFollow))
                leg::selFollow = NULL;
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
        if (!(component.compare("Occluder")) && !leg::selOccluder)
        {
            leg::selOccluder = new Components::Occluder(*leg::selection);
            if (!leg::selection->AddComponent(leg::selOccluder))
                leg::selOccluder = NULL;
            return;
        }
        if (!(component.compare("Parallax")) && !leg::selParallax)
        {
            leg::selParallax = new Components::Parallax(*leg::selection);
            if (!leg::selection->AddComponent(leg::selParallax))
                leg::selParallax = NULL;
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

    std::string LevelEditor::RequiredToolTip(std::string component)
    {
        std::string tooltip = "Requires: ";
        int counter = 0;
        ComponentType reqMask = leg::reqMap[component];

        if (static_cast<unsigned long long>(reqMask & ComponentType::Physics) != 0)
        {
            tooltip += "Physics";
            ++counter;
        }
        if (static_cast<unsigned long long>(reqMask & ComponentType::Collision) != 0)
        {
            if (counter)
                tooltip += ", ";
            tooltip += "Collision";
            ++counter;
        }
        if (static_cast<unsigned long long>(reqMask & ComponentType::Sprite) != 0)
        {
            if (counter)
                tooltip += ", ";
            tooltip += "Sprite";
            ++counter;
        }
        if (static_cast<unsigned long long>(reqMask & ComponentType::SoundEmitter) != 0)
        {
            if (counter)
                tooltip += ", ";
            tooltip += "Sound";
            ++counter;
        }
        if (static_cast<unsigned long long>(reqMask & ComponentType::ParticleSystem) != 0)
        {
            if (counter)
                tooltip += ", ";
            tooltip += "Particle System";
            ++counter;
        }

        if (!counter)
            tooltip += "None";

        return tooltip;
    }


}
