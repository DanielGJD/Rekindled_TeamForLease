/*!
    \file   StateExceptions.cpp
    \author Sean McGeer
    \date   9/20/15
    \brief
        Implements various exceptions related to States.
    \see StateExceptions.h
    \see State.h
    \see State.cpp

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "StateExceptions.h"

namespace ForLeaseEngine {

    /*!
        Constructor for an EntityNotFoundException.  Sets the ID to 0, which basically
        means none.  Also sets a generic error message.
    */
    EntityNotFoundException::EntityNotFoundException()
        : Exception("Entity not found."), ID(0) {}

    /*!
        Constructor for an EntityNotFoundException.  Sets the ID to a given ID.
        Also sets a generic error message.

        \param id
            A long unsigned int denoting the ID given by the user.
    */
    EntityNotFoundException::EntityNotFoundException(const long unsigned id)
        : Exception("Entity not found."), ID(id) {}

    /*!
        Constructor for an EntityNotFoundException.  Sets the ID to a given ID.
        Also sets the message to a given error message.

        \param id
            A long unsigned int denoting the ID given by the user.

        \param message
            The error message to use.
    */
    EntityNotFoundException::EntityNotFoundException(const long unsigned id,
        const std::string& message) : Exception(message), ID(id) {}

    long unsigned EntityNotFoundException::GetID() { return ID; }

}
