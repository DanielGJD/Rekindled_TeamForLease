/*!
    \file   Graphics.cpp
    \author Christopher Hudson
    \date   09/12/15

    \brief
        Implementation of a class that handles the creation of a window and
        rendering context

    \see Graphics.h
*/
#include <iostream>
#include "SDL.h"
#include "GraphicsException.h"
#include "Graphics.h"

namespace ForLeaseEngine {
    /*!
        \brief
            Creates a GraphicsProperties containing the default properties
    */
    GraphicsProperties::GraphicsProperties() : windowTitle("For Lease Engine") {
        xResolution = 800;
        yResolution = 600;
        visible = true;
        fullscreen = false;
        borderless = false;
        resizeable = false;
        mouseConstrained = false;
    }

    /*!
        \brief
            Creates a window and OpenGL context with default properties
    */
    Graphics::Graphics() {
        GraphicsProperties properties = GraphicsProperties();
        InitGraphics(properties);
    }

    /*!
        \brief
            Creates a window and OpenGL context with given properties

        \param properties
            Properties for the window
    */
    Graphics::Graphics(GraphicsProperties& properties) {
        InitGraphics(properties);
    }

    /*!
        \brief
            Cleans up window and closes SDL Video module
    */
    Graphics::~Graphics() {
        DestroyGraphics();
    }

    /*!
        \brief
            Creates a default Graphics object

        \return
            The new graphics object
    */
    Graphics* Graphics::CreateGraphics() {
        return new Graphics();
    }

    /*!
        \brief
            Creates a graphics object with the given properties

        \param properties
            Properties of the window

        \return
            The new graphics object
    */
    Graphics* Graphics::CreateGraphics(GraphicsProperties& properties) {
        return new Graphics(properties);
    }

    /*!
        \brief
            Destroys the given graphics object

        \param graphics
            Graphics object to destroy
    */
    void Graphics::DestroyGraphics(Graphics* graphics) {
        delete graphics;
    }

    /*!
        \brief
            Creates a new OpenGL context and destroys the old one
    */
    void Graphics::RequestNewContext() {
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
    void Graphics::UpdateWindow() {
        SDL_GL_SwapWindow(window);
    }

    /*!
        \brief
            Initializes SDL Video, creates a window, and creates an OpenGL context

        \param properties
            Properties for the window
    */
    void Graphics::InitGraphics(GraphicsProperties& properties) {
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

        window = SDL_CreateWindow(properties.windowTitle.c_str(),
                                  SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED,
                                  properties.xResolution,
                                  properties.yResolution,
                                  windowOptions);
        if(!window) {
            std::string errorMessage = "Failed to create window: ";
            errorMessage.append(SDL_GetError());
            DestroyGraphics();
            throw GraphicsException(errorMessage);
        }

        context = SDL_GL_CreateContext(window);
        if(!context) {
            std::string errorMessage = "Failed to create OpenGL context: ";
            errorMessage.append(SDL_GetError());
            DestroyGraphics();
            throw GraphicsException(errorMessage);
        }

        std::cout << glGetString(GL_VERSION) << std::endl;
        std::cout << glGetString(GL_EXTENSIONS) << std::endl;
    }

    /*!
        \brief
            Destroys the OpenGL context, window, and quits the SDL Video module
    */
    void Graphics::DestroyGraphics() {
        if(context)
            SDL_GL_DeleteContext(context);
        if(window)
            SDL_DestroyWindow(window);
        window = NULL;
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
    }
}
