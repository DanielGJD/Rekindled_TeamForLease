#ifndef RENDERER_H
#define RENDERER_H

#include "Vector.h"
#include "Matrix.h"
#include "Face.h"
#include "Edge.h"
#include "Mesh.h"

namespace ForLeaseEngine {
    class Renderer {
        public:
            Renderer();
            void SetProjection(Point position, float width, float height, float near, float far, float rotation);
            void SetProjection(Matrix projectionMatrix);
            void SetModelView(Point position, float scaleX, float scaleY, float rotation);
            void SetModelView(Matrix modelViewMatrix);

            void SetDrawingColor(float r, float g, float b);

            // Slow, each use is a single draw call.  For testing only. Might Get removed.
            void DrawRectangle(Point position, float width, float height, float rotation = 0);
            void DrawPolygon(Point position, float radius, int sides);
            void DrawLine(Point start, Point end);
            void DrawLine(Point start, Vector displacement);
            //void DrawLine(Point start, Vector direction, float magnitude);
            void DrawArrow(Point start, Point end);
            void DrawArrow(Point start, Vector displacement);
            //void DrawArrow(Point start, Vector direction, float magnitude);
            void DrawRectangleFilled(Point position, float width, float height, float rotation = 0);
            void DrawPolygonFilled(Point position, float radius, int sides);

            // Do not use any of these. For testing only. Will be removed
            void DrawFace(Face& face);
            void DrawEdge(Edge& edge);

            // This one is ok to use, uses a minimum number of draw calls
            void DrawMesh(Mesh& mesh, Point& location, float scaleX = 1, float scaleY = 1, float rotation = 0);

            void PrintMatrices();

        private:
            Matrix Projection;
            Matrix ModelView;
            Hcoord color;
            void ToScreenSpace(const Point& vertex, Point& transformed);
            void DrawTris(Point* vertices, int vertexCount);
            void DrawQuads(Point* vertices, int vertexCount);
            void DrawLines(Point* vertices, int vertexCount);
            void DrawLineStrip(Point* vertices, int vertexCount);
    };
}

#endif // RENDER_H
