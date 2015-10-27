/*!
    \file   Entity.h
    \author Sean McGeer
    \date   9/17/15
    \brief
        Defines the Entity class.
    \see Entity.cpp
*/

#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include "ComponentsInclude.h"
#include "EntityExceptions.h"
#include "Serializable.h"
#include "Serialize.h"

namespace ForLeaseEngine {

    /*!
        \class Entity
        \brief
            The base class for all game objects.
    */
    class Entity : public Serializable{

        public:
            Entity();
            ~Entity();
            void Update();
            void Serialize(Serializer& root);
            void Deserialize(Serializer& root);
            long unsigned GetID() const;
            void AddComponent(Component* component);
            ComponentType GetComponentMask();
            bool HasComponent(ComponentType type);
            /*!
                Get a Component from this Entity.

                \param type
                    The type of the given component.

                \param throwOnFail
                    A boolean determining whether to throw an error on failure, rather
                    than the default behavior, which is to return a null pointer.
            */
            template <typename T = Component>
            T* GetComponent(bool throwOnFail = false) const {
                if (T::Type == ComponentType::None && throwOnFail)
                    throw EntityException(ID, "No component specified.");

                for (Component* component : Components)
                    if (component->GetType() == T::Type)
                        return reinterpret_cast<T*>(component);

                if (throwOnFail) throw EntityException(ID, "Error finding component.");

                return 0;
            }
        private:
            //! The ID of this Entity, to differentiate from other Entities.
            long unsigned ID;
            //! The mask of all components ORd together that this Entity has.
            ComponentType ComponentMask;
            //! The std::vector of pointers to all Components on this Entity.
            std::vector<Component *> Components;
            //! A static ID of all Entities that have been created by the game.
            static long unsigned TotalEntities;

    };

    void AddComponentsToEntity(ComponentType mask, Entity* entity);
    Component* DeserializeComponent(Serializer& root, std::string& name, Entity& entity);

} // ForLeaseEngine

#endif
