/*!
    \file   Renderer.cpp
    \author Christopher Hudson
    \date   09/12/15

    \brief
        Defines a class to render things to the screen, internals will change a lot but functions should remain the same

    \see Renderer.h
*/
#include <iostream>
//#include <SDL_opengl.h>
//#include <GL\GLU.h>
#include <GL\GL.h>
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
//#include "ComponentSpriteText.h"

namespace ForLeaseEngine {
    namespace LevelComponents {
        Renderer::Renderer(State& owner) : LevelComponent(owner) {
            DrawColor = Color(1, 1, 1, 1);
            ClearColor = Color(0, 0, 0, 1);
            CurrentCamera = NULL;
            BlendingMode = BlendMode::NONE;
            CurrentTexture = 0;
            VertexCount = 0;
            TriCount = 0;
            LineCount = 0;
            TextureSwapCount = 0;
            BlendModeSwapCount = 0;
            RenderTime = 0;
        }

        void Renderer::Update(std::vector<Entity*>& entities) {
            Timer renderTimer = Timer();
            VertexCount = 0;
            TriCount = 0;
            LineCount = 0;
            TextureSwapCount = 0;
            BlendModeSwapCount = 0;
            RenderTime = 0;

            if(*CurrentCamera != NULL) {
                float aspectRatio = ForLease->GameWindow->GetYResolution() / ForLease->GameWindow->GetXResolution();
                Components::Transform* transform = (*CurrentCamera)->GetComponent<Components::Transform>();
                Components::Camera* camera = (*CurrentCamera)->GetComponent<Components::Camera>();
                SetProjection(transform->Position, camera->Size, camera->Size * aspectRatio, camera->Near, camera->Far, transform->Rotation);

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
                    Components::Transform* transform = entity->GetComponent<Components::Transform>();
                    Mesh* mesh = ForLease->Resources.GetMesh(model->ModelMesh);
                    SetBlendMode(model->BlendingMode);
                    Point o;
                    ModelView = Matrix::Translation(transform->Position) *
                                Matrix::Translation(mesh->GetCenter()) *
                                Matrix::RotationRad(transform->Rotation) * Matrix::Scale(transform->ScaleX, transform->ScaleY) *
                                Matrix::Translation(o - mesh->GetCenter());
                    DrawMesh(mesh);
                }
                if(entity->HasComponent(ComponentType::SpriteText)) {
                    Components::Transform* transform = entity->GetComponent<Components::Transform>();
                    Components::SpriteText* spriteText = entity->GetComponent<Components::SpriteText>();
                    SetBlendMode(BlendMode::ALPHA);
                    SetDrawingColor(spriteText->TextColor);
                    DrawSpriteText(spriteText, transform->Position, 1, 1, 0);
                }
            }
            glFinish();
            RenderTime = renderTimer.GetTime();
        }

        void Renderer::SetCamera(Entity** camera) {
            CurrentCamera = camera;
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
            std::string text = spriteText->Text;
            Font* font = ForLease->Resources.GetFont(spriteText->GetFont());
            float xMargin = position[0];
            Point currentDrawingLoc(position[0], position[1] - font->Base);
            for(unsigned int i = 0; i < text.length(); ++i) {
                char currentLetter = text.at(i);
                if(currentLetter == '\n') {
                    currentDrawingLoc[0] = xMargin;
                    currentDrawingLoc[1] -= font->LineHeight;
                } else {
                    Glyph currentGlyph = font->GetGlyph(currentLetter);
                    Point glyphDrawingLoc;
                    glyphDrawingLoc[0] = currentDrawingLoc[0] + currentGlyph.Offset[0] + currentGlyph.Width / 2;
                    glyphDrawingLoc[1] = currentDrawingLoc[1] - currentGlyph.Offset[1] - currentGlyph.Height / 2;
                    SetModelView(glyphDrawingLoc, scaleX, scaleY, rotation);
                    DrawTextureRegion(&currentGlyph.Region);
                    //DrawRectangle(glyphDrawingLoc, currentGlyph.Region.GetWidth(), currentGlyph.Region.GetHeight());
                    //DrawTextureRegion(Point(shiftedLoc[0] - currentGlyph.Offset[0] * 3, shiftedLoc[1] - currentGlyph.Offset[1] * 0.5f), &currentGlyph.Region, scaleX, scaleY, rotation);
                    //DrawRectangle(Point(shiftedLoc[0] - currentGlyph.Offset[0] * 3, shiftedLoc[1] - currentGlyph.Offset[1] * 0.5f), currentGlyph.Region.GetWidth(), currentGlyph.Region.GetHeight());
                    currentDrawingLoc[0] += currentGlyph.XAdvance;
                    //shiftedLoc[0] += currentGlyph.XAdvance;
                    TriCount += 2;
                }
            }
        }

        void Renderer::DrawModel(Components::Model* model) {
            SetBlendMode(model->BlendingMode);
            SetTexture(ForLease->Resources.GetTexture(model->ModelTexture));
            DrawMesh(ForLease->Resources.GetMesh(model->ModelMesh));
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
            float headLength = 10.0f;
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

            glBegin(GL_LINE_STRIP);
                for(int i = 0; i < 5; ++i) {
                    glVertex2f(vertices[i][0], vertices[i][1]);
                }
            glEnd();
        }

        void Renderer::DrawArrow(const Point& start, const Vector& end) {
            DrawArrow(start, start + end);
        }

        unsigned int Renderer::GetVertexCount() { return VertexCount; }

        unsigned int Renderer::GetTriCount() { return TriCount; }

        unsigned int Renderer::GetLineCount() { return LineCount; }

        unsigned int Renderer::GetTextureSwapCount() { return TextureSwapCount; }

        unsigned int Renderer::GetBlendModeSwapCount() { return BlendModeSwapCount; }

        float Renderer::GetRenderTime() { return RenderTime; }


        ////////////////////Private Functions////////////////////

        // This should never get called
        void Renderer::DrawTexture(Texture* texture) {
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

        void Renderer::DrawMesh(Mesh* mesh) {
            Point transformed[mesh->GetVertexCount()];
            for(int i = 0; i < mesh->GetVertexCount(); ++i) {
                ModelToScreen(mesh->GetVertex(i), transformed[i]);
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
