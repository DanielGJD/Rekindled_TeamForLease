/*!
    \author Sam Montanari

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "TestState.h"
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

using namespace ForLeaseEngine;

Point mousePos;
bool clickAdd = false;
bool newSelect = false;
bool moveMode = false;


LevelComponents::Renderer* render;
Mesh* testMesh;
Entity* camera;
SDL_Window* window;
Entity *selection;
Components::Transform* camTrans;
Components::Camera* camCamera;
std::vector<std::string> meshNames;
std::vector<std::string> componentNames;
void TestState::Load()
{
    render = new LevelComponents::Renderer(*this);

    camera = AddEntity();
    camTrans = new Components::Transform(*camera, 0, 0, 1, 1, 0);
    camera->AddComponent(camTrans);
    camCamera = new Components::Camera(*camera, 0, 1, 50);
    camera->AddComponent(camCamera);
    camera->SetName("Level Camera");

    render->SetCamera(*camera);
    window = ForLease->GameWindow->DangerousGetRawWindow();
    ForLease->Resources.LoadMesh("MeshTest.json");
    ForLease->Resources.LoadMesh("MainCharMesh.json");
    ForLease->Resources.LoadMesh("Building1Mesh.json");
    ForLease->Resources.LoadMesh("GroundMesh.json");
    ForLease->Resources.LoadMesh("Platform1Mesh.json");
    ForLease->Resources.LoadMesh("Platform2Mesh.json");
    ForLease->Resources.LoadFont("Arial.fnt");
    //ForLease->Resources.LoadSound("harpgraceful1.wav");
    meshNames = ForLease->Resources.GetLoadedMeshNames();

    componentNames.push_back("Player Controller");
    componentNames.push_back("Physics");
    componentNames.push_back("Collision");
    //componentNames.push_back("Sprite");
    componentNames.push_back("Camera");
    componentNames.push_back("Sprite Text");
    //componentNames.push_back("Light");
    componentNames.push_back("Sound Emitter");
}

void TestState::Initialize()
{
    ImGui_ImplSdl_Init(window);
}

Components::Transform* selTran;
Components::Model* selModel;
Components::Camera* selCamera;
Components::Light* selLight;
Components::Collision* selCollision;
Components::Physics* selPhysics;
Components::SoundEmitter* selSound;
Components::Sprite* selSprite;
Components::SpriteText* selSprtxt;
Components::CharacterController* selController;
static void AddComponent(const std::string& component)
{
    if (!(component.compare("Physics")) && !(selPhysics))
    {
        selPhysics = new Components::Physics(*selection);
        selection->AddComponent(selPhysics);
        return;
    }

    if (!(component.compare("Player Controller")) && !(selController))
    {
        selController = Components::CharacterController::Create(*selection);
        selection->AddComponent(selController);
        return;
    }

    if (!(component.compare("Collision"))&& !(selCollision))
    {
        selCollision = new Components::Collision(*selection);
        selection->AddComponent(selCollision);
        return;
    }

    if (!(component.compare("Sprite")) && !(selSprite))
    {
        selSprite = new Components::Sprite(*selection);
        selection->AddComponent(selSprite);
        return;
    }

    if (!(component.compare("Sprite Text")) && !(selSprtxt))
    {
        selSprtxt = new Components::SpriteText(*selection, "Arial.fnt");
        selection->AddComponent(selSprtxt);
        return;
    }

    if (!(component.compare("Light")) && !(selLight))
    {
        selLight = new Components::Light(*selection);
        selection->AddComponent(selLight);
        return;
    }

    if (!(component.compare("Sound Emitter")) && !(selSound))
    {
        selSound = new Components::SoundEmitter(*selection);
        selection->AddComponent(selSound);
        return;
    }

    if (!(component.compare("Camera")) && !(selCamera))
    {
        selCamera = new Components::Camera(*selection, 0, 1, 720);
        selection->AddComponent(selCamera);
    }

}


void ImGuiToEngine(Point& v)
{
    ImVec2 screen = ImGui::GetMousePos();
    v[0] = screen.x;
    v[1] = ForLease->GameWindow->GetYResolution() - screen.y;
    v = render->ScreenToWorld(v);
}

bool none = true;
bool showWindow = false;
bool selMade = false;
bool archSpawn = false;
bool selMode = false;
char entName[100];
char spriteTextBuf[500];
char statefile[70];
char meshfile[70];
char archetypefile[70];
float spriteTextColor[4];
void TestState::Update()
{
    ForLease->OSInput.ProcessAllInputWithImgui();

    ImGui_ImplSdl_NewFrame(window);
    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu("Save"))
    {
        ImGui::InputText("Filename", statefile, 70);
        ImGui::SameLine();
        if (ImGui::Button("Go"))
        {
            Serializer root;
            Serialize(root);
            root.WriteFile(statefile);
        }
        ImGui::EndMenu();
    }
    ImGui::PushItemWidth(100);
    if (ImGui::BeginMenu("Load"))
    {
        if (ImGui::BeginMenu("State"))
        {
            ImGui::InputText("Filename", statefile, 70);
            ImGui::SameLine();
            if(ImGui::Button("Go"))
            {
                Serializer root;
                if (root.ReadFile(statefile))
                {
                    DeleteAllEntities();
                    Deserialize(root);
                    camera = GetEntityByName("Level Camera", true);
                    camTrans = camera->GetComponent<Components::Transform>();
                    camCamera = camera->GetComponent<Components::Camera>();
                }
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Mesh"))
        {
            ImGui::InputText("Filename", meshfile, 70);
            ImGui::SameLine();
            if(ImGui::Button("Go"))
            {
                ForLease->Resources.LoadMesh(meshfile);
                meshNames = ForLease->Resources.GetLoadedMeshNames();
            }
            ImGui::EndMenu();
        }


        ImGui::EndMenu();
    }
    ImGui::PushItemWidth(100);
    if (ImGui::Button("Help"))
    {
        ImGui::OpenPopup("Help");
    }
    if (ImGui::BeginPopupModal("Help", NULL, ImGuiWindowFlags_AlwaysAutoResize))
    {
        ImGui::Text("Controls");
        ImGui::Separator();
        ImGui::Text("Left Click to select objects and drag them around.\nRight Click to place objects and/or move the camera.\nCTRL + C will select the level camera.\n");
        if (ImGui::Button("Done"))
        {
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup();
    }

    ImGui::EndMainMenuBar();
    ImGui::Begin("Editor");
    ImGui::Checkbox("Place Objects", &clickAdd);
    ImGui::Checkbox("Select Objects", &selMode);
    ImGui::Checkbox("Move Camera", &moveMode);
    ImGui::Checkbox("Spawn Archetype", &archSpawn);


    if (archSpawn)
    {
        ImGui::PushItemWidth(125);
        ImGui::InputText("Filename", archetypefile, 70);
        ImGui::PopItemWidth();
        ImGui::SameLine();
        if (ImGui::Button("Spawn"))
            selection = SpawnArchetype(archetypefile);

    }

    ImGui::End();
    if(clickAdd && ImGui::IsMouseClicked(1))
    {
        ImGuiToEngine(mousePos);
        Entity* entity = AddEntity();
        entity->AddComponent(new Components::Transform(*entity, mousePos[0], mousePos[1], 1, 1, 0));
        Components::Model* model = new Components::Model(*entity, true, false, false, "Platform1Mesh.json", "");
        entity->AddComponent(model);
    }

    if (ImGui::IsKeyPressed(Keys::Escape))
        selection = NULL;

    if (selection)
    {
        showWindow = true;
        ImGui::Begin("ObjectEditor", &showWindow);
        ImGui::PushItemWidth(100);
        ImGui::InputText("Name", entName, 100);
        if (selection != camera)
        {
            ImGui::SameLine();
            if (ImGui::Button("Set Name"))
                selection->SetName(entName);
        }
        ImGui::InputText("Filename", archetypefile, 70);
        ImGui::SameLine();
        if (ImGui::Button("Save Archetype"))
        {
            selection->CreateArchetype(archetypefile);
        }
        ImGui::PopItemWidth();

        if (ImGui::CollapsingHeader("Add Components"))
        {
            static ImGuiTextFilter compFilter;
            compFilter.Draw("Components", 500);
            ImGui::Text("Available Components");
            ImGui::Separator();
            ImGui::BeginChild("Components", ImVec2(0, 100));
            for (unsigned i = 0; i < componentNames.size(); i++)
            {
                if (compFilter.PassFilter(componentNames[i].c_str()))
                {
                    if (ImGui::MenuItem(componentNames[i].c_str()))
                    {
                        AddComponent(componentNames[i]);
                    }
                }
            }
            ImGui::EndChild();
        }

        if (selTran)
        {
            if (ImGui::CollapsingHeader("Transform"))
            {
                ImGui::PushItemWidth(100);
                ImGui::InputFloat("x      ", &(selTran->Position[0]), 0.5, 1);
                ImGui::SameLine();
                ImGui::InputFloat("y      ", &(selTran->Position[1]), 0.5, 1);
                ImGui::PopItemWidth();
                ImGui::DragFloat("rotation", &(selTran->Rotation), 0.05, 0, 44.0 / 7);
                ImGui::PushItemWidth(100);
                ImGui::InputFloat("x scale", &(selTran->ScaleX), 0.5, 1);
                ImGui::SameLine();
                ImGui::InputFloat("y scale", &(selTran->ScaleY), 0.5, 1);
                ImGui::InputInt("Z Order", &(selTran->ZOrder));
                ImGui::PopItemWidth();
            }
        }

        if (selModel)
        {
            if (ImGui::CollapsingHeader("Model"))
            {
                ImGui::Checkbox("Flip X", &(selModel->FlipX));
                ImGui::SameLine();
                ImGui::Checkbox("Flip Y", &(selModel->FlipY));
                static ImGuiTextFilter meshFilter;
                meshFilter.Draw("Mesh", 250.0f);
                ImGui::Text("Available Meshes");
                ImGui::Separator();
                ImGui::BeginChild("Meshes", ImVec2(0, 100), false, ImGuiWindowFlags_HorizontalScrollbar);
                for(unsigned i = 0; i < meshNames.size(); i++)
                {
                    if(meshFilter.PassFilter((meshNames[i]).c_str()))
                    {
                        if (ImGui::MenuItem((meshNames[i]).c_str()))
                        {
                            selModel->ModelMesh = meshNames[i];
                        }
                    }
                }
                ImGui::EndChild();
            }
        }

        if (selCollision)
        {
            if(ImGui::CollapsingHeader("Collision"))
            {
                ImGui::Text("Collision Box");
                ImGui::PushItemWidth(100);
                ImGui::InputFloat("Width", &(selCollision->Width));
                ImGui::SameLine();
                ImGui::InputFloat("Height", &(selCollision->Height));
                ImGui::PopItemWidth();
                render->DrawRectangle(selTran->Position, selCollision->Width, selCollision->Height, selTran->Rotation);
                ImGui::Checkbox("Resolve Collision", &(selCollision->ResolveCollisions));
            }
        }

        if (selSprite)
        {
            if(ImGui::CollapsingHeader("Sprite"))
            {

            }
        }

        if (selPhysics)
        {
            if(ImGui::CollapsingHeader("Physics"))
            {
                ImGui::InputFloat("Mass", &(selPhysics->Mass));
            }
        }

        if (selLight)
        {
            if(ImGui::CollapsingHeader("Light"))
            {

            }
        }

        if (selController)
        {
            if(ImGui::CollapsingHeader("Player Controller"))
            {
                ImGui::PushItemWidth(75);
                ImGui::InputInt("Jump Key", &(selController->JumpKey));
                ImGui::PopItemWidth();
                ImGui::InputInt("Move Left Key", &(selController->LeftKey));
                ImGui::InputInt("Move Right Key", &(selController->RightKey));
                ImGui::InputFloat("Jump Speed", &(selController->JumpSpeed));
                ImGui::InputFloat("Move Speed", &(selController->MoveSpeed));
            }
        }

        if (selSound)
        {
            if(ImGui::CollapsingHeader("Sound"))
            {
                ImGui::Checkbox("Looping", &(selSound->Looping));
                ImGui::SameLine();
                ImGui::PushItemWidth(75);
                ImGui::InputInt("Loop Count", &(selSound->LoopCount));
                ImGui::InputFloat("Volume", &(selSound->Volume));
                ImGui::SameLine();
                ImGui::InputFloat("Pitch", &(selSound->Pitch));
                ImGui::PopItemWidth();
            }
        }

        if (selCamera)
        {
            if(ImGui::CollapsingHeader("Camera"))
            {
                ImGui::InputFloat("Size", &(selCamera->Size));
                if (ImGui::Button("Set Camera"))
                    render->SetCamera(*selection);
            }
        }

        if (selSprtxt)
        {
            if(ImGui::CollapsingHeader("Sprite Text"))
            {
                ImGui::InputText("Text", spriteTextBuf, 500);
                selSprtxt->Text = spriteTextBuf;
                ImGui::ColorEdit4("Color", spriteTextColor);
                selSprtxt->TextColor.SetAll(spriteTextColor[0], spriteTextColor[1], spriteTextColor[2], spriteTextColor[3]);
            }
        }

        if (selection != camera && ImGui::Button("Delete Object"))
        {
            DeleteEntity(selection);
            selection = NULL;
        }
        ImGui::End();
    }

    else
        showWindow = false;


    if (selMode && ImGui::IsMouseClicked(0) && !ImGui::IsMouseHoveringAnyWindow())
    {
        ImGuiToEngine(mousePos);
        std::vector<Entity*> ents = GetEntitiesAtPosition(mousePos);
        selection = NULL;
        for (unsigned i = 0; i < ents.size(); i++)
        {
            if (ents[i] != camera)
            {
                selection = ents[i];
                std::string name = selection->GetName();
                strcpy(entName, name.c_str());
                selTran = selection->GetComponent<Components::Transform>();
                selModel = selection->GetComponent<Components::Model>();
                selCamera = selection->GetComponent<Components::Camera>();
                selLight = selection->GetComponent<Components::Light>();
                selCollision = selection->GetComponent<Components::Collision>();
                selPhysics = selection->GetComponent<Components::Physics>();
                selSound = selection->GetComponent<Components::SoundEmitter>();
                selSprite = selection->GetComponent<Components::Sprite>();
                selSprtxt = selection->GetComponent<Components::SpriteText>();
                selController = selection->GetComponent<Components::CharacterController>();
                if (selSprtxt)
                {
                    strcpy(spriteTextBuf, selSprtxt->Text.c_str());
                    spriteTextColor[0] = selSprtxt->TextColor.GetR();
                    spriteTextColor[1] = selSprtxt->TextColor.GetG();
                    spriteTextColor[2] = selSprtxt->TextColor.GetB();
                    spriteTextColor[3] = selSprtxt->TextColor.GetA();
                }
                selMade = true;
                break;
            }
        }
    }

    if (selMade && ImGui::IsMouseDown(0) && ImGui::IsMouseDragging(0))
    {
        ImGuiToEngine(mousePos);
        selTran->Position[0] = mousePos[0];
        selTran->Position[1] = mousePos[1];
    }

    if (moveMode && ImGui::IsMouseDown(1))
    {
        ImGuiToEngine(mousePos);
        Point delta = mousePos - camTrans->Position;
        camTrans->Position[0] += delta[0] * ForLease->FrameRateController().GetDt();
        camTrans->Position[1] += delta[1] * ForLease->FrameRateController().GetDt();
    }
    if ((selMode && ImGui::IsMouseReleased(0)) || !selMode)
        selMade = false;

    if (ImGui::IsKeyDown(ImGui::GetIO().KeyMap[ImGuiKey_LeftArrow]))
    {
        camTrans->Position[0] -= 100 * ForLease->FrameRateController().GetDt();
    }
    if (ImGui::IsKeyDown(ImGui::GetIO().KeyMap[ImGuiKey_RightArrow]))
    {
        camTrans->Position[0] += 100 * ForLease->FrameRateController().GetDt();
    }
    if (ImGui::IsKeyDown(ImGui::GetIO().KeyMap[ImGuiKey_DownArrow]))
    {
        camTrans->Position[1] -= 100 * ForLease->FrameRateController().GetDt();
    }
    if (ImGui::IsKeyDown(ImGui::GetIO().KeyMap[ImGuiKey_UpArrow]))
    {
        camTrans->Position[1] += 100 * ForLease->FrameRateController().GetDt();
    }
    if (ImGui::IsKeyDown(Keys::C) && ImGui::GetIO().KeyCtrl)
    {
        strcpy(entName, camera->GetName().c_str());
        selection = camera;
        selTran = camTrans;
        selCamera = camera->GetComponent<Components::Camera>();
    }
    render->Update(Entities);
    ImGui::Render();
    ForLease->GameWindow->UpdateGameWindow();
}

void TestState::Deinitialize()
{
    ImGui_ImplSdl_Shutdown();
}

void TestState::Unload()
{

}
