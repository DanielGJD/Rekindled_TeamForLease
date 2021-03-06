/*!
    \file   Window.cpp
    \author Christopher Hudson
    \date   09/12/15

    \brief
        Implementation of a class that handles the creation of a window and
        rendering context

    \copyright ęCopyright 2015 DigiPen Institute of Technology, All Rights Reserved

    \see Window.h
*/
#include <iostream>
#include <fstream>
#include <SDL.h>
#include <GL/glew.h>
#include "GraphicsException.h"
#include "Window.h"
#include "Engine.h"
#include "Filesystem.h"
#include "WindowEvent.h"
#include <SDL_image.h>

namespace ForLeaseEngine {
    namespace Systems {
        char const * const WindowProperties::DEFAULT_FILENAME = "Window.cfg";

        /*!
            \brief
                Creates a WindowProperties containing the default properties
        */
        WindowProperties::WindowProperties() : windowTitle("Rekindled") {
            xResolution = 1280;
            yResolution = 720;
            visible = true;
            fullscreen = true;
            borderless = false;
            resizeable = false;
            mouseConstrained = false;
        }

        void WindowProperties::WriteCfg() {
            std::ofstream out;
            out.open(ForLease->Filesystem.AssetDirectory(Modules::Filesystem::AssetType::Save).append(DEFAULT_FILENAME), std::ofstream::out);
            out << windowTitle << "\n"
                << xResolution << "\n"
                << yResolution << "\n"
                << visible << "\n"
                << fullscreen << "\n"
                << borderless << "\n"
                << resizeable << "\n"
                << mouseConstrained;

            //std::cout << "Writing window config to " << ForLease->Filesystem.AssetDirectory(Modules::Filesystem::AssetType::Save).append(DEFAULT_FILENAME);
        }

        void WindowProperties::ReadCfg() {
            std::ifstream in;
            in.open(ForLease->Filesystem.AssetDirectory(Modules::Filesystem::AssetType::Save).append(DEFAULT_FILENAME), std::istream::in);
            std::getline(in, windowTitle);
            in >> xResolution;
            in >> yResolution;
            in >> visible;
            in >> fullscreen;
            in >> borderless;
            in >> resizeable;
            in >> mouseConstrained;
            in.close();

            //std::cout << xResolution << "," << yResolution << "," << visible << "," << fullscreen << "," << borderless << "," << resizeable << "," << mouseConstrained << std::endl;

            //WriteCfg();
        }

        /*!
            \brief
                Creates a window and OpenGL context with default properties
        */
        Window::Window() {
            WindowProperties properties = WindowProperties();
            currentProperties = properties;
            InitGameWindow(properties);
            //currentProperties.WriteCfg();
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
            //currentProperties.WriteCfg();
            currentProperties.ReadCfg();
        }

