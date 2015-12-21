#include "Random.h"

namespace ForLeaseEngine {
    void RandomSeed() {
        srand(time(NULL));
    }

    int RandomInt(int min, int max) {
        return rand() % (max - min) + min;
    }

    float RandomFloat(float min, float max) {
        return static_cast<float>(rand()) / RAND_MAX * (max - min) + min;
    }
}
