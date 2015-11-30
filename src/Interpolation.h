/*!
    \file   Interpolation.h
    \author Christopher Hudson

    \brief
        Defines functions for interpolating between two values

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef INTERPOLATION_H
#define INTERPOLATION_H

namespace ForLeaseEngine {
    class Interpolation {
        public:
            static float Linear(float v1, float v2, float t);
            static float Exponential(float v1, float v2, float t);
            static float Logarithmic(float v1, float v2, float t);
            static float Trigonomentric(float v1, float v2, float t);
            static float PowerIn(int power, float v1, float v2, float t);
            static float PowerOut(int power, float v1, float v2, float t);
        private:
            Interpolation();
    };
}

#endif // INTERPOLATION_H
