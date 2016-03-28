/*!
    \file   LevelComponentRenderer.h
    \author Christopher Hudson

    \brief
        Defines a class for rendering

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef LEVEL_COMPONENT_RENDERER_H
#define LEVEL_COMPONENT_RENDERER_H

#include "LevelComponent.h"
#include "Entity.h"
#include "State.h"
#include "ComponentSpriteText.h"
#include "ComponentSprite.h"
#include "ComponentModel.h"
#include "ComponentParticleSystem.h"
#include "Mesh.h"
#include "Texture.h"
#include "TextureRegion.h"
#include "Color.h"
#include "Serializable.h"
#include "Serialize.h"

namespace ForLeaseEngine {
    namespace LevelComponents {
        class Renderer : public LevelComponent {
            public:
                static const ComponentType Type = ComponentType::Renderer;
                virtual ComponentType GetType() { return Type; }
                Renderer(State& owner);
                ~Renderer();

                void Serialize(Serializer& root);
                void Deserialize(Serializer& root);

                void Update(std::vector<Entity*>& entities);
                void SetCamera(const Entity& camera);
                void SetCamera(unsigned long camera);
                long GetCameraID();
                void SetClearColor(float r, float g, float b, float a);
                void SetOverlayColor(float r, float g, float b, float a);
                void SetOutline(float r, float g, float b, float a, float width = 4);
                void SetClearColor(const Color& color);
                void SetDrawingColor(float r, float g, float b, float a = 1);
                void SetDrawingColor(const Color& color);
                void SetDebugPointSize(float size);
                void SetDebugLineWidth(float width);

                // For debug purposes, may be removed at any time, do not use
                void DrawSprite(Components::Sprite* sprite);
                void DrawSpriteText(Components::SpriteText* spriteText, const Point& position, float scaleX, float scaleY, float rotation);
                void DrawModel(Components::Model* model);
                void DrawParticleSystem(Components::ParticleSystem* pSystem);
                void SetModelView(Components::Transform* transform);

                // Debug drawing
                void DrawRectangle(const Point& position, float width, float height, float rotation = 0);
                void DrawRectangleFilled(const Point& position, float width, float height, float rotation = 0, BlendMode blendMode = BlendMode::NONE);
                void DrawLine(const Point& start, const Point& end);
                void DrawLine(const Point& start, const Vector& displacement);
                void DrawArrow(const Point& start, const Point& end);
                void DrawArrow(const Point& start, const Vector& displacement);
                void DrawMesh(Mesh* mesh, bool drawEdges = false, bool drawVertices = false, std::string animationName = "", unsigned int frame = 0, float t = 0, Color const& color = Color(1, 1, 1, 1));
                void DrawPoint(const Point& point);
                void DrawCircle(const Point& center, float radius);

                // Debug stats
                unsigned int GetVertexCount();
                unsigned int GetTriCount();
                unsigned int GetLineCount();
                unsigned int GetTextureSwapCount();
                unsigned int GetBlendModeSwapCount();
                float GetRenderTime();

                // Screen to world transforms
                Point ScreenToWorld(const Point& point);
                Point WorldToScreen(const Point& point);
                void Reload();
            private:
                Matrix Projection;
                Matrix ModelView;
                Color DrawColor;
                Color ClearColor;
                Color OverlayColor;
                Color OutlineColor;
                float OutlineWidth;
                unsigned long CurrentCamera;
                BlendMode BlendingMode;
                GLuint CurrentTexture;
                GLuint LightTexture;
                GLuint LightFBO;
                GLuint UITexture;
                GLuint UIFBO;

                float DebugPointSize;
                float DebugLineWidth;

                // Debug info
                unsigned int VertexCount;
                unsigned int TriCount;
                unsigned int LineCount;
                unsigned int TextureSwapCount;
                unsigned int BlendModeSwapCount;
                float RenderTime;

                // Resource drawing functions
                void DrawTexture(Texture* texture);
                void DrawTextureRegion(TextureRegion* region);

                // Property functions
                void SetBlendMode(BlendMode mode);
                void SetTexture(Texture* texture);
                void SetProjection(const Point& position, float width, float height, float near, float far, float rotation);
                void SetModelView(const Point& position, float scaleX, float scaleY, float rotation);

                // Transformation functions
                //void ModelToWorld(const Point& source, Point& dest);
                //void WorldToModel(const Point& source, Point& dest);
                //void WorldToScreen(const Point& source, Point& dest);
                //void ScreenToWorld(const Point& source, Point& dest);
                void ModelToScreen(const Point& source, Point& dest);
                void ModelToScreen(const Point* source, Point* dest, int count);
        };

        void CalcParticleVerts(std::list<Particle*>::const_iterator source, Point* dest, Matrix const& proj, unsigned int count);
    }
}

#endif // RENDERER_H
