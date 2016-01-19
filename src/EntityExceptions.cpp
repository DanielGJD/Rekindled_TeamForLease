/*!
    \file   EntityExceptions.cpp
    \author Sean McGeer
    \date   9/20/15
    \brief
        Implements various exceptions related to Entities.
    \see EntityExceptions.h
    \see Entity.h
    \see Entity.cpp

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "EntityExceptions.h"

namespace ForLeaseEngine {

    /*!
        Constructor for an EntityException.  Sets the message to a generic error
        message, and sets the ID of the Entity that caused this to 0 (no Entity).
    */
    EntityException::EntityException() : Exception("Error with an entity."), ID(0) {}

    /*!
        Constructor for an EntityException.  Sets the message to a generic error
        message, and sets the ID of the Entity that caused this error.

        \param id
            The ID of the Entity that caused this error.
    */
    EntityException::EntityException(const long unsigned id)
        : Exception("Error with an entity."), ID(id) {}

    /*!
        Constructor for an EntityException.  Sets the message to a user-provided
        message, and sets the ID of the Entity that caused this error to 0 (no Entity).
    */
    EntityException::EntityException(const std::string& message)
        : Exception(message), ID(0) {}

    /*!
        Constructor for an EntityException.  Sets the message to a user-provided
        message, and sets the ID of the Entity that caused this error.

        \param id
            The ID of the Entity that caused this error.

        \param message
            The user-provided message for this error.
    */
    EntityException::EntityException(const long unsigned id, const std::string& message)
        : Exception(message), ID(id) {}

    /*!
        Returns the ID of the entity that caused this error.

        \return
            The long unsigned ID of the entity that caused the error.
    */
    long unsigned EntityException::GetEntityID() { return ID; }




    /*!
        Constructor for an AddComponentException.  Sets the base EntityException
        class to defaults with an altered message.
    */
    AddComponentException::AddComponentException()
        : EntityException("Error adding component."), Type(ComponentType::None) {}

    /*!
        Constructor for an AddComponentException.  Sets the message to a generic
        error message.

        \param type
            A ComponentType denoting which type of component caused the error.
    */
    AddComponentException::AddComponentException(const ComponentType type)
        : EntityException("Error adding component."), Type(type) {}

    /*!
        Constructor for an AddComponentException.  Sets the message to a generic
        error message, and sets the ID of the Entity that caused this.

        \param type
            A ComponentType to determine which type of component caused this error.

        \param id
            The long unsigned ID of the Entity that caused this error.
    */
    AddComponentException::AddComponentException(const ComponentType type, const long unsigned id)
        : EntityException(id, "Error adding component"), Type(type) {}

    /*!
        Constructor for an AddComponentException.

        \param type
            A ComponentType to determine which type of component caused this error.

        \param id
            The long unsigned ID of the Entity that caused this error.

        \param message
            A std::string that is the message to set for this exception.
    */
    AddComponentException::AddComponentException(const ComponentType type,
        const long unsigned id, const std::string& message)
        : EntityException(id, message), Type(type) {}

    /*!
        Gets the type of the component that caused this error.
    */
    ComponentType AddComponentException::GetComponentType() { return Type; }



    OutOfIDsException::OutOfIDsException() : Exception("Out of IDs."), MaxID(0) {}
    OutOfIDsException::OutOfIDsException(const long unsigned maxID)
        : Exception("Out of IDs."), MaxID(maxID) {}
    OutOfIDsException::OutOfIDsException(const long unsigned maxID, const std::string& message)
        : Exception(message), MaxID(maxID) {}
    OutOfIDsException::OutOfIDsException(const std::string& message)
        : Exception(message), MaxID(0) {}

    long unsigned OutOfIDsException::GetMaxID() { return MaxID; }

}
