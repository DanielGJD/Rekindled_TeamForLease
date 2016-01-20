/*!
    \file   Entity.h
    \author Sean McGeer
    \date   9/17/15
    \brief
        Defines the Entity class.
    \see Entity.cpp

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef ENTITY_H
#define ENTITY_H

#include <vector>
#include <string>
#include <set>
#include <random>
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
            Entity(std::string name = "", boolean serialize = true);
            ~Entity();
            void Update();
            void Serialize(Serializer& root);
            void Deserialize(Serializer& root);
            void CreateArchetype(std::string filename);
            long unsigned GetID() const;
            std::string GetName() const;
            void SetName(std::string name);
            bool AddComponent(Component* component);
            void DeleteComponent(ComponentType type);
            ComponentType GetComponentMask();
            bool HasComponent(ComponentType type);
            /*!
                Get a Component from this Entity.

                \param throwOnFail
                    A boolean determining whether to throw an error on failure, rather
                    than the default behavior, which is to return a null pointer.
            */
            template <typename T = Component>
            T* GetComponent(bool throwOnFail = false) const {
                if (T::Type == ComponentType::None && throwOnFail)
                    throw EntityException(ID, "No component specified.");

                for (Component* component : Components) {
                    if (!component) continue;

                    if (component->GetType() == T::Type)
                        return reinterpret_cast<T*>(component);
                }

                if (throwOnFail) throw EntityException(ID, "Error finding component.");

                return 0;
            }

            bool Delete;
            bool IncludeInSerialize;
        private:
            //! The ID of this Entity, to differentiate from other Entities.
            long unsigned ID;
            //! The name of this Entity, to differentiate from other Entities.
            std::string Name;
            //! The mask of all components ORd together that this Entity has.
            ComponentType ComponentMask;
            //! The std::vector of pointers to all Components on this Entity.
            std::vector<Component *> Components;
            //! A static ID of all Entities that have been created by the game.
            static long unsigned TotalEntities;

            static std::set<unsigned long> IDs;
            static const unsigned long MaxEntities = 9999;
            static std::default_random_engine RandomEngine;
            static std::uniform_int_distribution<unsigned long> Distribution;
            static unsigned long GetNewID();
            static void FreeID(unsigned long id);


    };

    void AddComponentsToEntity(ComponentType mask, Entity* entity);
    Component* DeserializeComponent(Serializer& root, Entity& entity);

} // ForLeaseEngine

#endif
