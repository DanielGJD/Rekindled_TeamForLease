/*!
    \file   Face.h
    \author Christopher Hudson
    \date   09/19/15

    \brief
        Defines classes to hold data about a triangular face

    \see Face.cpp
*/

#ifndef FACE_H
#define FACE_H

#include "Vector.h"
#include "Matrix.h"

namespace ForLeaseEngine {
    /*!
        \class Face

        \brief
            A face defined by world coordinates
    */
    struct Face {
        Face();
        Face(const Point& vert1, const Point& vert2, const Point& vert3);

        //! Three vertices that make up the face
        Point Vertices[3];
    };

    /*!
        \class IndexedFace

        \brief
            A face defined by an index into a Point array
    */
    struct IndexedFace {
        IndexedFace();
        IndexedFace(int index1, int index2, int index3);

        //! Three indices for the vertex array
        int Indices[3];
    };
}

#endif // FACE_H
