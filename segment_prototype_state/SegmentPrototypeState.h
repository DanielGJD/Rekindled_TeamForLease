#ifndef SEGMENT_PROTOTYPE_H
#define SEGMENT_PROTOTYPE_H

#include "State.h"
#include "Engine.h"

namespace ForLeaseEngine {
    class SegmentPrototypeState : public ForLeaseEngine::State {
        public:
            //TestingState();
            void Load();
            void Initialize();
            void Update();
            void Deinitialize();
            void Unload();
        private:
    };
}

#endif // SEGMENT_PROTOTYPE_H
