/*!
    \file   Graphics.h
    \author Christopher Hudson
    \date   09/12/15

    \brief
        Defines a class that handles the creation of a window and rendering
        context and a class to hold window properties

    \see Graphics.cpp
*/

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include <string>
#include <SDL.h>
#include <SDL_opengl.h>

namespace ForLeaseEngine {
    /*!
        \class GraphicsProperties

        \brief
            Contains data on the current graphics properties
    */
    class GraphicsProperties {
        public:
            //! Title of the window
            std::string windowTitle;
            //! X resolution of the window
            int xResolution;
            //! Y resolution of the window
            int yResolution;
            //! If the window is visible
            bool visible;
            //! If the window is fullscreen
            bool fullscreen;
            //! If the window is borderless
            bool borderless;
            //! If the window is resizeable
            bool resizeable;
            //! If the mouse is constrained to the window
            bool mouseConstrained;

            GraphicsProperties();
            GraphicsProperties(char* cfgFile);

            int WriteCfg();
            int WriteCfg(char* cfgFile);

        private:
            static const char* DEFAULT_FILENAME;
    };


    /*!
        \class Graphics

        \brief
            Creates and manages a window and rendering context
    */
    class Graphics {
        public:
            static Graphics* CreateGraphics();
            static Graphics* CreateGraphics(GraphicsProperties& properties);
            static void DestroyGraphics(Graphics* graphics);

            //int SetProperties(GraphicsProperties &properties);
            void RequestNewContext();
            void UpdateWindow();


        private:
            //! Major OpenGL version
            static const int FLE_GL_MAJOR_VERSION = 2;
            //! Minor OpenGL version
            static const int FLE_GL_MINOR_VERSION = 1;
            //! Application window
            SDL_Window* window;
            //! OpenGL context
            SDL_GLContext context;
            /*
            //! Current graphics properties
            GraphicsProperties currentProperties;
            */

            Graphics();
            Graphics(GraphicsProperties &properties);
            ~Graphics();

            void InitGraphics(GraphicsProperties& properties);
            void DestroyGraphics();
    };
}

#endif
