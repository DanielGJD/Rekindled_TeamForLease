

#ifndef LEVEL_H
#define LEVEL_H

#include "State.h"
#include <string>

namespace ForLeaseEngine {

    class Level : public State {
        public:
            Level(std::string file);
            ~Level();
            virtual void Load();
            virtual void Initialize();
            virtual void Update();
            virtual void Deinitialize();
            virtual void Unload();

        private:
            std::string File;
    };

}

#endif // LEVEL_H