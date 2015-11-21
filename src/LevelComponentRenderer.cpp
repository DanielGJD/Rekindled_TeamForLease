/*!
    \file   Renderer.cpp
    \author Christopher Hudson
    \date   09/12/15

    \brief
        Defines a class to render things to the screen, internals will change a lot but functions should remain the same

    \see Renderer.h
*/
#include <iostream>
#include <GL/gl.h>
#include "Vector.h"
#include "Matrix.h"
#include "LevelComponentRenderer.h"
#include "Entity.h"
#include "GraphicsException.h"
#include "State.h"
#include "ComponentSprite.h"
#include "ComponentModel.h"
#include "ComponentSpriteText.h"
#include "Color.h"
#include "Component.h"
#include "Timer.h"
#include "Engine.h"
#include "Exception.h"
#include "Serializable.h"
#include "Serialize.h"
//#include "json-forwards.h"
#include "MeshAnimation.h"

namespace ForLeaseEngine {
    namespace LevelComponents {
        Renderer::Renderer(State& owner) : LevelComponent(owner) {
            DrawColor = Color(1, 1, 1, 1);
            ClearColor = Color(0, 0, 0, 1);
            CurrentCamera = 0;
            BlendingMode = BlendMode::NONE;
            CurrentTexture = 0;
            VertexCount = 0;
            TriCount = 0;
            LineCount = 0;
            TextureSwapCount = 0;
            BlendModeSwapCount = 0;
            RenderTime = 0;
            DebugPointSize = 1;
            DebugLineWidth = 1;
        }

        void Renderer::Serialize(Serializer& root) {
            root.WriteUint("Type", static_cast<unsigned>(ComponentType::Renderer));
            Serializer renderer = root.GetChild("Renderer");
            Serializer clearColor = renderer.GetChild("ClearColor");
            ClearColor.Serialize(clearColor);
            renderer.Append(clearColor, "ClearColor");
            renderer.WriteUint("CurrentCamera", static_cast<unsigned int>(CurrentCamera));
            renderer.WriteUint("Type", static_cast<unsigned>(ComponentType::Renderer));
            root.Append(renderer, "Renderer");
        }

        void Renderer::Deserialize(Serializer& root) {
            Serializer renderer = root.GetChild("Renderer");
            Serializer clearColor = renderer.GetChild("ClearColor").GetChild("Color");
            float r,g,b,a;
            clearColor.ReadFloat("R", r);
            clearColor.ReadFloat("G", g);
            clearColor.ReadFloat("B", b);
            clearColor.ReadFloat("A", a);
            std::cout << r << " " << g << " " << b << " " << a << std::endl;
            SetClearColor(r, g, b, a);
            unsigned int cameraID;
            renderer.ReadUint("CurrentCamera", cameraID);
            CurrentCamera = cameraID;
            std::cout << "DESERIALIZED RENDERER WITH CAMERA ID " << CurrentCamera << std::endl;
        }

