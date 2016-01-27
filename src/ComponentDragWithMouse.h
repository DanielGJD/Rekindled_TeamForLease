/*!
    \file   ComponentDragWithMouse.h
    \author Christopher Hudson

    \brief
        Defines a component for dragging an object with the mouse

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef DRAG_WITH_MOUSE_H
#define DRAG_WITH_MOUSE_H

#include "Component.h"
#include "Event.h"

namespace ForLeaseEngine {
    namespace Components {
        class DragWithMouse : public Component {
            public:
                static DragWithMouse* Create(Entity& owner);
                ~DragWithMouse();
                virtual ComponentType GetType();
                void Update();
                void Serialize(Serializer& root);
                void Deserialize(Serializer& root);

                void OnMouseDown(Event const* e);
                void OnMouseUp(Event const* e);
                void OnMouseMove(Event const* e);

                static const ComponentType Type = ComponentType::DragWithMouse;
                bool Active;
            private:
                DragWithMouse(Entity& owner);
                void Initialize();
                bool MouseDown;
                Point Target;
        };
    }
}

#endif // DRAG_WITH_MOUSE_H
