/*!
    \file   Component.h
    \author Sean McGeer
    \date   9/18/15
    \brief
        Defines the Component class.  Also defines the ComponentType enum and
        corresponding operator overloads.
    \see Component.cpp

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef COMPONENT_H
#define COMPONENT_H

#include "Serializable.h"
#include "Serialize.h"
#include <iostream>

namespace ForLeaseEngine {
    class Entity; // Forward declaration for Entity

    /*!
        \enum ComponentType
        \brief
            The type of a component.  We treat it as an unsigned long to allow
            for 64 components.  This is unlikely to be enough for a game of any
            scale unless we implement scripting or some other solution for
            loading game code.
    */
    enum class ComponentType : unsigned long long {
        None                   = 0,         //! No component
        Transform              = 1 <<  0,   //! Transform
        PlayerController       = 1 <<  1,   //! Player controls
        Physics                = 1 <<  2,   //! Physics
        Collision              = 1 <<  3,   //! Collision
        Camera                 = 1 <<  4,   //! Camera
        Sprite                 = 1 <<  5,   //! Sprite
        Model                  = 1 <<  6,   //! Model
        SpriteText             = 1 <<  7,   //! Sprite text
        Renderer               = 1 <<  8,   //! Renderer
        Light                  = 1 <<  9,   //! Light
        SoundEmitter           = 1 << 10,   //! Sound Emitter
        DragWithMouse          = 1 << 11,   //! Item that can be dragged by the mouse
        ScaleWithKeyboard      = 1 << 12,   //! Item that can be scaled by the keyboard
        TransformModeControls  = 1 << 13,   //! Controls for the transform mode of the main character
        Menu                   = 1 << 14,   //! Menu
        VisionCone             = 1 << 15,   //! Used for detecting objects in a radius within an angle
        EnemyAI                = 1 << 16,   //! Very basic ai, does not move, just detects player and signals loss condition
        FadeWithDistance       = 1 << 17,   //! Causes and object's sprite to fade out with distance from an entity
        Follow                 = 1 << 18,   //! Entity will follow another entity
        ChangeLevelOnCollide   = 1 << 19,   //! Colliding with this entity will change the level
        BackgroundMusic        = 1 << 20,   //! Controls the background music
        ParticleSystem         = 1 << 21,   //! Collection of particles
        ParticleEmitter        = 1 << 22,   //! Emits particles from a system
        SimpleParticleDynamics = 1 << 23,   //! Adds dynamics to a particle system
        ParticleColorAnimator  = 1 << 24,   //! Adds color animation to particles
        Parallax               = 1 << 25,   //! Parallax scrolls the object
        Occluder               = 1 << 26,   //! Blocks light or vision
        Checkpoint             = 1 << 27,   //! Saves player position for later loading
        Health                 = 1 << 28,   //! Keeps track of health
        EnemyPace              = 1 << 29,   //! Has an enemy pace back and forth
        OwlAI                  = 1 << 30    //! Moves the vision cone from place to place
    };

    ComponentType operator|(const ComponentType& lhs, const ComponentType& rhs);
    ComponentType operator&(const ComponentType& lhs, const ComponentType& rhs);
    ComponentType& operator|=(ComponentType& lhs, const ComponentType& rhs);
    std::ostream& operator<<(std::ostream& os, const ComponentType& type);

    /*!
        \class Component
        \brief
            A class that defines a Component.  All other components should
            inherit from this.
    */
    class Component : public Serializable
    {

        public:
            static const ComponentType Type = ComponentType::None;
            virtual ComponentType GetType() { return Type; }
            Component(Entity& parent, ComponentType required = ComponentType::None);
            virtual ~Component() {};
            // ComponentType GetType();
            virtual void Serialize(Serializer& root);
            virtual void Deserialize(Serializer& root);
            ComponentType GetRequired();
            virtual void Initialize() {};
            virtual void Update() = 0;
        protected:
            Entity& Parent;
        private:
            // const ComponentType Type;     //! The type of component.
            const ComponentType Required; //! A mask of all components required for this component

    };

} // ForLeaseEngine

#endif
