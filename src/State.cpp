/*!
    \file   State.cpp
    \author Sean McGeer
    \date   9/17/15
    \brief
        Contains implementations for the State class defined in State.h.
    \see State.h
*/

#include "State.h"

namespace ForLeaseEngine {

    /*!
        Constructor for a State.  Initializes Entities.
    */
    State::State() : Entities() {}

    /*!
        Adds an entity to the Entities vector in the state.

        \return
            A pointer to the newly-created entity.
    */
    Entity* State::AddEntity() {
        Entity* entity = new Entity();
        Entities.push_back(entity);

        return entity;
    }

    /*!
        Get a pointer to an entity with the given ID.

        \param id
            The ID of the entity to return.
        \param throwOnFail
            Whether to throw an error on fail, or return a null pointer.
            Defaults to false.
        \return
            A pointer to an entity with the given ID.
    */
    Entity* State::GetEntityByID(long unsigned id, bool throwOnFail) {
        for (std::vector<Entity*>::iterator it = Entities.begin();
             it != Entities.end(); ++it) {

            if ((*it)->GetID() == id)
                return *it;
        }

        if (throwOnFail) throw EntityNotFoundException(id);

        else return 0;
    }

}
