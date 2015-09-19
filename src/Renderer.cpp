#include <iostream>
#include <SDL_opengl.h>
//#include <GL\GLU.h>
#include <GL\GL.h>
#include "Vector.h"
#include "Matrix.h"
#include "Renderer.h"
#include "GraphicsException.h"

namespace ForLeaseEngine {
    Renderer::Renderer() {
    }

    void Renderer::SetProjection(Point position, float width, float height, float near, float far, float rotation) {
        Point origin;
        Matrix trans = Matrix::Translation(origin - position);
        Matrix scale = Matrix::Scale(2 / width, 2 / height);
        Matrix rot = Matrix::RotationRad(-rotation);

        Projection = scale * rot * trans;
    }

    void Renderer::SetProjection(Matrix projectionMatrix) {
        Projection = projectionMatrix;
    }

    void Renderer::SetModelView(Point position, float scaleX, float scaleY, float rotation) {
        Point origin;
        Matrix trans = Matrix::Translation(position - origin);
        Matrix rot = Matrix::RotationRad(rotation);
        Matrix scale = Matrix::Scale(scaleX, scaleY);

        ModelView = trans * rot * scale;
    }

    void Renderer::SetModelView(Matrix modelViewMatrix) {
        ModelView = modelViewMatrix;
    }

    void Renderer::DrawRectangle(Point position, float width, float height, float rotation) {
        ModelView = Matrix::RotationRad(rotation);
        float halfWidth = width / 2;
        float halfHeight = height / 2;
        Point rectanglePoints[] = {Point(position[0] + halfWidth, position[1] - halfHeight),
                                   Point(position[0] + halfWidth, position[1] + halfHeight),
                                   Point(position[0] - halfWidth, position[1] + halfHeight),
                                   Point(position[0] - halfWidth, position[1] - halfHeight),
                                   Point(position[0] + halfWidth, position[1] - halfHeight)};

        DrawLineStrip(rectanglePoints, 5);
    }

    void Renderer::DrawLine(Point start, Point end) {
        ModelView = Matrix();
        Point vertices[] = {start, end};
        DrawLines(vertices, 2);
    }

    void Renderer::DrawLine(Point start, Vector displacement) {
        DrawLine(start, start + displacement);
    }

    void Renderer::DrawArrow(Point start, Point end) {
        ModelView = Matrix();
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

        DrawLineStrip(vertices, 5);
    }

    void Renderer::DrawArrow(Point start, Vector displacement) {
        DrawArrow(start, start + displacement);
    }

    void Renderer::DrawRectangleFilled(Point position, float width, float height, float rotation) {
        ModelView = Matrix::RotationRad(rotation);
        float halfWidth = width / 2;
        float halfHeight = height / 2;
        Point vertices[] = {Point(position[0] + halfWidth, position[1] - halfHeight),
                                   Point(position[0] + halfWidth, position[1] + halfHeight),
                                   Point(position[0] - halfWidth, position[1] + halfHeight),
                                   Point(position[0] - halfWidth, position[1] - halfHeight)};

        DrawQuads(vertices, 4);
    }

    void Renderer::DrawQuads(Point* vertices, int vertexCount) {
        Point transformed[vertexCount];
        for(int i = 0; i < vertexCount; ++i) {
            ToScreenSpace(vertices[i], transformed[i]);
            //transformed[i] = (ModelView * vertices[i]);
        }
        glBegin(GL_QUADS);
        glColor3f(1.0f, 0.0f, 0.0f);
        for(int i = 0; i < vertexCount; ++i) {
            glVertex2f(transformed[i][0], transformed[i][1]);
        }
        glEnd();
    }

    void Renderer::DrawLines(Point* vertices, int vertexCount) {
        if(vertexCount % 2)
            throw new GraphicsException("Odd number of vertices for lines");
        Point transformed[vertexCount];
        for(int i = 0; i < vertexCount; ++i) {
            ToScreenSpace(vertices[i], transformed[i]);
            //transformed[i] = (ModelView * vertices[i]);
        }
        glBegin(GL_LINES);
        glColor3f(1.0f, 0.0f, 0.0f);
        for(int i = 0; i < vertexCount; i += 2) {
            glVertex2f(transformed[i][0], transformed[i][1]);
            glVertex2f(transformed[i + 1][0], transformed[i + 1][1]);
        }
        glEnd();
    }

    void Renderer::DrawLineStrip(Point* vertices, int vertexCount) {
        Point transformed[vertexCount];
        for(int i = 0; i < vertexCount; ++i) {
            ToScreenSpace(vertices[i], transformed[i]);
            //transformed[i] = (ModelView * vertices[i]);
        }
        glBegin(GL_LINE_STRIP);
        glColor3f(1.0f, 0.0f, 0.0f);
        for(int i = 0; i < vertexCount; ++i) {
            glVertex2f(transformed[i][0], transformed[i][1]);
        }
        glEnd();
    }

    void Renderer::ToScreenSpace(const Point& vertex, Point& transformed) {
        Hcoord temp;
        temp = (Projection * ModelView * vertex);
        transformed = temp;
    }
}
