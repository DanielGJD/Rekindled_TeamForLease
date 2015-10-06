/*!
    \file   Renderer.h
    \author Christopher Hudson
    \date   09/22/15

    \brief
        Defines a class that draws things to the screen

    \see Renderer.cpp
*/

#ifndef RENDERER_H
#define RENDERER_H

#include "TextureRegion.h"
#include "Texture.h"
#include "Vector.h"
#include "Matrix.h"
#include "Face.h"
#include "Edge.h"
#include "Mesh.h"
#include "SpriteText.h"
#include "ComponentCamera.h"
#include "ComponentTransform.h"

namespace ForLeaseEngine {
    /*!
        \class Renderer

        \brief
            Class that can draw things to the screen
    */
    class Renderer {
        public:
            Renderer();
            void SetCamera(Entity* camera);
            void Update();
            unsigned int GetTextureSwapCount();
            unsigned int GetTriCount();
            unsigned int GetBlendModeSwapCount();

            void SetProjection(Point position, float width, float height, float near, float far, float rotation);
            void SetProjection(Matrix projectionMatrix);

            void SetModelView(Point position, float scaleX, float scaleY, float rotation);
            void SetModelView(Matrix modelViewMatrix);

            void SetDrawingColor(float r, float g, float b, float a = 1.0f);
            void SetDrawingColor(Color color);

            void SetBlendMode(BlendMode blend);

            void SetClearColor(float r, float g, float b, float a = 1.0f);
            void SetClearColor(Color color);

            void SetTexture(Texture* texture);

            void Clear();

            // Slow, each use is a single draw call.  For testing only. Might Get removed.
            void DrawRectangle(Point position, float width, float height, float rotation = 0);
            //void DrawPolygon(Point position, float radius, int sides);
            void DrawLine(Point start, Point end);
            void DrawLine(Point start, Vector displacement);
            void DrawArrow(Point start, Point end);
            void DrawArrow(Point start, Vector displacement);
            void DrawRectangleFilled(Point position, float width, float height, float rotation = 0);
            //void DrawPolygonFilled(Point position, float radius, int sides);
            void DrawTexture(Point position, Texture* texture, float scaleX = 1, float scaleY = 1, float rotation = 0);
            void DrawTextureRegion(Point position, TextureRegion* region, float scaleX = 1, float scaleY =1 , float rotation = 0);

            // Do not use any of these. For testing only. Will be removed
            void DrawFace(Face& face);
            void DrawEdge(Edge& edge);

            // This one is ok to use, uses a minimum number of draw calls
            void DrawMesh(Mesh& mesh, Point& position, float scaleX = 1, float scaleY = 1, float rotation = 0);
            void DrawWireframeMesh(Mesh& mesh, Point& position, float scaleX = 1, float scaleY = 1, float rotation = 0);

            void DrawSpriteText(SpriteText& spriteText, Point position, float scaleX = 1, float scaleY = 1, float rotation = 0);
        private:
            //! The projection matrix
            Matrix Projection;
            //! The modelview matrix
            Matrix ModelView;
            //! The current drawing color
            Color DrawColor;
            //! Clear color
            Color ClearColor;
            //! Current camera
            Entity* CurrentCamera;
            //! Current blending mode
            BlendMode BlendingMode;
            //! Current bound texture
            GLuint CurrentTexture;

            unsigned int TextureSwapCount;
            unsigned int TriCount;
            unsigned int BlendModeSwapCount;

            void ToScreenSpace(const Point& vertex, Point& transformed);
            void DrawTris(Point* vertices, int vertexCount);
            void DrawQuads(Point* vertices, int vertexCount);
            void DrawLines(Point* vertices, int vertexCount);
            void DrawLineStrip(Point* vertices, int vertexCount);
    };
}

#endif // RENDER_H
