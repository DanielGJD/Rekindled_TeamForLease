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
namespace ForLeaseEngine
{
    namespace LevelEditorGlobals
    {
        SDL_Window* window;
        Point       mousePos;

        LevelComponents::Renderer* render;
        LevelComponents::Physics*  levelPhysics;
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

        bool clickAdd   = false;
        bool newSelect  = false;
        bool moveMode   = false;
        bool showWindow = false;
        bool selMade    = false;
        bool archSpawn  = false;
        bool selMode    = true;
        bool toSave     = false;
        bool toLoad     = false;

        char entName[70];
        char spriteTextBuf[500];
        char statefile[70];
        char statename[70];
        char meshfile[70];
        char archetypefile[70];
        char soundfile[70];
        char fontfile[70];

        float spriteTextColor[4];
    }

    namespace leg = LevelEditorGlobals;

    void LevelEditor::Load()
    {
        leg::render = new LevelComponents::Renderer(*this);
        leg::levelPhysics = new LevelComponents::Physics(*this);
        AddLevelComponent(leg::render);
        AddLevelComponent(leg::levelPhysics);
        leg::gravity = leg::levelPhysics->GetGravity();
        leg::camera = AddEntity("Level Camera");
        leg::camTrans = new Components::Transform(*leg::camera);
        leg::camCamera = new Components::Camera(*leg::camera, 0, 1, 50);
        leg::camera->AddComponent(leg::camTrans);
        leg::camera->AddComponent(leg::camCamera);
        leg::render->SetCamera(*leg::camera);
        leg::window = ForLease->GameWindow->DangerousGetRawWindow();
        strcpy(leg::statename, Name.c_str());
        ForLease->Resources.LoadMesh("GroundMesh.json");
        leg::fontNames = ForLease->Resources.GetLoadedFontNames();
        leg::meshNames = ForLease->Resources.GetLoadedMeshNames();
        //leg::soundNames = ForLease->Resources.GetLodedSoundNames();
        leg::animationNames = ForLease->Resources.GetLoadedMeshAnimationNames();
        leg::textureNames = ForLease->Resources.GetLoadedTextureNames();
        leg::componentNames.push_back("Camera");
        leg::componentNames.push_back("Collision");
        leg::componentNames.push_back("Drag with Mouse");
        leg::componentNames.push_back("Menu");
        leg::componentNames.push_back("Physics");
        leg::componentNames.push_back("Player Controller");
        leg::componentNames.push_back("Scale with Keyboard");
        leg::componentNames.push_back("Sound");
        leg::componentNames.push_back("Sprite");
        leg::componentNames.push_back("Sprite Text");
        leg::componentNames.push_back("Transform Control");
        leg::componentNames.push_back("Vision Cone");
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

                if (ImGui::BeginMenu("Sound"))
                {
                    if (ImGui::InputText("File Name", leg::soundfile, 70, ImGuiInputTextFlags_EnterReturnsTrue))
                    {
                        ForLease->Resources.LoadSound(leg::soundfile);
                        //leg::soundNames = ForLease->Resources.GetLodedSoundNames();
                    }

                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Font"))
                {
                    if (ImGui::InputText("File Name", leg::fontfile, 70, ImGuiInputTextFlags_EnterReturnsTrue))
                    {
                        std::cout << leg::fontfile << std::endl;
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
            std::cout << leg::statename << std::endl;

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

        ImGui::End();
    }

    static void AddComponent(const std::string& component)
    {
        if (!(component.compare("Camera")) && !leg::selCamera)
        {
            leg::selCamera = new Components::Camera(*leg::selection, 0, 1, 50);
            leg::selection->AddComponent(leg::selCamera);
            return;
        }
        if (!(component.compare("Collision")) && !leg::selCollision)
        {
            leg::selCollision = new Components::Collision(*leg::selection);
            leg::selection->AddComponent(leg::selCollision);
            return;
        }
        if (!(component.compare("Drag with Mouse")) && !leg::selDrag)
        {
            leg::selDrag = leg::selDrag->Create(*leg::selection);
            leg::selection->AddComponent(leg::selDrag);
            return;
        }
        if (!(component.compare("Menu")) && !leg::selMenu)
        {
            leg::selMenu = new Components::Menu(*leg::selection);
            leg::selection->AddComponent(leg::selMenu);
            return;
        }
        if (!(component.compare("Physics")) && !leg::selPhysics)
        {
            leg::selPhysics = new Components::Physics(*leg::selection);
            leg::selection->AddComponent(leg::selPhysics);
            return;
        }
        if (!(component.compare("Player Controller")) && !leg::selController)
        {
            leg::selController = leg::selController->Create(*leg::selection);
            leg::selection->AddComponent(leg::selController);
            return;
        }
        if (!(component.compare("Scale with Keyboard")) && !leg::selScale)
        {
            leg::selScale = leg::selScale->Create(*leg::selection);
            leg::selection->AddComponent(leg::selScale);
            return;
        }
        if (!(component.compare("Sound")) && !leg::selSound)
        {
            leg::selSound = new Components::SoundEmitter(*leg::selection);
            leg::selection->AddComponent(leg::selSound);
            return;
        }
        if (!(component.compare("Sprite")) && !leg::selSprite)
        {
            leg::selSprite = new Components::Sprite(*leg::selection);
            leg::selection->AddComponent(leg::selSprite);
            return;
        }
        if (!(component.compare("Sprite Text")) && !leg::selSprtxt)
        {
            leg::selSprtxt = new Components::SpriteText(*leg::selection, "Arial.fnt");
            leg::selection->AddComponent(leg::selSprtxt);
            return;
        }
        if (!(component.compare("Transform Control")) && !leg::selTMC)
        {
            leg::selTMC = new Components::TransformModeControls(*leg::selection);
            leg::selection->AddComponent(leg::selTMC);
            return;
        }
        if (!(component.compare("Vision Cone")) && !leg::selVision)
        {
            leg::selVision = new Components::VisionCone(*leg::selection);
            leg::selection->AddComponent(leg::selVision);
        }
    }

    static void DrawObjectWindow()
    {
        leg::render->DrawRectangle(leg::selTran->Position, leg::selTran->ScaleX, leg::selTran->ScaleY, leg::selTran->Rotation);
        ImGui::Begin("Object Editor");
        ImGui::PushItemWidth(80);

        if (ImGui::InputText("Name", leg::entName, 70, ImGuiInputTextFlags_EnterReturnsTrue))
            leg::selection->SetName(leg::entName);

        ImGui::InputText("File Name", leg::archetypefile, 70);
        ImGui::SameLine();
        if (ImGui::Button("Save Archetype"))
            leg::selection->CreateArchetype(leg::archetypefile);

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
            ImGui::PushItemWidth(100);
            ImGui::InputFloat("x scale", &(leg::selTran->ScaleX), 0.5, 1);
            ImGui::SameLine();
            ImGui::InputFloat("y scale", &(leg::selTran->ScaleY), 0.5, 1);
            ImGui::InputInt("Z Order", &(leg::selTran->ZOrder));
            ImGui::PopItemWidth();
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
            }
            ImGui::Unindent();
        }

        if (leg::selCamera && ImGui::CollapsingHeader("Camera"))
        {
            ImGui::InputFloat("Size", &(leg::selCamera->Size));
                if (ImGui::Button("Set Camera"))
                    leg::render->SetCamera(*leg::selection);
        }

        if (leg::selCollision && ImGui::CollapsingHeader("Collision"))
        {
                ImGui::PushItemWidth(100);
                ImGui::InputFloat("Width", &(leg::selCollision->Width));
                ImGui::SameLine();
                ImGui::InputFloat("Height", &(leg::selCollision->Height));
                ImGui::PopItemWidth();
                leg::render->DrawRectangle(leg::selTran->Position, leg::selCollision->Width, leg::selCollision->Height, leg::selTran->Rotation);
                ImGui::Checkbox("Resolve Collision", &(leg::selCollision->ResolveCollisions));
        }

        if (leg::selDrag && ImGui::CollapsingHeader("Drag with Mouse"))
        {
            ImGui::Checkbox("Active", &(leg::selDrag->Active));
        }

        if (leg::selMenu && ImGui::CollapsingHeader("Menu"))
        {
            ImGui::InputFloat("Focused Scale", &(leg::selMenu->FocusedScale));
            ImGui::SameLine();
            ImGui::InputFloat("Unfocused Scale", &(leg::selMenu->UnfocusedScale));
            ImGui::InputFloat("Spacing X", &(leg::selMenu->Spacing[0]));
            ImGui::SameLine();
            ImGui::InputFloat("Spacing Y", &(leg::selMenu->Spacing[1]));
        }

        if (leg::selPhysics && ImGui::CollapsingHeader("Physics"))
        {
            ImGui::InputFloat("Mass", &(leg::selPhysics->Mass));
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
        }

        if (leg::selSprite && ImGui::CollapsingHeader("Sprite"))
        {

        }

        if (leg::selSprtxt && ImGui::CollapsingHeader("Sprite Text"))
        {
            ImGui::InputText("Text", leg::spriteTextBuf, 500);
            leg::selSprtxt->Text = leg::spriteTextBuf;
            ImGui::PushItemWidth(300);
            ImGui::ColorEdit4("Color", leg::spriteTextColor);
            leg::selSprtxt->TextColor.SetAll(leg::spriteTextColor[0],
                                             leg::spriteTextColor[1],
                                             leg::spriteTextColor[2],
                                             leg::spriteTextColor[3]);
            ImGui::PopItemWidth();
        }

        if (leg::selTMC && ImGui::CollapsingHeader("Transform Control"))
        {

        }

        if (leg::selVision && ImGui::CollapsingHeader("Vision Cone"))
        {

        }

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
            ent->AddComponent(new Components::Model(*ent, true, false, false, "GroundMesh.json"));
        }

        if (leg::selMode && ImGui::IsMouseClicked(0) && !ImGui::IsMouseHoveringAnyWindow())
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
                    if (leg::selSprtxt)
                    {
                        strcpy(leg::spriteTextBuf, leg::selSprtxt->Text.c_str());
                        leg::spriteTextColor[0] =  leg::selSprtxt->TextColor.GetR();
                        leg::spriteTextColor[1] =  leg::selSprtxt->TextColor.GetG();
                        leg::spriteTextColor[2] =  leg::selSprtxt->TextColor.GetB();
                        leg::spriteTextColor[3] =  leg::selSprtxt->TextColor.GetA();
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
                DeleteAllEntities();
                Deserialize(root);
                leg::camera = GetEntityByName("Level Camera", true);
                leg::camTrans = leg::camera->GetComponent<Components::Transform>();
                leg::camCamera = leg::camera->GetComponent<Components::Camera>();
                leg::selection = NULL;
                leg::toLoad = false;
            }
        }

        Input();
        leg::render->Update(Entities);
        ImGui::Render();
        ForLease->GameWindow->UpdateGameWindow();
    }

    void LevelEditor::Deinitialize()
    {
        ImGui_ImplSdl_Shutdown();
    }

    void LevelEditor::Unload()
    {
        Serializer root;
        SaveFiles(root);
        //root.WriteFile("LevelEditorData.json");
    }

    void LevelEditor::SaveFiles(Serializer& root)
    {

    }

    void LevelEditor::LoadFiles(Serializer& root)
    {

    }
}

