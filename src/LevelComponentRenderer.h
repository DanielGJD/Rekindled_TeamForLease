#ifndef LEVEL_COMPONENT_RENDERER_H
#define LEVEL_COMPONENT_RENDERER_H

#include "LevelComponent.h"
#include "Entity.h"
#include "State.h"
#include "ComponentSpriteText.h"
#include "ComponentSprite.h"
#include "ComponentModel.h"
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
                Renderer(State& owner);

                void Serialize(Serializer& root);
                void Deserialize(Serializer& root);

                void Update(std::vector<Entity*>& entities);
                void SetCamera(const Entity& camera);
                void SetCamera(long camera);
                void SetClearColor(float r, float g, float b, float a);
                void SetClearColor(const Color& color);
                void SetDrawingColor(float r, float g, float b, float a = 1);
                void SetDrawingColor(const Color& color);

                // For debug purposes, may be removed at any time, do not use
                void DrawSprite(Components::Sprite* sprite);
                void DrawSpriteText(Components::SpriteText* spriteText, const Point& position, float scaleX, float scaleY, float rotation);
                void DrawModel(Components::Model* model);
                void SetModelView(Components::Transform* transform);
                //void DrawParticleSystem(ParticleSystem* system); For later

                // Debug drawing
                void DrawRectangle(const Point& position, float width, float height, float rotation = 0);
                void DrawRectangleFilled(const Point& position, float width, float height, float rotation = 0);
                void DrawLine(const Point& start, const Point& end);
                void DrawLine(const Point& start, const Vector& displacement);
                void DrawArrow(const Point& start, const Point& end);
                void DrawArrow(const Point& start, const Vector& displacement);

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
            private:
                Matrix Projection;
                Matrix ModelView;
                Color DrawColor;
                Color ClearColor;
                unsigned long CurrentCamera;
                BlendMode BlendingMode;
                GLuint CurrentTexture;

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
                void DrawMesh(Mesh* mesh, bool drawEdges = false, bool drawVertices = false);

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
    }
}

#endif // RENDERER_H
