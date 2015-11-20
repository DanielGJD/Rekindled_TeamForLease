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
    //static void LoadMesh();

    // ImGui Stuff
    static void DrawMainMenu();
    static void DrawFileMenu();
    static void NewMeshPopup();
    static void OpenMeshPopup();
    static void MainWindow();
    static void VertexModeWindow();
    static void EdgeModeWindow();
    static void FaceModeWindow();

    // Misc Rendering
    static void RenderSelections();
    static void RenderGrid();
    //static void RenderShadowSelections();

    //input functions
    static Point GetMousePosition();
    static void VertexModeTasks();
    static void EdgeModeTasks();
    static void FaceModeTasks();
    static void ClearAllSelections();

    enum Mode {
        None,
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
    static Mode CurrentMode = Mode::None;

    static std::unordered_set<int> SelectedVertices;
    static std::unordered_set<int> SelectedEdges;
    static std::unordered_set<int> SelectedFaces;

    static std::unordered_map<int, Point> ShadowVertices;

    static Point LastMousePos;
    static Point CurrentMousePos;
    static Point TransformOrigin;
    static bool Moving;
    static bool Rotating;
    static bool Scaling;

    static int LastFaceSelected;

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
        render->SetClearColor(0.75f, 0.75f, 0.75f, 1);

        camera = AddEntity();
        camera->AddComponent(new Components::Transform(*camera, Point(0, 0.05f), 1, 1, 0));
        camera->AddComponent(new Components::Camera(*camera, 0, 1, 2.2));
        render->SetCamera(*camera);

        model = AddEntity();
        model->AddComponent(new Components::Transform(*model, Point(0, 0), 1, 1, 0));
        model->AddComponent(new Components::Model(*model, true, false, false, "", "", Color(1, 1, 1, 1), BlendMode::NONE, true, true));

//        instructions = AddEntity();
//        Components::Transform* trans = new Components::Transform(*instructions, Point(-ForLease->GameWindow->GetXResolution() / 2, ForLease->GameWindow->GetYResolution() / 2), 1, 1, 0, 0);
//        Components::SpriteText* text = new Components::SpriteText(*instructions, "Arial.fnt", "DUMMY TEXT", Color(1, 0, 0, 1));
//        instructions->AddComponent(trans);
//        instructions->AddComponent(text);
    }

    void MeshEditorState::Initialize() {
    }

    void MeshEditorState::Update() {
        // Handle input
        ForLease->OSInput.ProcessAllInputWithImgui();

        // Do Mode Tasks
        switch(CurrentMode) {
        case Mode::None:
            break;
        case Mode::Vertex:
            VertexModeTasks();
            break;
        case Mode::Edge:
            break;
        case Mode::Face:
            FaceModeTasks();
            break;
        }

        ImGui_ImplSdl_NewFrame(ForLease->GameWindow->DangerousGetRawWindow());
        DrawMainMenu();
        if(ShowNewPopup)
            NewMeshPopup();
        if(ShowOpenPopup)
            OpenMeshPopup();
        if(MeshLoaded)
            MainWindow();

        timer += ForLease->FrameRateController().GetDt();
        RenderGrid();
        render->Update(Entities);
        RenderSelections();
        //if(Moving || Rotating || Scaling)
            //RenderShadowSelections();
        render->SetDrawingColor(1, 0, 0);
        render->DrawLine(Point(0, 0), Point(0.1f, 0));
        render->SetDrawingColor(0, 1, 0, 1);
        render->DrawLine(Point(0, 0), Point(0, 0.1f));
        ImGui::Render();
        ForLease->GameWindow->UpdateGameWindow();
    }

    void MeshEditorState::Deinitialize() {

    }

    void MeshEditorState::Unload() {

    }

    static void DrawMainMenu() {
        if(ImGui::BeginMainMenuBar()) {
            DrawFileMenu();
            ImGui::EndMainMenuBar();
        }
    }

    static void DrawFileMenu() {
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
                Serializer meshSaver;
                mesh->Serialize(meshSaver);
                meshSaver.WriteFile(FileName);
            }
            if(ImGui::MenuItem("Close", "", false, MeshLoaded)) {
                CurrentMode = Mode::None;
                MeshLoaded = false;
                mesh = NULL;
                Components::Model* workingModel = model->GetComponent<Components::Model>();
                workingModel->ModelMesh = "";
                workingModel->ModelTexture = "";
                SelectedVertices.clear();
                SelectedEdges.clear();
                SelectedFaces.clear();
            }
            if(ImGui::MenuItem("Quit")) {
                ForLease->GameStateManager().SetAction(Modules::StateAction::Quit);
            }
            ImGui::EndMenu();
        }
    }

    static void NewMeshPopup() {
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
            ImGui::SameLine();
            if(ImGui::Button("Cancel")) {
                ShowNewPopup = false;
            }
            ImGui::EndPopup();
        }
    }

    static void OpenMeshPopup() {
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
            ImGui::SameLine();
            if(ImGui::Button("Cancel")) {
                ShowOpenPopup = false;
            }
            ImGui::EndPopup();
        }
    }

    static void MainWindow() {
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
            if(CurrentMode != Mode::Vertex) {
                CurrentMode = Mode::Vertex;
                ClearAllSelections();
            }
            VertexModeWindow();
            break;
        case 1:
            if(CurrentMode != Mode::Edge) {
                CurrentMode = Mode::Edge;
                ClearAllSelections();
            }
            EdgeModeWindow();
            break;
        case 2:
            if(CurrentMode != Mode::Face) {
                CurrentMode = Mode::Face;
                ClearAllSelections();
            }
            FaceModeWindow();
            break;
        }
        ImGui::End();
    }

    static void VertexModeWindow() {
        ImGui::Spacing();
        ImGui::Text("Selected Vertices: %d", SelectedVertices.size());
    }

    static void EdgeModeWindow() {
        ImGui::Spacing();
        ImGui::Text("Selected Edges: %d", SelectedEdges.size());
    }

    static void FaceModeWindow() {
        ImGui::Spacing();
        ImGui::Text("Selected Faces: %d", SelectedFaces.size());

        // Some bs testing code
        if(SelectedFaces.size() > 0) {
            //int faceIndex = *SelectedFaces.begin();
            //Face face = mesh->GetFace(0);
            Color faceColor = mesh->GetFaceColor(LastFaceSelected);
            float color[] = {faceColor.GetR(), faceColor.GetG(), faceColor.GetB(), faceColor.GetA()};
            ImGui::Spacing();
            if(SelectedFaces.size() == 1 && LastFaceSelected > 0) {
                ImGui::SameLine();
                if(ImGui::Button("MoveBack")) {
                    SelectedFaces.erase(LastFaceSelected);
                    IndexedFace tempFace = mesh->GetIndexedFace(LastFaceSelected);
                    Color tempColor = mesh->GetFaceColor(LastFaceSelected);
                    mesh->SetFace(mesh->GetIndexedFace(LastFaceSelected - 1), LastFaceSelected);
                    mesh->SetFace(tempFace, LastFaceSelected - 1);
                    mesh->SetFaceColor(mesh->GetFaceColor(LastFaceSelected - 1), LastFaceSelected);
                    mesh->SetFaceColor(tempColor, LastFaceSelected - 1);
                    --LastFaceSelected;
                    SelectedFaces.insert(LastFaceSelected);
                }
            }
            if(SelectedFaces.size() == 1 && LastFaceSelected < mesh->GetFaceCount() - 1) {
                ImGui::SameLine();
                if(ImGui::Button("MoveForward")) {
                    SelectedFaces.erase(LastFaceSelected);
                    IndexedFace tempFace = mesh->GetIndexedFace(LastFaceSelected);
                    Color tempColor = mesh->GetFaceColor(LastFaceSelected);
                    mesh->SetFace(mesh->GetIndexedFace(LastFaceSelected + 1), LastFaceSelected);
                    mesh->SetFace(tempFace, LastFaceSelected + 1);
                    mesh->SetFaceColor(mesh->GetFaceColor(LastFaceSelected + 1), LastFaceSelected);
                    mesh->SetFaceColor(tempColor, LastFaceSelected + 1);
                    ++LastFaceSelected;
                    SelectedFaces.insert(LastFaceSelected);
                }
            }
            ImGui::ColorEdit4("Face Color", color);
            if(color[0] != faceColor.GetR() || color[1] != faceColor.GetG() || color[2] != faceColor.GetB() || color[3] != faceColor.GetA()) {
                for(std::unordered_set<int>::iterator i = SelectedFaces.begin(); i != SelectedFaces.end(); ++i) {
                    mesh->SetFaceColor(color[0], color[1], color[2], color[3], *i);
                }
            }
        }
    }

    // Render selections
    static void RenderSelections() {
        render->SetDrawingColor(0, 0, 0, 1);
        render->SetDebugPointSize(8);
        for(std::unordered_set<int>::iterator i = SelectedFaces.begin(); i != SelectedVertices.end(); ++i) {
            IndexedFace face = mesh->GetIndexedFace(*i);
            Point v1 = mesh->GetVertex(face.Indices[0]);
            Point v2 = mesh->GetVertex(face.Indices[1]);
            Point v3 = mesh->GetVertex(face.Indices[2]);
            float x = (v1[0] + v2[0] + v3[0]) / 3;
            float y = (v1[1] + v2[1] + v3[1]) / 3;
            render->DrawPoint(Point(x, y));
        }

        render->SetDrawingColor(Color(0, 0, 0, 1));
        render->SetDebugLineWidth(4);
        for(std::unordered_set<int>::iterator i = SelectedEdges.begin(); i != SelectedEdges.end(); ++i) {
            IndexedEdge edge = mesh->GetIndexedEdge(*i);
            render->DrawLine(mesh->GetVertex(edge.Indices[0]), mesh->GetVertex(edge.Indices[1]));
        }

        render->SetDrawingColor(Color(1, 1, 1, 1));
        render->SetDebugPointSize(4);
        for(std::unordered_set<int>::iterator i = SelectedVertices.begin(); i != SelectedVertices.end(); ++i) {
            render->DrawPoint(mesh->GetVertex(*i));
        }
    }

    static void RenderGrid() {
        int numLines = 20;
        render->SetDrawingColor(0.6f, 0.6f, 0.6f);
        for(int i = 0; i <= numLines; ++i) {
            float coord = 1.0f - 2.0f / numLines * i;
            render->DrawLine(Point(-1, coord), Point(1, coord));
            render->DrawLine(Point(coord, -1), Point(coord, 1));
        }
    }

    // Input functions
    static Point GetMousePosition() {
        ImVec2 screenLocation = ImGui::GetMousePos();
        return render->ScreenToWorld(Point(screenLocation.x, ForLease->GameWindow->GetYResolution() - screenLocation.y));
    }

    static void VertexModeTasks() {
        // Mouse input
        if(ImGui::IsMouseClicked(0) && !ImGui::IsMouseHoveringAnyWindow() && !Moving && !Rotating && !Scaling) {
            int index = mesh->GetVertexIndexNear(GetMousePosition());
            if(ImGui::GetIO().KeyShift) {
                if(index >= 0) {
                    std::unordered_set<int>::iterator i = SelectedVertices.find(index);
                    if(i == SelectedVertices.end()) {
                        SelectedVertices.insert(index);
                    }
                    else {
                        SelectedVertices.erase(i);
                    }
                }
            }
            else {
                SelectedVertices.clear();
                if(index >= 0)
                    SelectedVertices.insert(index);
            }
        }

        if(Moving) {
            LastMousePos = CurrentMousePos;
            CurrentMousePos = GetMousePosition();
            Vector displacement = CurrentMousePos - LastMousePos;
            for(std::unordered_set<int>::iterator i = SelectedVertices.begin(); i != SelectedVertices.end(); ++i) {
                mesh->SetVertex(mesh->GetVertex(*i) + displacement, *i);
            }

            if(ImGui::IsKeyPressed(Keys::Escape)) {
                Moving = false;
                for(std::unordered_map<int, Point>::iterator i = ShadowVertices.begin(); i != ShadowVertices.end(); ++i) {
                    mesh->SetVertex((*i).second, (*i).first);
                }
            }

            if(ImGui::IsKeyPressed(Keys::Return)) {
                Moving = false;
            }
        }

        // Keyboard input
        if(!ImGui::IsAnyItemActive() && !Moving && !Scaling && !Rotating) {
            if(ImGui::IsKeyPressed(Keys::M)) {
                Moving = true;
                TransformOrigin = GetMousePosition();
                CurrentMousePos = TransformOrigin;
                ShadowVertices.clear();
                for(std::unordered_set<int>::iterator i = SelectedVertices.begin(); i != SelectedVertices.end(); ++i) {
                    ShadowVertices.insert(std::make_pair(*i, mesh->GetVertex(*i)));
                }
            }
            if(ImGui::IsKeyPressed(Keys::Delete)) {
                for(std::unordered_set<int>::iterator i = SelectedVertices.begin(); i != SelectedVertices.end(); ++i) {
                    mesh->DeleteVertex(*i);
                }
                ClearAllSelections();
            }
            if(ImGui::IsKeyPressed(Keys::V)) {
                Point CurrentMousePos = GetMousePosition();
                if(CurrentMousePos[0] > 1)
                    CurrentMousePos[0] = 1;
                else if(CurrentMousePos[0] < -1)
                    CurrentMousePos[0] = -1;
                if(CurrentMousePos[1] > 1)
                    CurrentMousePos[1] = 1;
                else if(CurrentMousePos[1] < -1)
                    CurrentMousePos[1] = -1;

                mesh->AddVertex(CurrentMousePos, CurrentMousePos);
            }
            if(ImGui::IsKeyPressed(Keys::E) && SelectedVertices.size() == 2) {
                std::unordered_set<int>::iterator i = SelectedVertices.begin();
                int v1 = *(i++);
                int v2 = *i;
                mesh->AddEdge(v1, v2);
            }
            if(ImGui::IsKeyPressed(Keys::F) && SelectedVertices.size() == 3) {
                std::unordered_set<int>::iterator i = SelectedVertices.begin();
                int v1 = *(i++);
                int v2 = *(i++);
                int v3 = *i;
                mesh->AddFace(v1, v2, v3, 0.5f, 0.5f, 0.5f, 1);
            }
        }
    }

    static void EdgeModeTasks() {
    }

    static void FaceModeTasks() {
        if(ImGui::IsMouseClicked(0) && !ImGui::IsMouseHoveringAnyWindow() && !Moving && !Rotating && !Scaling) {
            int index = mesh->GetFaceIndexAt(GetMousePosition());
            if(ImGui::GetIO().KeyShift) {
                if(index >= 0) {
                    std::unordered_set<int>::iterator i = SelectedFaces.find(index);
                    if(i == SelectedFaces.end()) {
                        if(SelectedFaces.size() == 0)
                            LastFaceSelected = index;
                        SelectedFaces.insert(index);
                    }
                    else {
                        bool resetLastFace = false;
                        if(index == LastFaceSelected)
                            resetLastFace = true;
                        SelectedFaces.erase(i);
                        std::unordered_set<int>::iterator j = SelectedFaces.begin();
                        if(resetLastFace && j != SelectedFaces.end())
                            LastFaceSelected = *j;
                    }
                }
            }
            else {
                SelectedFaces.clear();
                if(index >= 0)
                    SelectedFaces.insert(index);
                    LastFaceSelected = index;
            }
        }
    }

    static void ClearAllSelections() {
            SelectedVertices.clear();
            SelectedEdges.clear();
            SelectedFaces.clear();
        }
}
