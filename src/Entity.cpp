/*!
    \file   Entity.cpp
    \author Sean McGeer
    \date   9/18/15
    \brief
        Implements the Entity class defined in Entity.h.
    \see Entity.h

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#include "Entity.h"
#include <sstream>

namespace ForLeaseEngine {

    long unsigned Entity::TotalEntities = 0;

    unsigned long Entity::MaxEntities = 9999;
    std::set<unsigned long> Entity::IDs = std::set<unsigned long>();
    std::default_random_engine Entity::RandomEngine = std::default_random_engine();

    /*!
        Constructor for a new Entity.  Sets the ID of this Entity to a new,
        unique ID that is derived from the total number of entities created before
        it plus one.  It also initializes the bitfield mask of components added
        to this entity to None.
    */
    Entity::Entity(std::string name, bool serialize)
    : ID(GetNewID()), ComponentMask(ComponentType::None), IncludeInSerialize(serialize), Delete(false) {
        if (name != "")
            Name = name;
        else {
            std::stringstream ss;
            ss << "[Entity " << ID << "]";
            Name = ss.str();
        }
    }

    /*!
        Destructor for an Entity.  Destroys all components attached to it.
    */
    Entity::~Entity() {
        DeleteAllComponents();

        FreeID(ID);
    }

    void Entity::Serialize(Serializer& root) {
        Serializer entity = root.GetChild("Entity");
        entity.WriteUint("ID", static_cast<unsigned>(ID));
        entity.WriteString("Name", Name);
        ArraySerializer jsonComponents(entity);
        jsonComponents = entity.GetChild("Components");
        for(Component* component : Components) {
            Serializer componentSerializer;
            component->Serialize(componentSerializer);
            jsonComponents.Append(componentSerializer);
        }
        entity.Append(jsonComponents, "Components");
        root.Append(entity, "Entity");
    }

    void Entity::Deserialize(Serializer& root) {
        Serializer entity = root.GetChild("Entity");
        FreeID(ID);
        unsigned id;
        entity.ReadUint("ID", id);
        entity.ReadString("Name", Name);
        ID = id;
        auto insertion = IDs.insert(id);
        if (!insertion.second) {
            ID = GetNewID();
        }
        ArraySerializer jsonComponents(entity);
        jsonComponents = entity.GetChild("Components");
        for (unsigned i = 0; i < jsonComponents.Size(); ++i) {
            Serializer component = jsonComponents[i];
            AddComponent(DeserializeComponent(component, *this));
        }
    }

    void Entity::DeserializeWithoutID(Serializer& root) {
Serializer entity = root.GetChild("Entity");
entity.ReadString("Name", Name);
ArraySerializer jsonComponents(entity);
jsonComponents = entity.GetChild("Components");
for (unsigned i = 0; i < jsonComponents.Size(); ++i) {
    Serializer component = jsonComponents[i];
    AddComponent(DeserializeComponent(component, *this));
}
    }

    /*!
        Serializes an Entity into an Archetype.

        \param filename
            A std::string filename that we want to write the Entity to.
    */
    void Entity::CreateArchetype(std::string filename) {
        Serializer archetype;
        Serialize(archetype);
        archetype.WriteFile(filename);
    }

    /*!
        Update function for an Entity.  Updates all components attached to it.
    */
    void Entity::Update() {
        for (Component* component : Components)
            component->Update();
    }

    /*!
        Returns the ID of this Entity.

        \return
            A long unsigned integer that is this Entity's ID.
    */
    long unsigned Entity::GetID() const {
        return ID;
    }

    /*!
        Returns the name of this Entity.

        \return
            A string that is this Entity's name.
    */
    std::string Entity::GetName() const {
        return Name;
    }

    /*!
        Sets the name of this Entity.

        \param name
            A string that is this Entity's new name.
    */
    void Entity::SetName(std::string name) {
        Name = name;
    }

    /*!
        Add a Component to this Entity.

        \param component
            A pointer to the component to add.

    */
    bool Entity::AddComponent(Component* component) {
        if (static_cast<bool>(ComponentMask & component->GetType())) {
            delete component;
            return false;
        }
        if ((ComponentMask & component->GetRequired()) != component->GetRequired()) {
            delete component;
            return false;
        }

        ComponentMask |= component->GetType();
        Components.push_back(component);

        return true;
    }

    /*!
        Delete a Component from this Entity.

        \param type
            A ComponentType of the Component we want to delete.
    */
    void Entity::DeleteComponent(ComponentType type) {
        for (unsigned i = 0; i < Components.size(); ++i) {
            if (Components[i]->GetType() == type) {
                delete Components[i];
                Components.erase(Components.begin() + i);
                ComponentMask = static_cast<ComponentType>(static_cast<unsigned long long int>(ComponentMask) - static_cast<unsigned long long int>(type));
                break;
            }
        }
    }

    void Entity::DeleteAllComponents() {
        for (Component* component : Components) {
            delete component;
        }
        Components.clear();
        ComponentMask = ComponentType::None;
    }

    /*!
        Get the component mask from this Entity.

        \return
            A ComponentType that is this Entity's ComponentMask
    */
    ComponentType Entity::GetComponentMask() { return ComponentMask; }

    /*!
        Function that checks to see if the Entity has a Component or Components
        registered with it.

        \param type
            A ComponentType that we want to check for.

        \return
            A boolean value--true if the Entity has the Component, false otherwise.
    */
    bool Entity::HasComponent(ComponentType type) {
        if ((type & ComponentMask) == type) return true;
        else return false;
    }

    /*!
        Returns a unique ID for a new object.

        \return
            An unsigned long integer to be used as the ID.
    */
    unsigned long Entity::GetNewID() {
        if (IDs.size() > MaxEntities / 2) MaxEntities = MaxEntities * 10 + 9;
        if (IDs.size() >= MaxEntities)    throw OutOfIDsException(MaxEntities); // This should never happen

        std::uniform_int_distribution<unsigned long> distribution(1, MaxEntities);

        while (true) {
            auto result = IDs.insert(distribution(RandomEngine));

            if (result.second && *(result.first) != 0) {
                #ifdef FLE_DEBUG
                    std::cout << "Issued ID: " << *(result.first) << std::endl;
                #endif
                return *(result.first);
            }
        }
    }

    /*!
        Frees up a given ID.

        \param id
            An unsigned long ID to free up.
    */
    void Entity::FreeID(unsigned long id) {
        #ifdef FLE_DEBUG
            std::cout << "Freeing ID: " << id << std::endl;
        #endif
        IDs.erase(id);
    }

    /*!
        Helper function to aid in adding multiple Components to an Entity at the
        same time.

        \param mask
            The types of all of the components to add to the Entity, OR'd together.

        \param entity
            A pointer to the entity to add the Components to.
    */
    void AddComponentsToEntity(ComponentType mask, Entity* entity) {
        if (!static_cast<bool>(mask))
            throw AddComponentException(mask, entity->GetID(), "No component provided.");

        if (static_cast<bool>(mask & ComponentType::Transform))
            entity->AddComponent(new Components::Transform(*entity));
        if (static_cast<bool>(mask & ComponentType::Physics))
            entity->AddComponent(new Components::Physics(*entity));
        if (static_cast<bool>(mask & ComponentType::Collision))
            entity->AddComponent(new Components::Collision(*entity));
        else
            throw AddComponentException(mask, entity->GetID(), "Unimplemented component.");
    }

    Component* DeserializeComponent(Serializer& root, Entity& entity) {
        unsigned long long type;
        root.ReadUlonglong("Type", type);

        Component* component = 0;

        switch (static_cast<ComponentType>(type)) {
            case ComponentType::Camera:
                component = new Components::Camera(entity, 0, 0, 0);
                break;
            case ComponentType::Collision:
                component = new Components::Collision(entity);
                break;
            case ComponentType::Model:
                component = new Components::Model(entity, true, "", "");
                break;
            case ComponentType::Physics:
                component = new Components::Physics(entity);
                break;
            case ComponentType::Sprite:
                component = new Components::Sprite(entity);
                break;
            case ComponentType::SpriteText:
                component = new Components::SpriteText(entity, "");
                break;
            case ComponentType::Transform:
                component = new Components::Transform(entity);
                break;
            case ComponentType::PlayerController:
                component = Components::CharacterController::Create(entity);
                break;
            case ComponentType::Light:
                component = new Components::Light(entity);
                break;
            case ComponentType::SoundEmitter:
                component = new Components::SoundEmitter(entity);
                break;
            case ComponentType::DragWithMouse:
                component = Components::DragWithMouse::Create(entity);
                break;
            case ComponentType::Menu:
                component = new Components::Menu(entity);
                break;
            case ComponentType::ScaleWithKeyboard:
                component = Components::ScaleWithKeyboard::Create(entity);
                break;
            case ComponentType::TransformModeControls:
                component = new Components::TransformModeControls(entity);
                component->Initialize();
                break;
            case ComponentType::VisionCone:
                component = new Components::VisionCone(entity);
                break;
            case ComponentType::EnemyAI:
                component = Components::EnemyAI::Create(entity);
                break;
            case ComponentType::FadeWithDistance:
                component = new Components::FadeWithDistance(entity);
                break;
            case ComponentType::Follow:
                component = new Components::Follow(entity);
                break;
            case ComponentType::ChangeLevelOnCollide:
                component = new Components::ChangeLevelOnCollide(entity);
                component->Initialize();
                break;
            case ComponentType::BackgroundMusic:
                component = new Components::BackgroundMusic(entity);
                break;
            case ComponentType::ParticleSystem:
                component = new Components::ParticleSystem(entity);
                break;
            case ComponentType::ParticleEmitter:
                component = new Components::ParticleEmitter(entity);
                break;
            case ComponentType::ParticleColorAnimator:
                component = new Components::ParticleColorAnimator(entity);
                break;
            case ComponentType::SimpleParticleDynamics:
                component = new Components::SimpleParticleDynamics(entity);
                break;
            case ComponentType::Checkpoint:
                component = Components::Checkpoint::Create(entity);
                break;
            case ComponentType::Occluder:
                component = new Components::Occluder(entity);
                break;
            case ComponentType::Parallax:
                component = new Components::Parallax(entity);
                break;
            case ComponentType::Health:
                component = new Components::Health(entity);
                component->Initialize();
                break;
            case ComponentType::EnemyPace:
                component = Components::EnemyPace::Create(entity);
                break;
            case ComponentType::OwlAI:
                component = Components::OwlAI::Create(entity);
                break;
            case ComponentType::MovingPlatform:
                component = new Components::MovingPlatform(entity);
                break;
            case ComponentType::UsefulObject:
                component = Components::UsefulObject::Create(entity);
                break;
            case ComponentType::UsefulObjectInventory:
                component = Components::UsefulObjectInventory::Create(entity);
                break;
            case ComponentType::DamageOnCollide:
                component = new Components::DamageOnCollide(entity);
                component->Initialize();
                break;
            case ComponentType::FinaleOne:
                component = Components::FinaleOne::Create(entity);
                break;
            default:
                std::cout << "Unknown Type: " << type << std::endl;
                return 0;
        }

        component->Deserialize(root);

        return component;
    }

} // ForLeaseEngine
