#ifndef SCRIPT_H
#define SCRITP_H

#include "ComponentsInclude.h"
#include "Engine.h"
#include "LevelComponentsInclude.h"
#include "Event.h"
#include "UpdateEvent.h"
#include "KeyboardEvent.h"
#include "MouseButtonEvent.h"
#include "Keys.h"
#include "Mouse.h"
#include "Input.h"
#include "Vector.h"
#include "Color.h"
#include "Font.h"
#include "Mesh.h"
#include "Texture.h"
#include "TextureRegion.h"
#include "ResourceManager.h"
#include "FrameRateController.h"
#include "Window.h"
#include "Font.h"
#include "Entity.h"
#include "EventDispatcher.h"

using namespace ForLeaseEngine;
using namespace LevelComponents;
using namespace Components;

namespace ForLeaseEngine {
    class Script {
        public:
            Entity* Owner;
            Script(Entity* owner) : Owner(owner) {}
            virtual void Initialize() = 0;
            virtual ~Script() {}
    };
}

#endif // SCRIPT_H