        void Renderer::Update(std::vector<Entity*>& entities) {
            Timer renderTimer = Timer();
            VertexCount = 0;
            TriCount = 0;
            LineCount = 0;
            TextureSwapCount = 0;
            BlendModeSwapCount = 0;
            RenderTime = 0;

            if(CurrentCamera != 0) {
                float aspectRatio = static_cast<float>(ForLease->GameWindow->GetXResolution()) / ForLease->GameWindow->GetYResolution();
                try{
                    Entity* cameraEntity = Owner.GetEntityByID(CurrentCamera, true);
                    Components::Transform* transform = cameraEntity->GetComponent<Components::Transform>(true);
                    Components::Camera* camera = cameraEntity->GetComponent<Components::Camera>(true);
                    SetProjection(transform->Position, camera->Size * aspectRatio, camera->Size, camera->Near, camera->Far, transform->Rotation);
                }
                catch(Exception* e) {
                    std::cout << e->GetInfo();
                    char c;
                    std::cin >> c;
                }

                //std::cout << "Set camera" << std::endl;
            }
            else {
                Projection = Matrix();
                //std::cout << "No camera, setting to default" << std::cout;
            }

            for(unsigned int i = 0; i < entities.size(); ++i) {
                Entity* entity = entities[i];
                if(entity->HasComponent(ComponentType::Sprite)) {
                    Components::Sprite* sprite = entity->GetComponent<Components::Sprite>();
                    Components::Transform* transform = entity->GetComponent<Components::Transform>();
                    SetModelView(transform);
                    DrawSprite(sprite);
                }
                if(entity->HasComponent(ComponentType::Model)) {
                    Components::Model* model = entity->GetComponent<Components::Model>();
                    if(model->Visible) {
                        Components::Transform* transform = entity->GetComponent<Components::Transform>();
                        if(model->ModelMesh.compare("")) {
                            Mesh* mesh = ForLease->Resources.GetMesh(model->ModelMesh);
                            SetBlendMode(model->BlendingMode);
                            Point o;
                            ModelView = Matrix::Translation(transform->Position) *
                                        Matrix::Translation(mesh->GetCenter()) *
                                        Matrix::RotationRad(transform->Rotation) * Matrix::Scale(transform->ScaleX * (model->FlipY? -1 : 1), transform->ScaleY * (model->FlipX? -1 : 1)) *
                                        Matrix::Translation(o - mesh->GetCenter());
                            DrawMesh(mesh, model->DrawEdges, model->DrawVertices, model->GetAnimation(), model->GetFrame(), model->GetFrameTime() * model->FrameRate - model->GetFrame());
                        }
                    }
                }
                if(entity->HasComponent(ComponentType::SpriteText)) {
                    Components::Transform* transform = entity->GetComponent<Components::Transform>();
                    Components::SpriteText* spriteText = entity->GetComponent<Components::SpriteText>();
                    SetBlendMode(BlendMode::ALPHA);
                    SetDrawingColor(spriteText->TextColor);
                    DrawSpriteText(spriteText, transform->Position, transform->ScaleX, transform->ScaleY, 0);
                }
            }
            glFinish();
            SetBlendMode(BlendMode::NONE);
            SetTexture(NULL);
            RenderTime = renderTimer.GetTime();
        }

        void Renderer::SetCamera(const Entity& camera) {
            CurrentCamera = camera.GetID();
        }

        void Renderer::SetCamera(unsigned long camera) {
            CurrentCamera = camera;
        }

        long Renderer::GetCameraID() {
            return CurrentCamera;
        }

        void Renderer::SetClearColor(float r, float g, float b, float a) {
            ClearColor.SetAll(r, g, b, a);
            glClearColor(r, g, b, a);
        }

        void Renderer::SetClearColor(const Color& color) {
            ClearColor = color;
            glClearColor(color.GetR(), color.GetG(), color.GetB(), color.GetA());
        }

        void Renderer::SetDrawingColor(const Color& color) {
            DrawColor = color;
            glColor4f(color.GetR(), color.GetG(), color.GetB(), color.GetA());
        }

        void Renderer::SetDrawingColor(float r, float g, float b, float a) {
            DrawColor.SetAll(r, g, b, a);
            glColor4f(r, g, b, a);
        }

        void Renderer::SetDebugPointSize(float size) {
            DebugPointSize = size;
        }

        void Renderer::SetDebugLineWidth(float width) {
            DebugLineWidth = width;
        }

        void Renderer::SetBlendMode(BlendMode mode) {
            if(mode != BlendingMode) {
                BlendingMode = mode;
                switch(mode) {
                    case BlendMode::NONE:
                        glBlendFunc(GL_ONE, GL_ZERO);
                        break;
                    case BlendMode::ALPHA:
                        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                        break;
                    case BlendMode::ADDITIVE:
                        glBlendFunc(GL_ONE, GL_ONE);
                        break;
                    case BlendMode::MULTIPLY:
                        glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
                        break;
                }
                ++BlendModeSwapCount;
            }
        }

