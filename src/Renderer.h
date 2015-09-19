#ifndef RENDERER_H
#define RENDERER_H

#include "Vector.h"
#include "Matrix.h"

namespace ForLeaseEngine {
    class Renderer {
        public:
            Renderer();
            void SetProjection(Point position, float width, float height, float near, float far, float rotation);
            void SetProjection(Matrix projectionMatrix);
            void SetModelView(Point position, float scaleX, float scaleY, float rotation);
            void SetModelView(Matrix modelViewMatrix);

            void SetDrawingColor(float r, float g, float b);

            // Slow, each use is a single draw call.  For testing only.
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
