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

        Entity*                camera;
        Components::Transform* camTrans;
        Components::Camera*    camCamera;

        std::vector<std::string> meshNames;
        std::vector<std::string> componentNames;
        std::vector<std::string> soundNames;
        std::vector<std::string> archetypeNames;
        std::vector<std::string> fontNames;

        bool clickAdd   = false;
        bool newSelect  = false;
        bool moveMode   = false;
        bool showWindow = false;
        bool selMade    = false;
        bool archSpawn  = false;
        bool selMode    = false;

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
                    std::cout << leg::statefile << std::endl;
                }

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Load"))
            {
                if (ImGui::BeginMenu("Level"))
                {
                    if (ImGui::InputText("File Name", leg::statefile, 70, ImGuiInputTextFlags_EnterReturnsTrue))
                    {
                        std::cout << leg::statefile << std::endl;
                    }

                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Archetype"))
                {
                    if (ImGui::InputText("File Name", leg::archetypefile, 70, ImGuiInputTextFlags_EnterReturnsTrue))
                    {
                        std::cout << leg::archetypefile << std::endl;
                    }

                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Mesh"))
                {
                    if (ImGui::InputText("File Name", leg::meshfile, 70, ImGuiInputTextFlags_EnterReturnsTrue))
                    {
                        std::cout << leg::meshfile << std::endl;
                    }

                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Sound"))
                {
                    if (ImGui::InputText("File Name", leg::soundfile, 70, ImGuiInputTextFlags_EnterReturnsTrue))
                    {
                        std::cout << leg::soundfile << std::endl;
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
            static ImGuiTextFilter compfilter;
        }

        if (leg::selTran && ImGui::CollapsingHeader("Transform"))
        {

        }

        if (leg::selModel && ImGui::CollapsingHeader("Model"))
        {

        }

        if (leg::selCollision && ImGui::CollapsingHeader("Collision"))
        {

        }

        if (leg::selPhysics && ImGui::CollapsingHeader("Physics"))
        {

        }

        if (leg::selSound && ImGui::CollapsingHeader("Sound Emitter"))
        {

        }

        if (leg::selSprtxt && ImGui::CollapsingHeader("Sprite Text"))
        {

        }

        if (leg::selCamera && ImGui::CollapsingHeader("Camera"))
        {

        }

        if (leg::selController && ImGui::CollapsingHeader("Character Controller"))
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

    }

    void LevelEditor::SaveFiles(Serializer& root)
    {

    }

    void LevelEditor::LoadFiles(Serializer& root)
    {

    }
}

