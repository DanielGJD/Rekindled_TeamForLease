#ifndef MESH_EDITOR_STATE_H
#define MESH_EDITOR_STATE_H

#include "State.h"
#include "Mesh.h"
#include "Event.h"

namespace ForLeaseEngine {
    class MeshEditorState : public State {
        public:
            //TestingState();
            void Load();
            void Initialize();
            void Update();
            void Deinitialize();
            void Unload();
        private:
    //        Mesh* LoadedMesh;
    };

    class InputState {
        public:
            std::string Instructions;
            bool Active = false;
            virtual void Initialize() = 0;
    };

    class StartInput : public InputState {
        public:
            StartInput();
            void OnKeyDown(const Event* e);
            void Initialize();
    };

    class VertexModeInput : public InputState {
        public:
            VertexModeInput();
            void Initialize();
    };
}

#endif // MESH_EDITOR_STATE_H
