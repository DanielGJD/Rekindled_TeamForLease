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

#include "Texture.h"
#include "Vector.h"
#include "Matrix.h"
#include "Face.h"
#include "Edge.h"
#include "Mesh.h"

namespace ForLeaseEngine {
    /*!
        \class Renderer

        \brief
            Class that can draw things to the screen
    */
    class Renderer {
        public:
            Renderer();
            void SetProjection(Point position, float width, float height, float near, float far, float rotation);
            void SetProjection(Matrix projectionMatrix);
            void SetModelView(Point position, float scaleX, float scaleY, float rotation);
            void SetModelView(Matrix modelViewMatrix);

            void SetDrawingColor(float r, float g, float b);

            void Clear();

            // Slow, each use is a single draw call.  For testing only. Might Get removed.
            void DrawRectangle(Point position, float width, float height, float rotation = 0);
            //void DrawPolygon(Point position, float radius, int sides);
            void DrawLine(Point start, Point end);
            void DrawLine(Point start, Vector displacement);
            //void DrawLine(Point start, Vector direction, float magnitude);
            void DrawArrow(Point start, Point end);
            void DrawArrow(Point start, Vector displacement);
            //void DrawArrow(Point start, Vector direction, float magnitude);
            void DrawRectangleFilled(Point position, float width, float height, float rotation = 0);
            //void DrawPolygonFilled(Point position, float radius, int sides);
            void DrawTexture(Point position, Texture* texture, float scaleX = 1, float scaleY = 1, float rotation = 0);

            // Do not use any of these. For testing only. Will be removed
            void DrawFace(Face& face);
            void DrawEdge(Edge& edge);

            // This one is ok to use, uses a minimum number of draw calls
            void DrawMesh(Mesh& mesh, Point& position, float scaleX = 1, float scaleY = 1, float rotation = 0);
            void DrawWireframeMesh(Mesh& mesh, Point& position, float scaleX = 1, float scaleY = 1, float rotation = 0);

        private:
            //! The projection matrix
            Matrix Projection;
            //! The modelview matrix
            Matrix ModelView;
            //! The current drawing color
            Hcoord color;

            void ToScreenSpace(const Point& vertex, Point& transformed);
            void DrawTris(Point* vertices, int vertexCount);
            void DrawQuads(Point* vertices, int vertexCount);
            void DrawLines(Point* vertices, int vertexCount);
            void DrawLineStrip(Point* vertices, int vertexCount);
    };
}

#endif // RENDER_H
