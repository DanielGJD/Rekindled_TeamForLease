/*!
    \file   EntityExceptions.h
    \author Sean McGeer
    \date   9/20/15
    \brief
        Defines various exceptions related to Entities.
    \see EntityExceptions.cpp
    \see Entity.h
    \see Entity.cpp
*/

#ifndef ENTITY_EXCEPTIONS_H
#define ENTITY_EXCEPTIONS_H

#include <string>
#include "Exception.h"
#include "Component.h"

namespace ForLeaseEngine {

    /*!
        \class EntityException
        \brief
            A base class for all entity exceptions.
    */
    class EntityException : public Exception {
        public:
            EntityException();
            EntityException(const long unsigned id);
            EntityException(std::string& message);
            EntityException(const long unsigned id, const std::string& message);
            long unsigned GetEntityID();
        private:
            long unsigned ID; //! The ID of the Entity that this failed on.
    };

    /*!
        \class AddComponentException
        \brief
            A customized exception for when adding a component to an entity fails.
    */
    class AddComponentException : public EntityException {

        public:
            AddComponentException();
            AddComponentException(const ComponentType type);
            AddComponentException(const ComponentType type, const long unsigned id);
            AddComponentException(const ComponentType type, const long unsigned id, const std::string& message);
            ComponentType GetComponentType();
        private:
            ComponentType Type; //! The type of component that caused the exception.

    };

}

#endif
