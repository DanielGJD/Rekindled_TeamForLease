/*!
    \file   Window.h
    \author Christopher Hudson
    \date   09/12/15

    \brief
        Defines a class that handles the creation of a window and rendering
        context and a class to hold window properties

    \copyright ©Copyright 2015 DigiPen Institute of Technology, All Rights Reserved
*/

#ifndef WINDOW_H
#define WINDOW_H

#include <string>
#include <SDL.h>
#include <SDL_opengl.h>

namespace ForLeaseEngine {
    namespace Systems {
        /*!
            \class WindowProperties

            \brief
                Contains data on the current Window properties
        */
        class WindowProperties {
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

                WindowProperties();
                WindowProperties(char* cfgFile);

                void WriteCfg();
                void WriteCfg(char* cfgFile);

                void ReadCfg();
                void ReadCfg(char* cfgFile);

            private:
                static char const* const DEFAULT_FILENAME;
        };


        /*!
            \class Window

            \brief
                Creates and manages a window and rendering context
        */
        class Window {
            public:
                static Window* CreateGameWindow();
                static Window* CreateGameWindow(WindowProperties& properties);
                static void DestroyGameWindow(Window* window);

                //int SetProperties(GraphicsProperties &properties);
                void RequestNewContext();
                void UpdateGameWindow();
                int GetXResolution();
                int GetYResolution();

                SDL_Window* DangerousGetRawWindow();

            private:
                //! Major OpenGL version
                static const int FLE_GL_MAJOR_VERSION = 2;
                //! Minor OpenGL version
                static const int FLE_GL_MINOR_VERSION = 1;
                //! Application window
                SDL_Window* window;
                //! OpenGL context
                SDL_GLContext context;
                //! Current window properties
                WindowProperties currentProperties;

                Window();
                Window(WindowProperties& properties);
                ~Window();

                void InitGameWindow(WindowProperties& properties);
                void DestroyGameWindow();
        };
    }
}

#endif
