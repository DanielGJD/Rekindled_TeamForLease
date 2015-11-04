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
#include "../../fle/libs/imgui/imgui.h"
#include "../../fle/libs/imgui/imgui_impl_sdl.h"
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
std::vector<std::string> meshNames;
void TestState::Load()
{
    render = new LevelComponents::Renderer(*this);

    camera = AddEntity();
    camera->AddComponent(new Components::Transform(*camera, 0, 0, 1, 1, 0));
    camera->AddComponent(new Components::Camera(*camera, 0, 1, 720));

    render->SetCamera(*camera);
    window = ForLease->GameWindow->DangerousGetRawWindow();
    ForLease->Resources.LoadMesh("MeshTest.json");
    ForLease->Resources.LoadMesh("MainCharMesh.json");
    meshNames = ForLease->Resources.GetLoadedMeshNames();
}

void TestState::Initialize()
{
    ImGui_ImplSdl_Init(window);
}



void ImGuiToEngine(const ImVec2& v1, Point& v2)
{
    int dx = ForLease->GameWindow->GetXResolution() / 2;
    int dy = ForLease->GameWindow->GetYResolution() / 2;
    v2[0] = v1.x - dx;
    v2[1] = v1.y * -1 + dy;
}

bool none = true;
bool showWindow = false;
bool selMade = false;
Components::Transform* selTran;
Components::Model* selModel;
void TestState::Update()
{
    ForLease->OSInput.ProcessAllInputWithImgui();

    ImGui_ImplSdl_NewFrame(window);
    ImGui::Checkbox("Place Objects", &clickAdd);
    ImGui::Checkbox("Translate Tool", &moveMode);

    if(clickAdd && ImGui::IsMouseClicked(1))
    {
        ImGuiToEngine(ImGui::GetMousePos(), mousePos);
        render->ScreenToWorld(mousePos);
        Entity* entity = AddEntity();
        entity->AddComponent(new Components::Transform(*entity, mousePos[0], mousePos[1], 100, 100, 0));
        Components::Model* model = new Components::Model(*entity, true, "MeshTest.json", "");
        entity->AddComponent(model);
    }

    if (ImGui::IsKeyPressed(Keys::Q))
        ForLease->GameStateManager().SetAction(Modules::StateAction::Quit);

    if (ImGui::IsKeyPressed(Keys::Escape))
        selection = NULL;

    if (selection)
    {
        showWindow = true;
        ImGui::Begin("ObjectEditor", &showWindow);
        if (selTran && ImGui::CollapsingHeader("Transform"))
        {
            ImGui::PushItemWidth(100);
            ImGui::InputFloat("x      ", &(selTran->Position[0]), 0.5, 1);
            ImGui::SameLine();
            ImGui::InputFloat("y      ", &(selTran->Position[1]), 0.5, 1);
            ImGui::PopItemWidth();
            ImGui::DragFloat("rotation", &(selTran->Rotation), 0.1, 0, 360);
            ImGui::PushItemWidth(100);
            ImGui::InputFloat("x scale", &(selTran->ScaleX), 0.5, 1);
            ImGui::SameLine();
            ImGui::InputFloat("y scale", &(selTran->ScaleY), 0.5, 1);
            ImGui::InputInt("Z Order", &(selTran->ZOrder));
        }

        if (selModel && ImGui::CollapsingHeader("Model"))
        {
            static ImGuiTextFilter filter;
            filter.Draw("Mesh", 250.0f);
            ImGui::Text("Available Meshes");
            ImGui::Separator();
            ImGui::BeginChild("Meshes", ImVec2(0, 200), false, ImGuiWindowFlags_HorizontalScrollbar);
            for(unsigned i = 0; i < meshNames.size(); i++)
            {
                if(filter.PassFilter((meshNames[i]).c_str()))
                {
                    if (ImGui::MenuItem((meshNames[i]).c_str()))
                    {
                        selModel->ModelMesh = meshNames[i];
                    }
                }
            }
            ImGui::EndChild();

        }
        ImGui::End();
    }

    else
        showWindow = false;


    if (moveMode && ImGui::IsMouseClicked(0))
    {

        Point delta;
        ImGuiToEngine(ImGui::GetMousePos(), mousePos);
        render->ScreenToWorld(mousePos);
        for (unsigned i = 0; i < Entities.size(); i++)
        {
            Components::Transform* tran = Entities[i]->GetComponent<Components::Transform>();
            if (!selMade && tran && Point::Distance(mousePos, tran->Position) <= 20)
            {
                selection = Entities[i];
                selTran = tran;
                selModel = selection->GetComponent<Components::Model>();
                selMade = true;
                break;
            }

        }
    }

    if (ImGui::IsMouseDown(0) && ImGui::IsMouseDragging(0) && selMade)
    {
        ImGuiToEngine(ImGui::GetMousePos(), mousePos);
        render->ScreenToWorld(mousePos);
        selTran->Position[0] = mousePos[0];
        selTran->Position[1] = mousePos[1];
    }

    if ((moveMode && ImGui::IsMouseReleased(0)) || !moveMode)
        selMade = false;

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
