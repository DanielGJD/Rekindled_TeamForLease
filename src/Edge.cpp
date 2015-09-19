/*!
    \file   Edge.cpp
    \author Christopher Hudson
    \date   09/19/15

    \brief
        Implementation of a class that handles the creation of a window and
        rendering context

    \see Edge.h
*/

#include "Edge.h"
#include "Vector.h"

namespace ForLeaseEngine {
    /*!
        \brief
            Creates an edge with all zero vertices
    */
    Edge::Edge() : Vertices({Point(), Point()}) {}

    /*!
        \brief
            Creates an edge out of the given vertices

        \param vert1
            First vertex

        \param vert 2
            Second vertex
    */
    Edge::Edge(Point vert1, Point vert2) : Vertices({vert1, vert2}) {}

    /*!
        \brief
            Creates an indexed edge with all zero indices
    */
    IndexedEdge::IndexedEdge() : Indices{0, 0} {}

    /*!
        \brief
            Creates an indexed face out of the given indices

        \param index1
            First index

        \param index2
            Second index
    */
    IndexedEdge::IndexedEdge(int index1, int index2) : Indices{index1, index2} {}
}
