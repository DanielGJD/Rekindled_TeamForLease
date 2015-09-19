/*!
    \file   Face.cpp
    \author Christopher Hudson
    \date   09/19/15

    \brief
        Implementation of a class that handles the creation of a window and
        rendering context

    \see Face.h
*/
#include "Vector.h"
#include "Face.h"

namespace ForLeaseEngine {
    /*!
        \brief
            Creates a face with all zero vertices
    */
    Face::Face() : Vertices({Point(), Point(), Point()}) {}

    /*!
        \brief
            Creates a face out of the given vertices

        \param vert1
            First vertex

        \param vert2
            Second vertex

        \param vert 3
            Third vertex
    */
    Face::Face(const Point& vert1, const Point& vert2, const Point& vert3) : Vertices({vert1, vert2, vert3}) {}

    /*!
        \brief
            Creates an indexed face with all zero indices
    */
    IndexedFace::IndexedFace() : Indices{0, 0, 0} {}

    /*!
        \brief
            Creates an indexed face out of the given indices

        \param index1
            First index

        \param index2
            Second index

        \param index3
            Third index
    */
    IndexedFace::IndexedFace(int index1, int index2, int index3) : Indices{index1, index2, index3} {}
}
