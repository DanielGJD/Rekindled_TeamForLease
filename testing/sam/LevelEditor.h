/*!
    \author Sam Montanari

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef LEVELEDITOR_H
#define LEVELEDITOR_H

#include "LevelComponentsInclude.h"
#include "Mesh.h"
#include "Vector.h"
#include "Engine.h"
#include "Event.h"
#include "Entity.h"
#include "ComponentsInclude.h"

namespace ForLeaseEngine
{
    class LevelEditor : public State
    {
    public:
        void Load();
        void Initialize();
        void Update();
        void Deinitialize();
        void Unload();
        void SaveFiles(Serializer& root);
        void LoadFiles(Serializer& root);
        void Input();
    };
}



#endif // LEVELEDITOR_H
