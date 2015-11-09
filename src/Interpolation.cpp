#include "Interpolation.h"
#include <cmath>

namespace ForLeaseEngine {
    float Interpolation::Linear(float v1, float v2, float t) {
        return v1 + t * (v2 - v1);
    }

    float Interpolation::Exponential(float v1, float v2, float t) {
        float e = 2.718281828459045235360287;

        return v1 + (v2 - v1) * (pow(e, t) - 1) / (e - 1);
    }

    float Interpolation::Logarithmic(float v1, float v2, float t) {
        v1 + (v2 - v1) * log(t + 1) / log(2);
    }

    float Interpolation::Trigonomentric(float v1, float v2, float t) {
        float pi = 3.14159265358979323846264;

        return v1 + (v2 - v1) / 2 * (-cos(pi * t) + 1);
    }

    float Interpolation::PowerIn(int power, float v1, float v2, float t) {
        return v1 + (v2 - v1) * abs(pow(t - 1, power));
    }

    float Interpolation::PowerOut(int power, float v1, float v2, float t) {
        return v1 + (v2 - v1) * (-pow(t, power) + 1);
    }
}