        void Renderer::DrawSprite(Components::Sprite* sprite) {
            //std::cout << "Drawing sprite" << std::endl;
            SetBlendMode(sprite->BlendingMode);
            SetDrawingColor(sprite->SpriteColor);
            DrawTextureRegion(sprite->GetCurrentRegion());
        }

        // Currently a little weird, doesn't support scaling or rotation
        void Renderer::DrawSpriteText(Components::SpriteText* spriteText, const Point& position, float scaleX, float scaleY, float rotation) {
            //SetModelView(position, scaleX, scaleY, rotation);
            SetBlendMode(BlendMode::ALPHA);
            std::string text = spriteText->Text;
            Font* font = ForLease->Resources.GetFont(spriteText->GetFont());
            if(!font) {
                std::cout << "Font " << spriteText->GetFont() << " not loaded" << std::endl;
                return;
            }
            float xMargin = position[0];
            Point currentDrawingLoc(position[0], position[1] - font->Base);
            for(unsigned int i = 0; i < text.length(); ++i) {
                char currentLetter = text.at(i);
                if(currentLetter == '\n') {
                    currentDrawingLoc[0] = xMargin;
                    currentDrawingLoc[1] -= font->LineHeight * scaleY;
                } else {
                    Glyph currentGlyph = font->GetGlyph(currentLetter);
                    Point glyphDrawingLoc;
                    glyphDrawingLoc[0] = currentDrawingLoc[0] + currentGlyph.Offset[0] + currentGlyph.Width / 2 * scaleX;
                    glyphDrawingLoc[1] = currentDrawingLoc[1] - currentGlyph.Offset[1] - currentGlyph.Height / 2 * scaleY;
                    SetModelView(glyphDrawingLoc, scaleX, scaleY, rotation);
                    DrawTextureRegion(&currentGlyph.Region);
                    //DrawRectangle(glyphDrawingLoc, currentGlyph.Region.GetWidth(), currentGlyph.Region.GetHeight());
                    //DrawTextureRegion(Point(shiftedLoc[0] - currentGlyph.Offset[0] * 3, shiftedLoc[1] - currentGlyph.Offset[1] * 0.5f), &currentGlyph.Region, scaleX, scaleY, rotation);
                    //DrawRectangle(Point(shiftedLoc[0] - currentGlyph.Offset[0] * 3, shiftedLoc[1] - currentGlyph.Offset[1] * 0.5f), currentGlyph.Region.GetWidth(), currentGlyph.Region.GetHeight());
                    currentDrawingLoc[0] += currentGlyph.XAdvance * scaleX;
                    //shiftedLoc[0] += currentGlyph.XAdvance;
                    TriCount += 2;
                }
            }
        }

        void Renderer::DrawModel(Components::Model* model) {
            SetBlendMode(model->BlendingMode);
            SetTexture(ForLease->Resources.GetTexture(model->ModelTexture));
            Mesh* mesh = ForLease->Resources.GetMesh(model->ModelMesh);
            if(mesh)
                DrawMesh(mesh);
        }

        void Renderer::SetModelView(Components::Transform* transform) {
            SetModelView(transform->Position, transform->ScaleX, transform->ScaleY, transform->Rotation);
        }

        void Renderer::DrawRectangle(const Point& position, float width, float height, float rotation) {
            SetBlendMode(BlendMode::NONE);
            SetTexture(NULL);
            SetModelView(position, width / 2, height / 2, rotation);
            Point vertices[] = {Point(1, 1), Point(-1, 1), Point(-1, -1), Point(1, -1), Point(1, 1)};
            ModelToScreen(vertices, vertices, 5);
            glBegin(GL_LINE_STRIP);
                for(int i = 0; i < 5; ++i) {
                    glVertex2f(vertices[i][0], vertices[i][1]);
                }
            glEnd();
        }

        void Renderer::DrawRectangleFilled(const Point& position, float width, float height, float rotation) {
            SetBlendMode(BlendMode::NONE);
            SetTexture(NULL);
            SetModelView(position, width / 2, height / 2, rotation);
            Point vertices[] = {Point(1, 1), Point(-1, 1), Point(-1, -1), Point(1, -1)};
            ModelToScreen(vertices, vertices, 4);
            glBegin(GL_QUADS);
                for(int i = 0; i < 4; ++i) {
                    glVertex2f(vertices[i][0], vertices[i][1]);
                }
            glEnd();
        }

