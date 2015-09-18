/*!
    \file   GraphicsException.cpp
    \author Christopher Hudson
    \date   09/12/15

    \brief
        Implementation of an exception for graphics errors

    \see GraphicsException.h
*/
#include <string>

#include "Exception.h"

#include "GraphicsException.h"

namespace ForLeaseEngine {
    /*!
        \brief
            Creates a graphics exception with a generic message
    */
    GraphicsException::GraphicsException() : Exception("Graphics Exception") {}

    /*!
        \brief
            Creates a graphics exception with the given message

        \param exceptionMessage
            The exception message
    */
    GraphicsException::GraphicsException(const std::string exceptionMessage) : Exception(exceptionMessage) {}
}
