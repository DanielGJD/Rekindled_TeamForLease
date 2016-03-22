/*!
    \file   Renderer.cpp
    \author Christopher Hudson
    \date   09/12/15

    \brief
        Defines a class to render things to the screen, internals will change a lot but functions should remain the same

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved

    \see Renderer.h
*/
#include <iostream>
#include <GL/glew.h>
#include "Vector.h"
#include "Matrix.h"
#include "LevelComponentRenderer.h"
#include "Entity.h"
#include "GraphicsException.h"
#include "State.h"
#include "ComponentSprite.h"
#include "ComponentModel.h"
#include "ComponentSpriteText.h"
#include "ComponentParallax.h"
#include "Color.h"
#include "Component.h"
#include "Timer.h"
#include "Engine.h"
#include "Exception.h"
#include "Serializable.h"
#include "Serialize.h"
//#include "json-forwards.h"
#include "MeshAnimation.h"
#include <map>
#include <vector>
#include <list>
#include <thread>
#include <cmath>
#define PARTICLE_THREADS 2

GLuint index;

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
            OverlayColor = Color(1, 1, 1, 0);
            OutlineColor = Color(1, 1, 1, 1);
            OutlineWidth = 0;

            index = glGenLists(1);
            //GLuint framebuffer;
            //glGenFramebuffers(1, &framebuffer);

            // Generate texture for lighting
            glGenTextures(1, &LightTexture);
            glBindTexture(GL_TEXTURE_2D, LightTexture);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ForLease->GameWindow->GetXResolution(), ForLease->GameWindow->GetYResolution(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

            glGenTextures(1, &UITexture);
            glBindTexture(GL_TEXTURE_2D, UITexture);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ForLease->GameWindow->GetXResolution(), ForLease->GameWindow->GetYResolution(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

            glBindTexture(GL_TEXTURE_2D, 0);

            glGenFramebuffers(1, &LightFBO);
            glBindFramebuffer(GL_FRAMEBUFFER, LightFBO);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, LightTexture, 0);
            if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
                std::cout << "ERROR CREATING LIGHTING FRAMEBUFFER" << std::endl;
            }

            glGenFramebuffers(1, &UIFBO);
            glBindFramebuffer(GL_FRAMEBUFFER, UIFBO);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, UITexture, 0);
            if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
                std::cout << "ERROR CREATING UI FRAMEBUFFER" << std::endl;
            }

            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }

        Renderer::~Renderer() {
            glDeleteTextures(1, &LightTexture);
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
            //std::cout << r << " " << g << " " << b << " " << a << std::endl;
            SetClearColor(r, g, b, a);
            unsigned int cameraID;
            renderer.ReadUint("CurrentCamera", cameraID);
            CurrentCamera = cameraID;
            //std::cout << "DESERIALIZED RENDERER WITH CAMERA ID " << CurrentCamera << std::endl;
        }

        void Renderer::Update(std::vector<Entity*>& entities) {
            //glClear(GL_COLOR_BUFFER_BIT);
            Timer renderTimer = Timer();
            VertexCount = 0;
            TriCount = 0;
            LineCount = 0;
            TextureSwapCount = 0;
            BlendModeSwapCount = 0;
            RenderTime = 0;

            LevelComponents::Light* lighting = Owner.GetLevelComponent<LevelComponents::Light>();
            if(lighting) {
                glBindFramebuffer(GL_FRAMEBUFFER, LightFBO);
                SetBlendMode(BlendMode::NONE);
                SetTexture(NULL);
                //glPushAttrib(GL_VIEWPORT_BIT);
                //glViewport(0, 0, ForLease->GameWindow->GetXResolution(), ForLease->GameWindow->GetYResolution());
                //std::cout << lighting->AmbientLight.GetR() << "|" << lighting->AmbientLight.GetG() <<
                SetDrawingColor(lighting->AmbientLight);
                //SetDrawingColor(1, 1, 0, 1);
                glBegin(GL_QUADS);
                    glVertex2f(-1, -1);
                    glVertex2f(-1, 1);
                    glVertex2f(1, 1);
                    glVertex2f(1, -1);
                glEnd();
                //glClearColor(lighting->AmbientLight.GetR(), lighting->AmbientLight.GetG(), lighting->AmbientLight.GetB(), lighting->AmbientLight.GetA());
                //glClearColor(1, 1, 1, 1);
                //glClear(GL_COLOR_BUFFER_BIT);
                //glBindFramebuffer(GL_FRAMEBUFFER, 0);
                //glClearColor(ClearColor.GetR(), ClearColor.GetG() ,ClearColor.GetB() ,ClearColor.GetA());
                //glPopAttrib();
                //glBindFramebuffer(GL_FRAMEBUFFER, 0);
            }

            glBindFramebuffer(GL_FRAMEBUFFER, UIFBO);
            glClearColor(0, 0, 0, 0);
            glClear(GL_COLOR_BUFFER_BIT);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
//            glFlush();

            if(CurrentCamera != 0) {
                float aspectRatio = static_cast<float>(ForLease->GameWindow->GetXResolution()) / ForLease->GameWindow->GetYResolution();
                try{
                    Entity* cameraEntity = Owner.GetEntityByID(CurrentCamera, true);
                    Components::Transform* transform = cameraEntity->GetComponent<Components::Transform>(true);
                    Components::Camera* camera = cameraEntity->GetComponent<Components::Camera>(true);
                    SetProjection(transform->Position, camera->Size * aspectRatio, camera->Size, camera->Near, camera->Far, transform->Rotation);
                }
                catch(Exception& e) {
                    std::cout << e.GetInfo();
                    char c;
                    std::cin >> c;
                }

                //std::cout << "Set camera" << std::endl;
            }
            else {
                Projection = Matrix();
                //std::cout << "No camera, setting to default" << std::endl;
            }

            std::map<int, std::vector<Entity*>> sorted;
            for(unsigned int i = 0; i < entities.size(); ++i) {
                Components::Transform* trans = entities[i]->GetComponent<Components::Transform>();
                if (!trans) continue;
                std::map<int, std::vector<Entity*>>::iterator it = sorted.find(trans->ZOrder);
                if(it == sorted.end()) {
                    sorted.insert(std::make_pair(trans->ZOrder, std::vector<Entity*>()));
                    it = sorted.find(trans->ZOrder);
                }

                (*it).second.push_back(entities[i]);
            }

            Vector cameraShift;
            int cameraZ = 0;
            if(CurrentCamera != 0) {
                Components::Transform* cameraTrans = Owner.GetEntityByID(CurrentCamera, true)->GetComponent<Components::Transform>();
                cameraShift = cameraTrans->Position - Point();
                cameraZ = cameraTrans->ZOrder;
            }
            for(std::map<int, std::vector<Entity*>>::const_iterator it = sorted.begin(); it != sorted.end(); ++it) {
                for(unsigned int i = 0; i < (*it).second.size(); ++i) {
                    Entity* entity = (*it).second[i];

                    float parallaxAmount = 0;
                    Components::Parallax* parallax = entity->GetComponent<Components::Parallax>();
                    Components::Transform* trans = entity->GetComponent<Components::Transform>();

                    if(trans->UILayer) {
                        glBindFramebuffer(GL_FRAMEBUFFER, UIFBO);
                    }
                    else {
                        glBindFramebuffer(GL_FRAMEBUFFER, 0);
                    }

                    if(parallax && parallax->Active && trans->ZOrder != cameraZ) {
                        parallaxAmount = 1.0 - 1.0 / abs(trans->ZOrder - cameraZ);
                        //std::cout << parallaxAmount << std::endl;
                    }
                    else {
                        parallaxAmount = 1;
                    }


                    if(entity->HasComponent(ComponentType::VisionCone)) {
                        Components::VisionCone* visionCone = entity->GetComponent<Components::VisionCone>();
                        if(visionCone->Visible) {
                            //Components::Transform* trans = entity->GetComponent<Components::Transform>();
                            glBindFramebuffer(GL_FRAMEBUFFER, UIFBO);
                            SetTexture(NULL);
                            SetBlendMode(BlendMode::ALPHA);
                            ModelView = Matrix::Translation(trans->Position);
                            //SetBlendMode(BlendMode::ALPHA);
                            DrawMesh(visionCone->GetVisionMesh(), visionCone->DrawOutline, false);
                            if(!trans->UILayer) {
                                glBindFramebuffer(GL_FRAMEBUFFER, 0);
                            }
                        }
                    }
                    if(entity->HasComponent(ComponentType::Sprite)) {
                        Components::Sprite* sprite = entity->GetComponent<Components::Sprite>();
                        if(sprite->Visible) {
                            //Components::Transform* transform = entity->GetComponent<Components::Transform>();
                            SetModelView(trans->Position + cameraShift * parallaxAmount, trans->ScaleX, trans->ScaleY, trans->Rotation);
                            DrawSprite(sprite);
                        }
                    }
                    if(entity->HasComponent(ComponentType::Model)) {
                        Components::Model* model = entity->GetComponent<Components::Model>();
                        if(model->Visible) {
                            Components::Transform* transform = entity->GetComponent<Components::Transform>();
                            if(model->ModelMesh.compare("")) {
                                Mesh* mesh = ForLease->Resources.GetMesh(model->ModelMesh);
                                SetBlendMode(model->BlendingMode);
                                SetTexture(NULL);
                                Point o;
                                ModelView = Matrix::Translation(transform->Position) *
                                            Matrix::Translation(mesh->GetCenter()) *
                                            Matrix::RotationRad(transform->Rotation) * Matrix::Scale(transform->ScaleX * (model->FlipY? -1 : 1), transform->ScaleY * (model->FlipX? -1 : 1)) *
                                            Matrix::Translation(o - mesh->GetCenter());
                                //std::cout << "Model Color (" << model->ModelColor.GetR() << "," << model->ModelColor.GetG() << "," << model->ModelColor.GetB() << "," << model->ModelColor.GetA() << ")" << std::endl;
                                DrawMesh(mesh, model->DrawEdges, model->DrawVertices, model->GetAnimation(), model->GetFrame(), model->GetFrameTime() * model->FrameRate - model->GetFrame(), model->ModelColor);
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
                    if(entity->HasComponent(ComponentType::ParticleSystem)) {
                        glBindFramebuffer(GL_FRAMEBUFFER, 0);
                        Components::ParticleSystem* pSystem = entity->GetComponent<Components::ParticleSystem>();
                        DrawParticleSystem(pSystem);
                    }
                    if(entity->HasComponent(ComponentType::Light)) {
                        Components::Light* light = entity->GetComponent<Components::Light>();
                        ModelView = Matrix::Translation(trans->Position);

                        ////////////////////////////////////////////////////////////////////////////////////
//                        SetDrawingColor(1, 0, 0, 0);
//                        Mesh* lightMesh = light->GetLightMesh();
//                        DrawArrow(trans->Position, light->Direction);
//                        DrawArrow(trans->Position, Point(ModelView * lightMesh->GetVertex(1)));
//                        DrawArrow(trans->Position, Point(ModelView * lightMesh->GetVertex(lightMesh->GetVertexCount() - 1)));
//                        SetDrawingColor(0, 1, 0, 1);
//                        for(int j = 4; j < lightMesh->GetVertexCount() - 3; ++j) {
//                            DrawArrow(trans->Position, Point(ModelView * lightMesh->GetVertex(j)));
//                        }
                        ///////////////////////////////////////////////////////////////////////////////////////

                        SetBlendMode(light->LightMode);
                        if(light->LightTexture.empty()) {
                            SetTexture(NULL);
                        }
                        else {
                            SetTexture(ForLease->Resources.GetTexture(light->LightTexture));
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
                            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
                        }

                        glBindFramebuffer(GL_FRAMEBUFFER, LightFBO);
                        DrawMesh(light->GetLightMesh(), false, true);
                        if(trans->UILayer) {
                            glBindFramebuffer(GL_FRAMEBUFFER, UIFBO);
                        }
                        else {
                            glBindFramebuffer(GL_FRAMEBUFFER, 0);
                        }
                    }
                }
            }

            GLboolean texEnable = glIsEnabled(GL_TEXTURE_2D);
            glEnable(GL_TEXTURE_2D);

            if(lighting) {
                SetBlendMode(BlendMode::MULTIPLY);
                SetDrawingColor(1, 1, 1);
                //glBindFramebuffer(GL_FRAMEBUFFER, LightFBO);
                glBindTexture(GL_TEXTURE_2D, LightTexture);
                glBegin(GL_QUADS);
                    glTexCoord2f(0, 0); glVertex2f(-1, -1);
                    glTexCoord2f(0, 1); glVertex2f(-1, 1);
                    glTexCoord2f(1, 1); glVertex2f(1, 1);
                    glTexCoord2f(1, 0); glVertex2f(1, -1);
                glEnd();
                //glBindFramebuffer(GL_FRAMEBUFFER, 0);
            }

            SetBlendMode(BlendMode::ALPHA);
            SetDrawingColor(1, 1, 1, 1);
            glBindTexture(GL_TEXTURE_2D, UITexture);
            glBegin(GL_QUADS);
                glTexCoord2f(0, 0); glVertex2f(-1, -1);
                glTexCoord2f(0, 1); glVertex2f(-1, 1);
                glTexCoord2f(1, 1); glVertex2f(1, 1);
                glTexCoord2f(1, 0); glVertex2f(1, -1);
            glEnd();

            if(texEnable == GL_FALSE) {
                glDisable(GL_TEXTURE_2D);
            }
            glBindTexture(GL_TEXTURE_2D, CurrentTexture);

            //glBindFramebuffer(GL_FRAMEBUFFER, 0);

            Entity* cameraEntity = Owner.GetEntityByID(CurrentCamera);
            Components::Transform* cameraTrans = cameraEntity->GetComponent<Components::Transform>();
            Components::Camera* camera = cameraEntity->GetComponent<Components::Camera>();

            float height = camera->Size;
            float width = height * ForLease->GameWindow->GetXResolution() / ForLease->GameWindow->GetYResolution();
            float halfwidth = width / 2;
            float halfheight = height / 2;
            Point pos = cameraTrans->Position;

            // Draw outline
//            SetDrawingColor(OutlineColor);
//            DrawRectangleFilled(pos + Vector(0, halfheight), width, OutlineWidth);
//            DrawRectangleFilled(pos + Vector(0, -halfheight), width, OutlineWidth);
//            DrawRectangleFilled(pos + Vector(halfwidth, 0), OutlineWidth, height);
//            DrawRectangleFilled(pos + Vector(-halfwidth, 0), OutlineWidth, height);

            // Draw overlay
            SetDrawingColor(OverlayColor);

            DrawRectangleFilled(cameraTrans->Position, width, height, 0, BlendMode::ALPHA);
        }

        void Renderer::SetCamera(const Entity& camera) {
            CurrentCamera = camera.GetID();
            //std::cout << "Set camera to " << CurrentCamera << std::endl;
        }

        void Renderer::SetCamera(unsigned long camera) {
            CurrentCamera = camera;
            //std::cout << "Set Camera to " << CurrentCamera << std::endl;
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

        void Renderer::SetOverlayColor(float r, float g, float b, float a) {
            OverlayColor = Color(r, g, b, a);
        }

        void Renderer::SetOutline(float r, float g, float b, float a, float width) {
            OutlineColor = Color(r, g, b, a);
            OutlineWidth = width;
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
                        glBlendFunc(GL_SRC_ALPHA, GL_ONE);
                        break;
                    case BlendMode::MULTIPLY:
                        glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_SRC_ALPHA);
                        break;
                    case BlendMode::LIGHT:
                        glBlendFunc(GL_DST_COLOR, GL_SRC_ALPHA);
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
            if(spriteText->Text.size() == 0)
                return;

            Font* font = ForLease->Resources.GetFont(spriteText->GetFont());
            if(!font) {
                std::cout << "Font " << spriteText->GetFont() << " not loaded" << std::endl;
                return;
            }

            SetModelView(position, scaleX / font->LineHeight, scaleY / font->LineHeight, rotation);
            SetBlendMode(BlendMode::ALPHA);
            std::string text = spriteText->Text;

            std::vector<Point> verts = std::vector<Point>(text.size() * 4);
            Point currentDrawingLoc(0, 0);

//            float xMargin = position[0];
//            Point currentDrawingLoc(position[0], position[1] - font->Base);
            for(unsigned int i = 0; i < text.length(); ++i) {
                char currentLetter = text.at(i);
                if(currentLetter == '\n') {
                    currentDrawingLoc[0] = 0;
                    currentDrawingLoc[1] -= font->LineHeight;
                } else {
                    Glyph currentGlyph = font->GetGlyph(currentLetter);
                    Point glyphDrawingLoc;
                    glyphDrawingLoc[0] = currentDrawingLoc[0] - currentGlyph.Offset[0];
                    glyphDrawingLoc[1] = currentDrawingLoc[1] - currentGlyph.Offset[1];
                    verts[i * 4] = Point(glyphDrawingLoc[0] + currentGlyph.Width, glyphDrawingLoc[1]);
                    verts[i * 4 + 1] = Point(glyphDrawingLoc[0], glyphDrawingLoc[1]);
                    verts[i * 4 + 2] = Point(glyphDrawingLoc[0], glyphDrawingLoc[1] - currentGlyph.Height);
                    verts[i * 4 + 3] = Point(glyphDrawingLoc[0] + currentGlyph.Width, glyphDrawingLoc[1] - currentGlyph.Height);
                    //SetModelView(glyphDrawingLoc, scaleX, scaleY, rotation);
                    //DrawTextureRegion(&currentGlyph.Region);
                    //DrawRectangle(glyphDrawingLoc, currentGlyph.Region.GetWidth(), currentGlyph.Region.GetHeight());
                    //DrawTextureRegion(Point(shiftedLoc[0] - currentGlyph.Offset[0] * 3, shiftedLoc[1] - currentGlyph.Offset[1] * 0.5f), &currentGlyph.Region, scaleX, scaleY, rotation);
                    //DrawRectangle(Point(shiftedLoc[0] - currentGlyph.Offset[0] * 3, shiftedLoc[1] - currentGlyph.Offset[1] * 0.5f), currentGlyph.Region.GetWidth(), currentGlyph.Region.GetHeight());
                    currentDrawingLoc[0] += currentGlyph.XAdvance;
                    //shiftedLoc[0] += currentGlyph.XAdvance;
                    //TriCount += 2;
                }
            }

            //Matrix combinged = Projection * ModelView;
//            SetDrawingColor(1, 1, 1, 1);
//            SetDebugPointSize(4);

            //glBegin(GL_POINTS);
            for(int i = 0; i < verts.size(); ++i) {
                ModelToScreen(verts[i], verts[i]);
                //glVertex2f(verts[i][0], verts[i][1]);
                //std::cout << i << ":" << verts[i][0] << "," << verts[i][1] << std::endl;
            }
            //glEnd();

//            glBegin(GL_LINES);
//            glVertex2f(0, 0);
//            glVertex2f(1, 0);
//            glVertex2f(0, 0);
//            glVertex2f(0, -1);
//            glVertex2f(0, -font->LineHeight * 2 / 720.0);
//            glVertex2f(1, -font->LineHeight * 2 / 720.0);
//            glVertex2f(0, -font->LineHeight * 2 * 2 / 720.0);
//            glVertex2f(1, -font->LineHeight * 2 * 2 / 720.0);
//            glVertex2f(0, -font->LineHeight * 3 * 2 / 720.0);
//            glVertex2f(1, -font->LineHeight * 3 * 2 / 720.0);
//            glEnd();

            SetDrawingColor(spriteText->TextColor);
            SetTexture(ForLease->Resources.GetTexture(font->FontTextures[0]));
            glBegin(GL_QUADS);
            for(unsigned int i = 0; i < text.length(); ++i) {
                char currentLetter = text.at(i);
                if(currentLetter != '\n') {
                    Glyph glyph = font->GetGlyph(currentLetter);
                    glTexCoord2f(glyph.Region.GetUV()[0][0], glyph.Region.GetUV()[0][1]); glVertex2f(verts[i * 4][0], verts[i * 4][1]);
                    glTexCoord2f(glyph.Region.GetUV()[1][0], glyph.Region.GetUV()[1][1]); glVertex2f(verts[i * 4 + 1][0], verts[i * 4 + 1][1]);
                    glTexCoord2f(glyph.Region.GetUV()[2][0], glyph.Region.GetUV()[2][1]); glVertex2f(verts[i * 4 + 2][0], verts[i * 4 + 2][1]);
                    glTexCoord2f(glyph.Region.GetUV()[3][0], glyph.Region.GetUV()[3][1]); glVertex2f(verts[i * 4 + 3][0], verts[i * 4 + 3][1]);
                }
            }
            glEnd();
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

        void Renderer::DrawRectangleFilled(const Point& position, float width, float height, float rotation, BlendMode blendMode) {
            SetBlendMode(blendMode);
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
            //glBindFrameBuffer(GL_FRAMEBUFFER, UIFBO);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            SetTexture(NULL);
            SetBlendMode(BlendMode::NONE);
            glBegin(GL_LINE_STRIP);
                for(int i = 0; i < 5; ++i) {
                    glVertex2f(vertices[i][0], vertices[i][1]);
                }
            glEnd();
            //glBindFrameBuffer(GL_FRAMEBUFFER, 0);
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

        void Renderer::DrawCircle(const Point& center, float radius) {
            const float controlPointCount = 40;
            const float stepSize = 3.1415927 * 2 / controlPointCount;
            SetModelView(center, radius, radius, 0);
            Matrix combigned = Projection * ModelView;
            SetBlendMode(BlendMode::NONE);
            glBegin(GL_LINE_STRIP);
            for(int i = 0; i <= controlPointCount; ++i) {
                Point p = combigned * Point(cos(stepSize * i), sin(stepSize * i));
                glVertex2f(p[0], p[1]);
            }
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
            if(region->GetTexture().compare("") == 0) {
                return;
            }
            Texture* texture = ForLease->Resources.GetTexture(region->GetTexture());
            //float halfWidth = texture->GetWidth() / 2;
            //float halfHeight = texture->GetHeight() / 2;
            Point vertices[] = {Point(1, 1), Point(-1, 1),
                                Point(-1, -1), Point(1, - 1)};
            for(int i = 0; i < 4; ++i) {
                ModelToScreen(vertices[i], vertices[i]);
            }
            SetTexture(texture);
            glBegin(GL_QUADS);
                //for(int i = 0; i < 4; ++i) {
                //    glTexCoord2f(region->GetUV()[i][0], region->GetUV()[i][1]);
                //    glVertex2f(vertices[i][0], vertices[i][1]);
                //}
                glTexCoord2f(1, 0);
                glVertex2f(vertices[0][0], vertices[0][1]);
                glTexCoord2f(0, 0);
                glVertex2f(vertices[1][0], vertices[1][1]);
                glTexCoord2f(0, 1);
                glVertex2f(vertices[2][0], vertices[2][1]);
                glTexCoord2f(1, 1);
                glVertex2f(vertices[3][0], vertices[3][1]);
            glEnd();
            TriCount += 2;
        }

        void Renderer::DrawMesh(Mesh* mesh, bool drawEdges, bool drawVertices, std::string animationName, unsigned int frame, float t, Color const& color) {
            if(!mesh)
                return;
            //Point* transformed = new Point[mesh->GetVertexCount()];
            //Color* faceColors = new Color[mesh->GetFaceCount()];
            std::vector<Point> transformed = std::vector<Point>(mesh->GetVertexCount());
            std::vector<Color> faceColors = std::vector<Color>(mesh->GetFaceCount());

            // Transform vertices to screen space
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

            // Calculate face colors
            for(unsigned int i = 0; i < mesh->GetFaceCount(); ++i) {
                faceColors[i] = mesh->GetFaceColor(i) * color;
            }

            /*glBegin(GL_TRIANGLES);
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
            }*/

            if(drawEdges) {
                glLineWidth(2);
                std::multimap<unsigned int, IndexedEdge> SortedEdges;
                for(unsigned int i = 0; i < mesh->GetEdgeCount(); ++i) {
                    SortedEdges.insert(std::make_pair(mesh->GetEdgeDrawOrder(i), mesh->GetIndexedEdge(i)));
                }

                unsigned int i = 0;
                std::multimap<unsigned int, IndexedEdge>::const_iterator it = SortedEdges.begin();

                while(i < mesh->GetFaceCount() || it != SortedEdges.end()) {
                    if(i < mesh->GetFaceCount() && (it == SortedEdges.end() || i <= (*it).first)) {
                        SetDrawingColor(faceColors[i]);
                        IndexedFace face = mesh->GetIndexedFace(i);
                        Point uv0 = mesh->GetUV(face.Indices[0]);
                        Point uv1 = mesh->GetUV(face.Indices[1]);
                        Point uv2 = mesh->GetUV(face.Indices[2]);
                        glBegin(GL_TRIANGLES);
                            glTexCoord2f(uv0[0], uv0[1]);
                            glVertex2f(transformed[face.Indices[0]][0], transformed[face.Indices[0]][1]);
                            glTexCoord2f(uv1[0], uv1[1]);
                            glVertex2f(transformed[face.Indices[1]][0], transformed[face.Indices[1]][1]);
                            glTexCoord2f(uv2[0], uv2[1]);
                            glVertex2f(transformed[face.Indices[2]][0], transformed[face.Indices[2]][1]);
                        glEnd();
                        ++TriCount;
                        ++i;
                    }

                    if(it != SortedEdges.end() && ((*it).first < i || i >= mesh->GetFaceCount())) {
                        SetDrawingColor(Color(0, 0, 0, 1));
                        glBegin(GL_LINES);
                            glVertex2f(transformed[(*it).second.Indices[0]][0], transformed[(*it).second.Indices[0]][1]);
                            glVertex2f(transformed[(*it).second.Indices[1]][0], transformed[(*it).second.Indices[1]][1]);
                        glEnd();
                        ++LineCount;
                        ++it;
                    }
                }
            }
            else {
                glBegin(GL_TRIANGLES);
                    for(int i = 0; i < mesh->GetFaceCount(); ++i) {
                        IndexedFace face = mesh->GetIndexedFace(i);
                        SetDrawingColor(faceColors[i]);
                        for(int j = 0; j < 3; ++j) {
                            Point uv = mesh->GetUV(face.Indices[j]);
                            glTexCoord2f(uv[0], uv[1]);
                            glVertex2f(transformed[face.Indices[j]][0], transformed[face.Indices[j]][1]);
                        }
                        ++TriCount;
                    }
                glEnd();
            }

            if(drawVertices) {
                glColor3f(1, 1, 1);
                glPointSize(4);
                glBegin(GL_POINTS);
                    for(int i = 0; i < mesh->GetVertexCount(); ++i) {
                        glVertex2f(transformed[i][0], transformed[i][1]);
                    }
                glEnd();
            }

			//delete[] transformed;
			//delete[] faceColors;
        }

        void Renderer::DrawParticleSystem(Components::ParticleSystem* pSystem) {
            float TransformTime = 0;
            float CompileTime = 0;
            float RasterTime = 0;
            Timer timer;
            SetTexture(ForLease->Resources.GetTexture(pSystem->SpriteSource));
            SetBlendMode(pSystem->BlendingMode);
            std::list<Particle*> const* particles = pSystem->GetActiveParticles();
            std::vector<Point> transformed = std::vector<Point>(particles->size() * 4);
            int p = 0;
            timer.Reset();
            for(std::list<Particle*>::const_iterator i = particles->begin(); i != particles->end(); ++i) {
                SetModelView((*i)->Position, (*i)->Size, (*i)->Size, (*i)->Rotation);
                Matrix combigned = Projection * ModelView;
                transformed[p++] = combigned * Point(-0.5, 0.5);
                transformed[p++] = combigned * Point(-0.5, -0.5);
                transformed[p++] = combigned * Point(0.5, -0.5);
                transformed[p++] = combigned * Point(0.5, 0.5);
            }
            /*std::list<Particle*>::const_iterator sources[PARTICLE_THREADS];
            std::vector<std::thread> threads;
            unsigned int batchSize = particles->size() / PARTICLE_THREADS;
            for(unsigned int i = 0; i < PARTICLE_THREADS; ++i) {
                sources[i] = particles->begin();
                for(unsigned int i = 0; i < batchSize; ++i)
                    ++sources[i];
                if(i < PARTICLE_THREADS - 1)
                    threads.push_back(std::thread(CalcParticleVerts, sources[i], &transformed[i * batchSize], Projection, batchSize));
                else
                    threads.push_back(std::thread(CalcParticleVerts, sources[i], &transformed[i * batchSize], Projection, particles->size() - batchSize * (PARTICLE_THREADS - 1)));
            }

            for(unsigned int i = 0; i < PARTICLE_THREADS; ++i)
                threads[i].join();

            TransformTime = timer.GetTime();*/
            //std::cout << "Drawing " << particles->size() << " particles" << std::endl;
//            GLuint index = glGenLists(1);
            glNewList(index, GL_COMPILE);
            glBegin(GL_QUADS);
                p = 0;
                for(std::list<Particle*>::const_iterator i = particles->begin(); i != particles->end(); ++i) {
                    glColor4fv((*i)->ParticleColor.GetAll());

                    glVertex2f(transformed[p][0], transformed[p][1]);
                    glTexCoord2f(0, 0);
                    ++p;
                    glVertex2f(transformed[p][0], transformed[p][1]);
                    glTexCoord2f(0, 1);
                    ++p;
                    glVertex2f(transformed[p][0], transformed[p][1]);
                    glTexCoord2f(1, 1);
                    ++p;
                    glVertex2f(transformed[p][0], transformed[p][1]);
                    glTexCoord2f(1, 0);
                    ++p;
                }
            glEnd();
//            TransformTime = timer.GetTime();
//            timer.Reset();
            glEndList();
//            CompileTime = timer.GetTime();
//            timer.Reset();
            glCallList(index);
//            RasterTime = timer.GetTime();
            //std::cout << std::endl << "FPS:       " << 1 / ForLease->FrameRateController().GetDt() << std::endl
              //        << "Transform: " << TransformTime << std::endl;
//                      << "Compile:   " << CompileTime << std::endl
//                      << "Raster:    " << RasterTime << std::endl;
//            glDeleteLists(index, 1);
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

        void CalcParticleVerts(std::list<Particle*>::const_iterator source, Point* dest, Matrix const& proj, unsigned int count) {
            for(unsigned int i = 0; i < count; ++i) {
                //SetModelView((*i)->Position, (*i)->Size, (*i)->Size, (*i)->Rotation);
                Matrix combigned = proj * Matrix::Translation((*source)->Position) * Matrix::RotationRad((*source)->Rotation) * Matrix::Scale((*source)->Size, (*source)->Size);
                *(dest++) = combigned * Point(-0.5, 0.5);
                *(dest++) = combigned * Point(-0.5, -0.5);
                *(dest++) = combigned * Point(0.5, -0.5);
                *(dest++) = combigned * Point(0.5, 0.5);
                ++source;
            }
        }
    }
}
