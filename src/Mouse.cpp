#include "Mouse.h"

namespace ForLeaseEngine {
    namespace Mouse {
        void SetMouseVisible(bool visible) {
            if(visible) {
                SDL_ShowCursor(SDL_ENABLE);
            }
            else {
                SDL_ShowCursor(SDL_DISABLE);
            }
        }

        void ToggleMouseVisible() {
            int state = SDL_ShowCursor(SDL_QUERY);
            SetMouseVisible(state == SDL_DISABLE);
        }
    }
}
