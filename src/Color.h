/*!
    \file   Color.h
    \author Christopher Hudson
    \date   10/03/15

    \brief
        Defines a class to store colors in 0 - 1 format.

    \copyright ęCopyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef COLOR_H
#define COLOR_H

#include "Serializable.h"
#include "Serialize.h"

namespace ForLeaseEngine {
    /*!
        \class Color

        \brief
            A class that holds color data in 0 - 1 format.
    */
    class Color : public Serializable{
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

            void Serialize(Serializer& root);
            void Deserialize(Serializer& root);

            float GetR() const;
            float GetG() const;
            float GetB() const;
            float GetA() const;
            float const* GetAll() const;
            void SetR(float r);
            void SetG(float g);
            void SetB(float b);
            void SetA(float a);
            void SetAll(float r, float g, float b, float a);
            void SetAll(float const* data);

            Color operator+(Color const& rhs);
            Color operator-(Color const& rhs);
            Color operator*(Color const& rhs);

        private:
            float ColorComponents[4]; /**< Components of the color */
    };

    enum BlendMode {
        NONE,
        ALPHA,
        ADDITIVE,
        MULTIPLY,
        LIGHT
    };
}

#endif // COLOR_H
