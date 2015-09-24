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
    /*!
        \brief
            Creates a renderer with black clear color and white drawing color
    */
    Renderer::Renderer() {
        SetDrawingColor(1.0f, 1.0f, 1.0f);
        glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
    }

    /*!
        \brief
            Sets the projection matrix

        \param position
            Position of the center of the viewport

        \param width
            Width of the viewport

        \param height
            Height of the viewport

        \param near
            Near clip plane of the viewport (Unused)

        \param far
            Far clip plane of the viewport (Unused)

        \param rotation
            Rotation of the viewport
    */
    void Renderer::SetProjection(Point position, float width, float height, float near, float far, float rotation) {
        Point origin;
        Matrix trans = Matrix::Translation(origin - position);
        Matrix scale = Matrix::Scale(2 / width, 2 / height);
        Matrix rot = Matrix::RotationRad(-rotation);

        Projection = scale * rot * trans;
    }

    /*!
        \brief
            Sets the projection matrix to a given matrix

        \param projectionMatrix
            The new projection matrix
    */
    void Renderer::SetProjection(Matrix projectionMatrix) {
        Projection = projectionMatrix;
    }

    /*!
        \brief
            Sets the ModelView matrix

        \param position
            Position of the object

        \param scaleX
            X scale of the object

        \param scaleY
            Y scale of the object

        \param rotation
            Rotation of the object
    */
    void Renderer::SetModelView(Point position, float scaleX, float scaleY, float rotation) {
        Point origin;
        Matrix trans = Matrix::Translation(position - origin);
        Matrix rot = Matrix::RotationRad(rotation);
        Matrix scale = Matrix::Scale(scaleX, scaleY);

        ModelView = trans * rot * scale;
    }

    /*!
        \brief
            Sets the ModelView matrix from another matrix

        \param modelViewMatrix
            The new ModelView matrix
    */
    void Renderer::SetModelView(Matrix modelViewMatrix) {
        ModelView = modelViewMatrix;
    }

    /*!
        \brief
            Sets the drawing color

        \param r
            Red component

        \param g
            Green component

        \param b
            Blue component
    */
    void Renderer::SetDrawingColor(float r, float g, float b) {
        glColor3f(r, g, b);
    }

    /*!
        \brief
            Clears the pixel buffer color data
    */
    void Renderer::Clear() {
        glClear(GL_COLOR_BUFFER_BIT);
    }

    /*!
        \brief
            Draws a rectangle with given properties

        \param position
            Position of the rectangle

        \param width
            Width of the rectangle

        \param height
            Height of the rectangle

        \param rotation
            Rotation of the rectangle
    */
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

    /*!
        \brief
            Draws a line

        \param start
            Starting point of the line

        \param end
            Ending point of the line
    */
    void Renderer::DrawLine(Point start, Point end) {
        ModelView = Matrix();
        Point vertices[] = {start, end};
        DrawLines(vertices, 2);
    }

    /*!
        \brief
            Draws a line

        \param start
            Starting point of the line

        \param displacement
            Displacement from the start to the end
    */
    void Renderer::DrawLine(Point start, Vector displacement) {
        DrawLine(start, start + displacement);
    }

    /*!
        \brief
            Draws an arrow

        \param start
            Starting point of the arrow

        \param end
            Ending point of the arrow
    */
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

    /*!
        \brief
            Draws an arrow

        \param start
            Starting point of the arrow

        \param displacement
            Displacement from the start to the end of the arrow
    */
    void Renderer::DrawArrow(Point start, Vector displacement) {
        DrawArrow(start, start + displacement);
    }

    /*!
        \brief
            Draws a filled rectangle

        \param position
            Position of the rectangle

        \param width
            Width of the rectangle

        \param height
            Height of the rectangle

        \param rotation
            Rotation of the rectangle
    */
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

    void Renderer::DrawTexture(Point position, Texture* texture, float scaleX, float scaleY, float rotation) {
        Point o;
        ModelView = Matrix::Translation(position - o) * Matrix::RotationRad(rotation) * Matrix::Scale(scaleX, scaleY);
        float halfWidth = texture->GetWidth() / 2;
        float halfHeight = texture->GetHeight() / 2;
        Point vertices[] = {Point(halfWidth, halfHeight), Point(-halfWidth, halfHeight),
                            Point(-halfWidth, -halfHeight), Point(halfWidth, - halfHeight)};
        Point uv[] = {Point(1, 1), Point(0, 1), Point(0, 0), Point(1, 0)};
        for(int i = 0; i < 4; ++i) {
            ToScreenSpace(vertices[i], vertices[i]);
        }
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, texture->GetID());
        glBegin(GL_QUADS);
            for(int i = 0; i < 4; ++i) {
                glTexCoord2f(uv[i][0], uv[i][1]);
                glVertex2f(vertices[i][0], vertices[i][1]);
            }
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);
    }

    /*!
        \brief
            Draws a triangle face

        \param face
            The face to draw
    */
    void Renderer::DrawFace(Face& face) {
        Point vertices[] = {face.Vertices[0], face.Vertices[1], face.Vertices[2]};

        DrawTris(vertices, 3);
    }

    /*!
        \brief
            Draws a mesh

        \param mesh
            The mesh to draw

        \param position
            The position of the mesh

        \param scaleX
            X scale of the mesh

        \param scaleY
            Y scale of the mesh

        \param rotation
            rotation of the mesh
    */
    void Renderer::DrawMesh(Mesh& mesh, Point& position, float scaleX, float scaleY, float rotation) {
        Point o;
        ModelView = Matrix::Translation(position) * Matrix::Translation(mesh.GetCenter() - o) * Matrix::RotationRad(rotation) * Matrix::Scale(scaleX, scaleY) * Matrix::Translation(o - mesh.GetCenter());
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

    void Renderer::DrawWireframeMesh(Mesh& mesh, Point& position, float scaleX, float scaleY, float rotation) {
        Point o;
        ModelView = Matrix::Translation(position) * Matrix::Translation(mesh.GetCenter() - o) * Matrix::RotationRad(rotation) * Matrix::Scale(scaleX, scaleY) * Matrix::Translation(o - mesh.GetCenter());
        Point transformed[mesh.GetVertexCount()];
        for(int i = 0; i < mesh.GetVertexCount(); ++i) {
            ToScreenSpace(mesh.GetVertex(i), transformed[i]);
        }
        glBegin(GL_LINES);
            for(int i = 0; i < mesh.GetEdgeCount(); ++i) {
                IndexedEdge edge = mesh.GetIndexedEdge(i);
                glVertex2f(transformed[edge.Indices[0]][0], transformed[edge.Indices[0]][1]);
                glVertex2f(transformed[edge.Indices[1]][0], transformed[edge.Indices[1]][1]);
            }
        glEnd();
    }

    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /*!
        \brief
            Draws tris from a list of vertices

        \param vertices
            Vertex data to draw

        \param vertexCount
            Number of vertices to draw
    */
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

    /*!
        \brief
            Draws quads from a list of vertices

        \param vertices
            Vertex data to draw

        \param vertexCount
            Number of vertices to draw
    */
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

    /*!
        \brief
            Draws lines from a list of vertices

        \param vertices
            Vertex data to draw

        \param vertexCount
            Number of vertices to draw
    */
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

    /*!
        \brief
            Draws a line strip from a list of vertices

        \param vertices
            Vertex data to draw

        \param vertexCount
            Number of vertices
    */
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

    /*!
        \brief
            Transforms a vertex from local space into screen space

        \param vertex
            The vertex to transform

        \param transformed
            The transformed vertex
    */
    void Renderer::ToScreenSpace(const Point& vertex, Point& transformed) {
        Hcoord temp;
        temp = (Projection * ModelView * vertex);
        transformed = temp;
    }
}
