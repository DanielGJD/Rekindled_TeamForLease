/*!
    \file   Edge.cpp
    \author Christopher Hudson
    \date   09/19/15

    \brief
        Implementation of a classes defining an edge and an edge in an indexed vertex list

    \see Edge.h

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "Edge.h"
#include "Vector.h"

namespace ForLeaseEngine {
    /*!
        \brief
            Creates an edge with all zero vertices
    */
    Edge::Edge() {
		Vertices[0] = Point();
		Vertices[1] = Point();
	}

    /*!
        \brief
            Creates an edge out of the given vertices

        \param vert1
            First vertex

        \param vert2
            Second vertex
    */
    Edge::Edge(Point vert1, Point vert2) {
		Vertices[0] = vert1;
		Vertices[1] = vert2;
	}

    /*!
        \brief
            Creates an indexed edge with all zero indices
    */
    IndexedEdge::IndexedEdge() {
		Indices[0] = 0;
		Indices[1] = 1;
	}

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
