#include "MeshEditorState.h"
#include <string>
#include "Entity.h"
#include "ComponentsInclude.h"
#include "Vector.h"
#include "Engine.h"
#include "Color.h"
#include "LevelComponentRenderer.h"
#include "Mesh.h"
#include "Serialize.h"
#include "Event.h"
#include "KeyboardEvent.h"
#include "Keys.h"
#include "imgui.h"
#include "Exception.h"
#include "imgui_impl_sdl.h"
#include "Face.h"
#include "Edge.h"
#include <vector>
#include <iostream>
#include <cmath>
#include <unordered_set>

namespace ForLeaseEngine {
    void LoadMesh();

    void DrawMainMenu();
    void DrawFileMenu();
    void NewMeshPopup();
    void OpenMeshPopup();
    void MainWindow();
    void VertexModeWindow();
    void EdgeModeWindow();
    void FaceModeWindow();

    enum Mode {
        Vertex,
        Edge,
        Face
    };

    static LevelComponents::Renderer* render;
    static Entity* camera;
    static Mesh* mesh;
    static Entity* model;

    static std::string FileName;

    static float timer;

    static bool ShowNewPopup = false;
    static bool ShowOpenPopup = false;
    static bool MeshLoaded = false;
    static Mode CurrentMode = Mode::Vertex;

    static std::unordered_set<int> SelectedVertices;
    static std::unordered_set<int> SelectedEdges;
    static std::unordered_set<int> SelectedFaces;

    void MeshEditorState::Load() {
        if(!ImGui_ImplSdl_Init(ForLease->GameWindow->DangerousGetRawWindow())){
            std::cout << "Failed to init ImGui" << std::endl;
            throw new Exception("Failed to init ImGui");
        }
        else {
            std::cout << "ImGui init success" << std::endl;
        }

        timer = 0;

        render = new LevelComponents::Renderer(*this);
        render->SetClearColor(0, 0, 0.5f, 1);
        ForLease->Resources.LoadFont("Arial.fnt");

        camera = AddEntity();
        camera->AddComponent(new Components::Transform(*camera, Point(0, 0.05f), 1, 1, 0));
        camera->AddComponent(new Components::Camera(*camera, 0, 1, 2.2));
        render->SetCamera(*camera);

        model = AddEntity();
        model->AddComponent(new Components::Transform(*model, Point(0, 0), 1, 1, 0));
        model->AddComponent(new Components::Model(*model, true, "", "", Color(1, 1, 1, 1), BlendMode::NONE, true, true));

//        instructions = AddEntity();
//        Components::Transform* trans = new Components::Transform(*instructions, Point(-ForLease->GameWindow->GetXResolution() / 2, ForLease->GameWindow->GetYResolution() / 2), 1, 1, 0, 0);
//        Components::SpriteText* text = new Components::SpriteText(*instructions, "Arial.fnt", "DUMMY TEXT", Color(1, 0, 0, 1));
//        instructions->AddComponent(trans);
//        instructions->AddComponent(text);
    }

    void MeshEditorState::Initialize() {
    }

    void MeshEditorState::Update() {
        ForLease->OSInput.ProcessAllInputWithImgui();
        ImGui_ImplSdl_NewFrame(ForLease->GameWindow->DangerousGetRawWindow());
        DrawMainMenu();
        if(ShowNewPopup)
            NewMeshPopup();
        if(ShowOpenPopup)
            OpenMeshPopup();
        if(MeshLoaded)
            MainWindow();

        timer += ForLease->FrameRateController().GetDt();
        render->Update(Entities);
        render->SetDrawingColor(1, 0, 0, 1);
        render->DrawRectangle(Point(0, 0), 2, 2, 0);
        ImGui::Render();
        ForLease->GameWindow->UpdateGameWindow();
    }

    void MeshEditorState::Deinitialize() {

    }

    void MeshEditorState::Unload() {

    }

