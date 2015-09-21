/*!
    \file   Renderer.cpp
    \author Christopher Hudson
    \date   09/12/15

    \brief
        Defines a class to render things to the screen, internals will change a lot but functions should remain the same

    \see Renderer.h
*/
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
        SetDrawingColor(1.0f, 1.0f, 1.0f);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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

    void Renderer::SetDrawingColor(float r, float g, float b) {
        glColor3f(r, g, b);
    }

    void Renderer::Clear() {
        glClear(GL_COLOR_BUFFER_BIT);
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

    void Renderer::DrawFace(Face& face) {
        Point vertices[] = {face.Vertices[0], face.Vertices[1], face.Vertices[2]};

        DrawTris(vertices, 3);
    }

    void Renderer::DrawMesh(Mesh& mesh, Point& location, float scaleX, float scaleY, float rotation) {
        Point o;
        ModelView = Matrix::Translation(location) * Matrix::Translation(mesh.GetCenter() - o) * Matrix::RotationRad(rotation) * Matrix::Scale(scaleX, scaleY) * Matrix::Translation(o - mesh.GetCenter());
        Point transformed[mesh.GetVertexCount()];
        for(int i = 0; i < mesh.GetVertexCount(); ++i) {
            ToScreenSpace(mesh.GetVertex(i), transformed[i]);
        }
        glBegin(GL_TRIANGLES);
            for(int i = 0; i < mesh.GetFaceCount(); ++i) {
                IndexedFace face = mesh.GetIndexedFace(i);
                for(int j = 0; j < 3; ++j) {
                    glVertex2f(transformed[face.Indices[j]][0], transformed[face.Indices[j]][1]);
                }
            }
        glEnd();
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    void Renderer::DrawTris(Point* vertices, int vertexCount)
    {
        if(vertexCount % 3) {
            throw new GraphicsException("Incorrect number of vertices to draw tris");
        }

        Point transformed[vertexCount];
        for(int i = 0; i < vertexCount; ++i) {
            ToScreenSpace(vertices[i], transformed[i]);
        }
        /*std::cout << "transformed vertex array: " << transformed[0][0] << "," << transformed[0][1] << " | "
                  << transformed[1][0] << "," << transformed[1][1] << " | "
                  << transformed[2][0] << "," << transformed[2][1] << std::endl;*/
        glBegin(GL_TRIANGLES);
            for(int i = 0; i < vertexCount; ++i) {
                glVertex2f(transformed[i][0], transformed[i][1]);
            }
        glEnd();
    }

    void Renderer::DrawQuads(Point* vertices, int vertexCount) {
        if(vertexCount % 4) {
            throw new GraphicsException("Incorrect number of vertices to draw quads");
        }
        Point transformed[vertexCount];
        for(int i = 0; i < vertexCount; ++i) {
            ToScreenSpace(vertices[i], transformed[i]);
            //transformed[i] = (ModelView * vertices[i]);
        }
        glBegin(GL_QUADS);
        for(int i = 0; i < vertexCount; ++i) {
            glVertex2f(transformed[i][0], transformed[i][1]);
        }
        glEnd();
    }

    void Renderer::DrawLines(Point* vertices, int vertexCount) {
        if(vertexCount % 2)
            throw new GraphicsException("Incorrect number of vertices to draw lines");
        Point transformed[vertexCount];
        for(int i = 0; i < vertexCount; ++i) {
            ToScreenSpace(vertices[i], transformed[i]);
            //transformed[i] = (ModelView * vertices[i]);
        }
        glBegin(GL_LINES);
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