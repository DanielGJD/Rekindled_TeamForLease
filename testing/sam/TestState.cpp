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
bool addObject = false;
bool clickAdd = false;
class MouseListener {
    public:
    void OnMouseDown(const ForLeaseEngine::Event* e) {
        const ForLeaseEngine::MouseButtonEvent* mouse_e = reinterpret_cast<const ForLeaseEngine::MouseButtonEvent*>(e);
        std::cout << mouse_e->EventName << ":" << std::endl
                  << mouse_e->Button << std::endl
                  << mouse_e->Clicks << std::endl
                  << "[" << mouse_e->ScreenLocation[0] << "," << mouse_e->ScreenLocation[1] << "]" << std::endl << std::endl;
        mousePos[0] = mouse_e->ScreenLocation[0] - 720 / 2;
        mousePos[1] = (mouse_e->ScreenLocation[1] - 720 / 2);

        if (mouse_e->Button == 3 && clickAdd)
            addObject = true;

    }

    void OnMouseUp(const ForLeaseEngine::Event* e) {
        const ForLeaseEngine::MouseButtonEvent* mouse_e = reinterpret_cast<const ForLeaseEngine::MouseButtonEvent*>(e);
        std::cout << mouse_e->EventName << ":" << std::endl
                  << mouse_e->Button << std::endl
                  << mouse_e->Clicks << std::endl
                  << "[" << mouse_e->ScreenLocation[0] << "," << mouse_e->ScreenLocation[1] << "]" << std::endl << std::endl;
        addObject = false;
    }
};

class KeyboardListener
{
public:
    void OnKeyDown(const Event* e)
    {
        const KeyboardEvent * key_e = reinterpret_cast<const KeyboardEvent*>(e);
        std::cout << key_e->EventName << std::endl;
        std::cout << key_e->Key << std::endl;
        std::cout << key_e->State << std::endl;
        std::cout << key_e->Modifier << std::endl;
        if (key_e->Key == Keys::Escape && key_e->State == KeyState::Pressed)
        {
            ForLease->GameStateManager().SetAction(Modules::StateAction::Quit);
        }
    }
};

LevelComponents::Renderer* render;
Mesh* testMesh;
Entity* camera;
Entity* ent;
MouseListener mouse;
KeyboardListener keyboard;
SDL_Window* window;
Entity *selection;
std::vector<std::string> meshNames;
void TestState::Load()
{
    render = new LevelComponents::Renderer(*this);
    ForLease->Dispatcher.Attach(NULL, &mouse, "MouseButtonDown", &MouseListener::OnMouseDown);
    ForLease->Dispatcher.Attach(NULL, &mouse, "MouseButtonUp", &MouseListener::OnMouseUp);
    ForLease->Dispatcher.Attach(NULL, &keyboard, "KeyDown", &KeyboardListener::OnKeyDown);

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

bool editMode = false;
bool none = true;
bool showWindow = false;
char meshBuffer[500] = {0};
void TestState::Update()
{
    if (editMode)
        ForLease->OSInput.ProcessAllInputWithImgui();

    else
        ForLease->OSInput.ProcessAllInput();

    ImGui_ImplSdl_NewFrame(window);
    ImGui::Checkbox("Place Objects", &clickAdd);
    ImGui::Checkbox("Edit Mode", &editMode);

    if(addObject && none)
    {
        Entity* entity = AddEntity();
        entity->AddComponent(new Components::Transform(*entity, mousePos[0], mousePos[1], 100, 100, 0));
        Components::Model* model = new Components::Model(*entity, true, "MeshTest.json", "");
        entity->AddComponent(model);
        selection = entity;
        addObject = false;
        none = false;
    }

    if (selection)
    {
        showWindow = true;
        ImGui::Begin("ObjectEditor", &showWindow);
        Components::Transform* tran = selection->GetComponent<Components::Transform>();
        if (tran && ImGui::CollapsingHeader("Transform"))
        {
            ImGui::PushItemWidth(100);
            ImGui::InputFloat("x      ", &(tran->Position[0]), 0.5, 1);
            ImGui::SameLine();
            ImGui::InputFloat("y      ", &(tran->Position[1]), 0.5, 1);
            ImGui::PopItemWidth();
            ImGui::DragFloat("rotation", &(tran->Rotation), 0.1, 0, 360);
            ImGui::PushItemWidth(100);
            ImGui::InputFloat("x scale", &(tran->ScaleX), 0.5, 1);
            ImGui::SameLine();
            ImGui::InputFloat("y scale", &(tran->ScaleY), 0.5, 1);
            ImGui::InputInt("Z Order", &(tran->ZOrder));
        }

        Components::Model* selModel = selection->GetComponent<Components::Model>();

        if (selModel && ImGui::CollapsingHeader("Model"))
        {
            static ImGuiTextFilter filter;
            ImGui::InputText("Mesh", filter.InputBuf, 500);
            std::string temp = meshBuffer;
            Mesh* check = ForLease->Resources.GetMesh(temp);

            if (check)
                selModel->ModelMesh = temp;
        }
        ImGui::End();
    }

    else
        showWindow = false;

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
