/*!
    \file   Exception.cpp
    \author Christopher Hudson
    \date   09/12/15

    \brief
        Implementation of a base class for all exceptions

    \see Exception.h
*/
#include "Exception.h"

namespace ForLeaseEngine {
    /*!
        \brief
            Creates an exception with a blank message
    */
    Exception::Exception() : message("") {}

    /*!
        \brief
            Creates an exception with the given message

        \param exceptionMessage
            The exception message
    */
    Exception::Exception(const std::string& exceptionMessage) : message(exceptionMessage) {}

    /*!
        \brief
            Sets the exception message

        \param exeptionMessage
            New exceptionMessage message
    */
    void Exception::SetInfo(const std::string& exceptionMessage) {
        message = exceptionMessage;
    }

    /*!
        \brief
            Gets the exception message

        \return
            The exception message
    */
    std::string const& Exception::GetInfo() {
        return message;
    }
}