        void Renderer::DrawLine(const Point& start, const Point& end) {
            //ModelView = Matrix();
            SetBlendMode(BlendMode::NONE);
            Point vertices[] = {Projection * start, Projection * end};
            glLineWidth(DebugLineWidth);
            glBegin(GL_LINES);
                glVertex2f(vertices[0][0], vertices[0][1]);
                glVertex2f(vertices[1][0], vertices[1][1]);
            glEnd();
        }

        void Renderer::DrawLine(const Point& start, const Vector& displacement) {
            DrawLine(start, start + displacement);
        }

        void Renderer::DrawArrow(const Point& start, const Point& end) {
            //ModelView = Matrix();
            Components::Camera* camera = Owner.GetEntityByID(CurrentCamera)->GetComponent<Components::Camera>();
            float headLength = camera->Size / 40;
            float headAngle = 3.1415927 / 6;
            Vector direction = start - end;
            direction.Normalize();
            Point vertices[6];
            vertices[0] = start;
            vertices[1] = end;
            vertices[2] = end + Vector::Scale(Vector::Rotate(direction, headAngle), headLength);
            vertices[3] = end + Vector::Scale(Vector::Rotate(direction, -headAngle), headLength);
            vertices[4] = end;

            for(int i = 0; i < 5; ++i) {
                vertices[i] = Projection * vertices[i];
            }

            glLineWidth(2);
            glBegin(GL_LINE_STRIP);
                for(int i = 0; i < 5; ++i) {
                    glVertex2f(vertices[i][0], vertices[i][1]);
                }
            glEnd();
        }

        void Renderer::DrawArrow(const Point& start, const Vector& end) {
            DrawArrow(start, start + end);
        }

        void Renderer::DrawPoint(const Point& point) {
            Point transformed = Projection * point;
            //SetBlendMode(BlendMode::ALPHA);
            glPointSize(DebugPointSize);
            glBegin(GL_POINTS);
                glVertex2f(transformed[0], transformed[1]);
            glEnd();
        }

        unsigned int Renderer::GetVertexCount() { return VertexCount; }

        unsigned int Renderer::GetTriCount() { return TriCount; }

        unsigned int Renderer::GetLineCount() { return LineCount; }

        unsigned int Renderer::GetTextureSwapCount() { return TextureSwapCount; }

        unsigned int Renderer::GetBlendModeSwapCount() { return BlendModeSwapCount; }

        float Renderer::GetRenderTime() { return RenderTime; }

        Point Renderer::ScreenToWorld(const Point& point) {
            State& state = ForLease->GameStateManager().CurrentState();
            Entity* cameraEntity = state.GetEntityByID(CurrentCamera);
            return cameraEntity->GetComponent<Components::Camera>()->ScreenToWorld(point);
        }

        Point Renderer::WorldToScreen(const Point& point) {
            Entity* cameraEntity = Owner.GetEntityByID(CurrentCamera);
            return cameraEntity->GetComponent<Components::Camera>()->WorldToScreen(point);
        }


        ////////////////////Private Functions////////////////////

        // This should never get called
        void Renderer::DrawTexture(Texture*) {
            throw(new GraphicsException("Why the hell are you calling this?"));
        }

        void Renderer::DrawTextureRegion(TextureRegion* region) {
            //std::cout << "Drawing texture region" << std::endl;
            Texture* texture = ForLease->Resources.GetTexture(region->GetTexture());
            float halfWidth = region->GetWidth() / 2;
            float halfHeight = region->GetHeight() / 2;
            Point vertices[] = {Point(halfWidth, halfHeight), Point(-halfWidth, halfHeight),
                                Point(-halfWidth, -halfHeight), Point(halfWidth, - halfHeight)};
            for(int i = 0; i < 4; ++i) {
                ModelToScreen(vertices[i], vertices[i]);
            }
            SetTexture(texture);
            glBegin(GL_QUADS);
                for(int i = 0; i < 4; ++i) {
                    glTexCoord2f(region->GetUV()[i][0], region->GetUV()[i][1]);
                    glVertex2f(vertices[i][0], vertices[i][1]);
                }
            glEnd();
            TriCount += 2;
        }

