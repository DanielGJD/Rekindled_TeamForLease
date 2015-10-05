/*!
    \file   Window.cpp
    \author Christopher Hudson
    \date   09/12/15

    \brief
        Implementation of a class that handles the creation of a window and
        rendering context

    \see Window.h
*/
#include <iostream>
#include <fstream>
#include <SDL.h>
#include "GraphicsException.h"
#include "Window.h"

namespace ForLeaseEngine {
    namespace Systems {
        char const * const WindowProperties::DEFAULT_FILENAME = "Window.cfg";

        /*!
            \brief
                Creates a WindowProperties containing the default properties
        */
        WindowProperties::WindowProperties() : windowTitle("For Lease Engine") {
            xResolution = 800;
            yResolution = 600;
            visible = true;
            fullscreen = false;
            borderless = false;
            resizeable = false;
            mouseConstrained = false;
        }

        void WindowProperties::WriteCfg() {
            std::ofstream out;
            out.open(DEFAULT_FILENAME, std::ofstream::out);
            out << windowTitle << "\n"
                << xResolution << "\n"
                << yResolution << "\n"
                << visible << "\n"
                << fullscreen << "\n"
                << borderless << "\n"
                << resizeable << "\n"
                << mouseConstrained;
        }

        void WindowProperties::ReadCfg() {
            std::ifstream in;
            in.open(DEFAULT_FILENAME, std::istream::in);
            std::getline(in, windowTitle);
            in >> xResolution;
            in >> yResolution;
            in >> visible;
            in >> fullscreen;
            in >> borderless;
            in >> resizeable;
            in >> mouseConstrained;
            in.close();

            WriteCfg();
        }

        /*!
            \brief
                Creates a window and OpenGL context with default properties
        */
        Window::Window() {
            WindowProperties properties = WindowProperties();
            currentProperties = properties;
            InitGameWindow(properties);
        }

        /*!
            \brief
                Creates a window and OpenGL context with given properties

            \param properties
                Properties for the window
        */
        Window::Window(WindowProperties& properties) : currentProperties(properties) {
            //properties.ReadCfg();
            InitGameWindow(properties);
        }

        /*!
            \brief
                Cleans up window and closes SDL Video module
        */
        Window::~Window() {
            //currentProperties.WriteCfg();
            DestroyGameWindow();
        }

        /*!
            \brief
                Creates a default Window object

            \return
                The new graphics object
        */
        Window* Window::CreateGameWindow() {
            return new Window();
        }

        /*!
            \brief
                Creates a window object with the given properties

            \param properties
                Properties of the window

            \return
                The new window object
        */
        Window* Window::CreateGameWindow(WindowProperties& properties) {
            return new Window(properties);
        }

        /*!
            \brief
                Destroys the given window object

            \param window
                Window object to destroy
        */
        void Window::DestroyGameWindow(Window* window) {
            delete window;
        }

        /*!
            \brief
                Creates a new OpenGL context and destroys the old one
        */
        void Window::RequestNewContext() {
            if(!window) {
                throw GraphicsException("Tried to create context without window");
            }

            if(context) {
                SDL_GL_DeleteContext(context);
            }

            context = SDL_GL_CreateContext(window);
            if(!context) {
                std::string errorMessage = "Failed to create OpenGL context: ";
                errorMessage.append(SDL_GetError());
                throw GraphicsException(errorMessage);
            }
        }

        /*!
            \brief
                Swaps the window buffer, called to display the draw image
        */
        void Window::UpdateGameWindow() {
            SDL_GL_SwapWindow(window);
            glClear(GL_COLOR_BUFFER_BIT);
        }

        /*!
            \brief
                Initializes SDL Video, creates a window, and creates an OpenGL context

            \param properties
                Properties for the window
        */
        void Window::InitGameWindow(WindowProperties& properties) {
            if(SDL_InitSubSystem(SDL_INIT_VIDEO) != 0) {
                std::string errorMessage = "Failed to initialize SDL Video: ";
                errorMessage.append(SDL_GetError());
                throw GraphicsException(errorMessage);
            }

            int windowOptions = SDL_WINDOW_OPENGL;

            if(properties.visible)
                windowOptions |= SDL_WINDOW_SHOWN;
            if(properties.fullscreen)
                windowOptions |= SDL_WINDOW_FULLSCREEN;
            if(properties.borderless)
                windowOptions |= SDL_WINDOW_BORDERLESS;
            if(properties.resizeable)
                windowOptions |= SDL_WINDOW_RESIZABLE;
            if(properties.mouseConstrained)
                windowOptions |= SDL_WINDOW_INPUT_GRABBED;

            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,
                                FLE_GL_MAJOR_VERSION);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,
                                FLE_GL_MINOR_VERSION);
            SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
            SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

            window = SDL_CreateWindow(properties.windowTitle.c_str(),
                                      SDL_WINDOWPOS_UNDEFINED,
                                      SDL_WINDOWPOS_UNDEFINED,
                                      properties.xResolution,
                                      properties.yResolution,
                                      windowOptions);
            if(!window) {
                std::string errorMessage = "Failed to create window: ";
                errorMessage.append(SDL_GetError());
                DestroyGameWindow();
                throw GraphicsException(errorMessage);
            }

            context = SDL_GL_CreateContext(window);
            if(!context) {
                std::string errorMessage = "Failed to create OpenGL context: ";
                errorMessage.append(SDL_GetError());
                DestroyGameWindow();
                throw GraphicsException(errorMessage);
            }

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            //std::cout << glGetString(GL_VERSION) << std::endl;
            //std::cout << glGetString(GL_EXTENSIONS) << std::endl;
        }

        /*!
            \brief
                Destroys the OpenGL context, window, and quits the SDL Video module
        */
        void Window::DestroyGameWindow() {
            if(context)
                SDL_GL_DeleteContext(context);
            if(window)
                SDL_DestroyWindow(window);
            window = NULL;
            SDL_QuitSubSystem(SDL_INIT_VIDEO);
        }
    }
}
