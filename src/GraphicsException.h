/*!
    \file   GraphicsException.h
    \author Christopher Hudson
    \date   09/12/15

    \brief
        Defines a class for all graphics related exceptions

    \see GraphicsException.cpp
*/
#ifndef GRAPHICSEXCEPTION_H
#define GRAPHICSEXCEPTION_H

#include <string>

#include "Exception.h"

namespace ForLeaseEngine {
    /*!
        \class Graphics Exception

        \brief
            Exception class for graphics exceptions
    */
    class GraphicsException : public Exception {
        public:
            GraphicsException();
            GraphicsException(const std::string exceptionMessage);
    };
}

#endif
