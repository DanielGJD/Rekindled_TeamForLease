/*!
    \file   Color.cpp
    \author Christopher Hudson
    \date   10/03/15

    \brief
        Implementation of a class to store colors in 0 - 1 format.
*/

#include "Color.h"

namespace ForLeaseEngine {
    /*!
        \brief
            Creates a color with all components full (White)
    */
    Color::Color() : ColorComponents{1, 1, 1, 1} {}

    /*!
        \brief
            Creates a color with given values and full alpha

        \param r
            Red component

        \param g
            Green component

        \param b
            Blue component
    */
    Color::Color(float r, float g, float b) {
        SetAll(r, g, b, 1);
    }

    /*!
        \brief
            Creates a color with given values

        \param r
            Red component

        \param g
            Green component

        \param b
            Blue component

        \param a
            Alpha component
    */
    Color::Color(float r, float g, float b, float a) {
        SetAll(r, g, b, a);
    }

    void Color::Serialize(Serializer& root) {
        Serializer color = root.GetChild("Color");
        color.WriteFloat("R", GetR());
        color.WriteFloat("G", GetG());
        color.WriteFloat("B", GetB());
        color.WriteFloat("A", GetA());
        root.Append(color, "Color");
    }

    void Color::Deserialize(Serializer& root) {
        Serializer color = root.GetChild("Color");
        float component;
        color.ReadFloat("R", component);
        SetR(component);
        color.ReadFloat("G", component);
        SetG(component);
        color.ReadFloat("B", component);
        SetB(component);
        color.ReadFloat("A", component);
        SetA(component);
    }

    /*!
        \brief
            Gets the red component

        \return
            Red component
    */
    float Color::GetR() const { return ColorComponents[0]; }

    /*!
        \brief
            Gets the green component

        \return
            Green component
    */
    float Color::GetG() const { return ColorComponents[1]; }

    /*!
        \brief
            Gets the blue component

        \return
            Blue component
    */
    float Color::GetB() const { return ColorComponents[2]; }

    /*!
        \brief
            Gets the alpha component

        \return
            Alpha component
    */
    float Color::GetA() const { return ColorComponents[3]; }

    /*!
        \brief
            Sets the red component

        \param r
            Red component
    */
    void Color::SetR(float r) {
        if(r > 1)
            r = 1;
        if(r < 0)
            r = 0;

        ColorComponents[0] = r;
    }

    /*!
        \brief
            Sets the green component

        \param g
            Green component
    */
    void Color::SetG(float g) {
        if(g > 1)
            g = 1;
        if(g < 0)
            g = 0;

        ColorComponents[1] = g;
    }

    /*!
        \brief
            Sets the blue component

        \param b
            Blue component
    */
    void Color::SetB(float b) {
        if(b > 1)
            b = 1;
        if(b < 0)
            b = 0;

        ColorComponents[2] = b;
    }

    /*!
        \brief
            Sets the alpha component

        \param a
            Alpha component
    */
    void Color::SetA(float a) {
        if(a > 1)
            a = 1;
        if(a < 0)
            a = 0;

        ColorComponents[3] = a;
    }

    /*!
        \brief
            Sets all the components

        \param r
            Red component

        \param g
            Green component

        \param b
            Blue component

        \param a
            Alpha component
    */
    void Color::SetAll(float r, float g, float b, float a) {
        SetR(r);
        SetG(g);
        SetB(b);
        SetA(a);
    }
}
