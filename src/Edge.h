/*!
    \file   Edge.h
    \author Christopher Hudson
    \date   09/19/15

    \brief
        Defines classes to hold data about an edge

    \see Edge.cpp
*/

#ifndef EDGE_H
#define EDGE_H

#include "Vector.h"

namespace ForLeaseEngine {
    /*!
        \class Edge

        \brief
            An edge defined by world coordinates
    */
    struct Edge {
        Edge();
        Edge(Point vert1, Point vert2);

        //! Two vertices that make up the edge
        Point Vertices[2];
    };

    /*!
        \class IndexedEdge

        \brief
            An edge defined by world coordinates
    */
    struct IndexedEdge {
        IndexedEdge();
        IndexedEdge(int index1, int index2);

        //! Two indices for the vertex array
        int Indices[2];
    };
}

#endif // EDGE_H