        /*!
            \brief
                Cleans up window and closes SDL Video module
        */
        Window::~Window() {
            currentProperties.WriteCfg();
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
                Gets the X resolution of the window

            \return
                X resolution of the window
        */
        int Window::GetXResolution() { return currentProperties.xResolution; }

        /*!
            \brief
                Gets the Y resolution of the window

            \return
                Y resolution of the window
        */
        int Window::GetYResolution() { return currentProperties.yResolution; }

        bool Window::GetFullscreen() { return currentProperties.fullscreen; }

        void Window::SetResolution(int x, int y) {
            std::cout << "Setting resolution " << x << "," << y << std::endl;
            WindowEvent e = WindowEvent(WindowEvent::FocusLost);
            ForLease->Dispatcher.Dispatch(&e, NULL);

            currentProperties.xResolution = x;
            currentProperties.yResolution = y;

            int windowOptions = SDL_WINDOW_OPENGL;

            if(currentProperties.visible)
                windowOptions |= SDL_WINDOW_SHOWN;
            if(currentProperties.fullscreen)
                windowOptions |= SDL_WINDOW_FULLSCREEN;
            if(currentProperties.borderless)
                windowOptions |= SDL_WINDOW_BORDERLESS;
            if(currentProperties.resizeable)
                windowOptions |= SDL_WINDOW_RESIZABLE;
            if(currentProperties.mouseConstrained)
                windowOptions |= SDL_WINDOW_INPUT_GRABBED;
            SDL_DestroyWindow(window);
            window = SDL_CreateWindow(currentProperties.windowTitle.c_str(),
                                      SDL_WINDOWPOS_UNDEFINED,
                                      SDL_WINDOWPOS_UNDEFINED,
                                      currentProperties.xResolution,
                                      currentProperties.yResolution,
                                      windowOptions);
            SDL_GL_MakeCurrent(window, context);
            glViewport(0, 0, x, y);
        }

        void Window::SetFullscreen(bool fullscreen) {
            std::cout << "Setting fullscreen " << fullscreen << std::endl;
            WindowEvent e = WindowEvent(WindowEvent::FocusLost);
            ForLease->Dispatcher.Dispatch(&e, NULL);
            //ForLease->Dispatcher.Dispatch(&e, NULL);

            currentProperties.fullscreen = fullscreen;

            if(fullscreen) {
                SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
            }
            else {
                SDL_SetWindowFullscreen(window, 0);
            }

//            e = WindowEvent(WindowEvent::FocusGained);
//            ForLease->Dispatcher.Dispatch(&e, NULL);
        }

        const WindowProperties& Window::GetProperties() {
            return currentProperties;
        }

        void Window::SetProperties(const WindowProperties& properties) {
            WindowEvent e = WindowEvent(WindowEvent::FocusLost);
            ForLease->Dispatcher.Dispatch(&e, NULL);

            currentProperties = properties;

            int windowOptions = SDL_WINDOW_OPENGL;

            if(currentProperties.visible)
                windowOptions |= SDL_WINDOW_SHOWN;
            if(currentProperties.fullscreen)
                windowOptions |= SDL_WINDOW_FULLSCREEN;
            if(currentProperties.borderless)
                windowOptions |= SDL_WINDOW_BORDERLESS;
            if(currentProperties.resizeable)
                windowOptions |= SDL_WINDOW_RESIZABLE;
            if(currentProperties.mouseConstrained)
                windowOptions |= SDL_WINDOW_INPUT_GRABBED;
            SDL_DestroyWindow(window);
            window = SDL_CreateWindow(currentProperties.windowTitle.c_str(),
                                      SDL_WINDOWPOS_UNDEFINED,
                                      SDL_WINDOWPOS_UNDEFINED,
                                      currentProperties.xResolution,
                                      currentProperties.yResolution,
                                      windowOptions);
            SDL_GL_MakeCurrent(window, context);
            glViewport(0, 0, currentProperties.xResolution, currentProperties.yResolution);

            // Set window icon
            SDL_Surface* icon = IMG_Load("Rekindled.ico");
            if (icon != NULL) {
                SDL_SetWindowIcon(window, icon);
            }
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
            SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

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
            glEnable(GL_MULTISAMPLE);
            glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
            glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
            glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            //glDisable(GL_BLEND);
            //glDisable(GL_TEXTURE_2D);
            glDisable(GL_ALPHA_TEST);
            glDisable(GL_DITHER);
            glDisable(GL_DEPTH_TEST);
            glDisable(GL_LIGHTING);
            //glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
            //std::cout << glGetString(GL_VERSION) << std::endl;
            //std::cout << glGetString(GL_EXTENSIONS) << std::endl;

            // Initialize glew
            GLenum glewErr = glewInit();
            if(glewErr != GLEW_OK) {
                std::cout << "FAILED TO INITIALIZE GLEW: " << glewGetErrorString(glewErr) << std::endl;
            }
            else {
                std::cout << "GLEW INITIALIZED" << std::endl;
            }

            // Set window icon
            SDL_Surface* icon = IMG_Load("Rekindled.ico");
            if (icon != NULL) {
                SDL_SetWindowIcon(window, icon);
            }
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



        /*!
            \brief
                Gets the raw SDL window, DON'T USE UNLESS YOU KNOW WHAT YOU'RE DOING!
        */
        SDL_Window* Window::DangerousGetRawWindow() {
            return window;
        }
    }
}
