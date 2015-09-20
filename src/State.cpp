/*!
    \file   State.cpp
    \author Sean McGeer
    \date   9/17/15
    \brief
        Contains implementations for the State class defined in State.h.
    \see State.h
*/

namespace ForLeaseEngine {

    /*!
        Constructor for a State.  Initializes Entities.
    */
    State::State() : Entities() {}

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

            if (*(it->GetID()) == id)
                return *it;
        }

        if (throwOnFail) throw EntityNotFoundException(id);

        else return 0;
    }

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

}
