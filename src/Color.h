/*!
    \file   Color.h
    \author Christopher Hudson
    \date   10/03/15

    \brief
        Defines a class to store colors in 0 - 1 format.
*/

#ifndef COLOR_H
#define COLOR_H

namespace ForLeaseEngine {
    /*!
        \class Color

        \brief
            A class that holds color data in 0 - 1 format.
    */
    class Color {
        public:
//            static Color Red;
//            static Color Orange;
//            static Color Yellow;
//            static Color Green;
//            static Color Blue;
//            static Color Purple;
//            static Color White;
//            static Color LightGray;
//            static Color Gray;
//            static Color DarkGrey;
//            static Color Black;


            Color();
            Color(float r, float g, float b);
            Color(float r, float g, float b, float a);
            float GetR();
            float GetG();
            float GetB();
            float GetA();
            void SetR(float r);
            void SetG(float g);
            void SetB(float b);
            void SetA(float a);
            void SetAll(float r, float g, float b, float a);

        private:
            float ColorComponents[4]; /**< Components of the color */
    };

    enum BlendMode {
        NONE,
        ALPHA,
        ADDITIVE,
        MULTIPLY
    };
}

#endif // COLOR_H