        void Renderer::DrawMesh(Mesh* mesh, bool drawEdges, bool drawVertices, std::string animationName, unsigned int frame, float t) {
            Point* transformed = new Point[mesh->GetVertexCount()];
            if(animationName.compare("") != 0) {
                MeshAnimation* animation = ForLease->Resources.GetMeshAnimation(animationName);
                for(int i = 0; i < mesh->GetVertexCount(); ++i) {
                    ModelToScreen(animation->InterpolateVertex(frame, i, t), transformed[i]);
                }
            }
            else {
                for(int i = 0; i < mesh->GetVertexCount(); ++i) {
                    ModelToScreen(mesh->GetVertex(i), transformed[i]);
                }
            }

            glBegin(GL_TRIANGLES);
                for(int i = 0; i < mesh->GetFaceCount(); ++i) {
                    IndexedFace face = mesh->GetIndexedFace(i);
                    SetDrawingColor(mesh->GetFaceColor(i));
                    for(int j = 0; j < 3; ++j) {
                        glVertex2f(transformed[face.Indices[j]][0], transformed[face.Indices[j]][1]);
                    }
                    ++TriCount;
                }
            glEnd();

            if(drawEdges) {
                glColor3f(0, 0, 0);
                glLineWidth(2);
                glBegin(GL_LINES);
                    for(int i = 0; i < mesh->GetEdgeCount(); ++i) {
                        IndexedEdge edge = mesh->GetIndexedEdge(i);
                        glVertex2f(transformed[edge.Indices[0]][0], transformed[edge.Indices[0]][1]);
                        glVertex2f(transformed[edge.Indices[1]][0], transformed[edge.Indices[1]][1]);
                    }
                    ++LineCount;
                glEnd();
            }

            if(drawVertices) {
                glColor3f(1, 1, 1);
                glPointSize(2);
                glBegin(GL_POINTS);
                    for(int i = 0; i < mesh->GetVertexCount(); ++i) {
                        glVertex2f(transformed[i][0], transformed[i][1]);
                    }
                glEnd();
            }

			delete[] transformed;
        }

        void Renderer::SetTexture(Texture* texture) {
            if(texture == NULL && CurrentTexture != 0) {
                CurrentTexture = 0;
                glBindTexture(GL_TEXTURE_2D, 0);
                glDisable(GL_TEXTURE_2D);
                ++TextureSwapCount;
            }
            else if(texture && texture->GetID() != CurrentTexture) {
                if(CurrentTexture == 0) {
                    glEnable(GL_TEXTURE_2D);
                }
                CurrentTexture = texture->GetID();
                glBindTexture(GL_TEXTURE_2D, CurrentTexture);
                ++TextureSwapCount;
            }
        }

        void Renderer::SetProjection(const Point& position, float width, float height, float near, float far, float rotation) {
            Matrix trans = Matrix::Translation(Point(-position[0], -position[1]));
            Matrix scale = Matrix::Scale(2 / width, 2 / height);
            Matrix rot = Matrix::RotationRad(-rotation);

            Projection = scale * rot * trans;
        }

        void Renderer::SetModelView(const Point& position, float scaleX, float scaleY, float rotation) {
            Matrix trans = Matrix::Translation(position);
            Matrix rot = Matrix::RotationRad(rotation);
            Matrix scale = Matrix::Scale(scaleX, scaleY);

            ModelView = trans * rot * scale;
        }

        void Renderer::ModelToScreen(const Point& source, Point& dest) {
            dest = Point(Projection * ModelView * source);
        }

        void Renderer::ModelToScreen(const Point* source, Point* dest, int count) {
            for(int i = 0; i < count; ++i) {
                dest[i] = Point(Projection * ModelView * source[i]);
            }
        }
    }
}
