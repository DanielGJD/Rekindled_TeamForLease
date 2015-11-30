/*!
    \file   StateExceptions.h
    \author Sean McGeer
    \date   9/20/15
    \brief
        Defines various exceptions related to States.
    \see StateExceptions.cpp
    \see State.h
    \see State.cpp

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef STATE_EXCEPTIONS_H
#define STATE_EXCEPTIONS_H

#include "State.h"

namespace ForLeaseEngine {

    /*!
        \class EntityNotFoundException
        \brief
            A customized exception for when finding an entity fails.
    */
    class EntityNotFoundException : public Exception {

        public:
            EntityNotFoundException();
            EntityNotFoundException(const long unsigned id);
            EntityNotFoundException(const long unsigned id, const std::string& message);
            long unsigned GetID();
        private:
            const long unsigned ID;

    };

}

#endif