    void DrawMainMenu() {
        if(ImGui::BeginMainMenuBar()) {
            DrawFileMenu();
            ImGui::EndMainMenuBar();
        }
    }

    void DrawFileMenu() {
        if(ImGui::BeginMenu("File")) {
            if(ImGui::MenuItem("New")) {
                std::cout << "NEW SELECTED" << std::endl;
                ShowNewPopup = true;
            }
            if(ImGui::MenuItem("Open")) {
                std::cout << "OPEN SELECTED" << std::endl;
                ShowOpenPopup = true;
            }
            if(ImGui::MenuItem("Save", "", false, MeshLoaded)) {

            }
            ImGui::EndMenu();
        }
    }

    void NewMeshPopup() {
        ImGui::OpenPopup("NewMesh");
        if(ImGui::BeginPopup("NewMesh")) {
            static char fileName[32] = "";
            ImGui::Text("New Mesh");
            ImGui::InputText("File Name", fileName, 32);
            if(ImGui::Button("Create")) {
                ShowNewPopup = false;
                Mesh tempmesh;
                Serializer meshSaver;
                tempmesh.Serialize(meshSaver);
                meshSaver.WriteFile(fileName);
                ForLease->Resources.LoadMesh(fileName);
                mesh = ForLease->Resources.GetMesh(fileName);
                model->GetComponent<Components::Model>()->ModelMesh = fileName;
                FileName = fileName;
                fileName[0] = 0;
                MeshLoaded = true;
            }
            ImGui::EndPopup();
        }
    }

    void OpenMeshPopup() {
        ImGui::OpenPopup("OpenMesh");
        if(ImGui::BeginPopup("OpenMesh")) {
            static char fileName[32] = "";
            ImGui::Text("Open Mesh");
            ImGui::InputText("File Name", fileName, 32);
            if(ImGui::Button("Open")) {
                ShowOpenPopup = false;
                ForLease->Resources.LoadMesh(fileName);
                mesh = ForLease->Resources.GetMesh(fileName);
                model->GetComponent<Components::Model>()->ModelMesh = fileName;
                FileName = fileName;
                fileName[0] = 0;
                MeshLoaded = true;
            }
            ImGui::EndPopup();
        }
    }

    void MainWindow() {
        ImGui::Begin(FileName.c_str());
        static int mode = 0;
        ImGui::RadioButton("Vertex", &mode, 0); ImGui::SameLine();
        ImGui::RadioButton("Edge", &mode, 1); ImGui::SameLine();
        ImGui::RadioButton("Face", &mode, 2);
        ImGui::Text("Mesh Stats:");
        ImGui::Text("Vertices: %d\nEdges: %d\nFaces: %d", mesh->GetVertexCount(),
                                                          mesh->GetEdgeCount(),
                                                          mesh->GetFaceCount());
        switch(mode) {
        case 0:
            CurrentMode = Mode::Vertex;
            VertexModeWindow();
            break;
        case 1:
            CurrentMode = Mode::Edge;
            EdgeModeWindow();
            break;
        case 2:
            CurrentMode = Mode::Face;
            FaceModeWindow();
            break;
        }
        ImGui::End();
    }

    void VertexModeWindow() {
        ImGui::Spacing();
        ImGui::Text("Selected Vertices: %d", SelectedVertices.size());
    }

    void EdgeModeWindow() {
        ImGui::Spacing();
        ImGui::Text("Selected Edges: %d", SelectedEdges.size());
    }

    void FaceModeWindow() {
        ImGui::Spacing();
        ImGui::Text("Selected Faces: %d", SelectedFaces.size());

        // Some bs testing code
        if(mesh->GetFaceCount() > 0) {
            int faceIndex = 0;
            //Face face = mesh->GetFace(0);
            //Color faceColor = mesh->GetFaceColor(0);
            if(faceIndex > 1) {
                ImGui::Button("MoveBack");
            }
            if(faceIndex < mesh->GetFaceCount() - 1) {
                ImGui::Button("MoveForward");
            }
        }
    }
}
