//
// Created by danil on 11.11.2022.
//

#ifndef MEDGRAPHICS_GLFW_WINDOW_PROVIDER_H
#define MEDGRAPHICS_GLFW_WINDOW_PROVIDER_H

#include <string>
#include "GL/glew.h"
#include "GLFWWindowWrapper.h"
#include "ContextCallbacks.h"

typedef unsigned int uint;

class GLFWWindowProvider {
public:
    static GLFWWindowWrapper*
    createWindow(ContextCallbacks* callbacks, uint width, uint height, const std::string& title, bool vsync);
};


#endif //MEDGRAPHICS_GLFW_WINDOW_PROVIDER_H
