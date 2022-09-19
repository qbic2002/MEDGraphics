//
// Created by danil on 17.09.2022.
//

#include "Context.h"
#include "../img/imageLoader.h"

namespace view {

    GLuint loadTexture(unsigned char const* data, int width, int height) {
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glBindTexture(GL_TEXTURE_2D, 0);
        return texture;
    }

    Context::Context(const std::string& fileName) {
        raster = img::loadImageData(fileName);
        textureId = loadTexture(raster->getRgbaData(), raster->getWidth(), raster->getHeight());
    }

    void Context::update() {

    }

    void Context::render() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureId);

        bgRenderer.render();
        imageView.render();

        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);
    }

    void Context::onWindowResize(unsigned int width, unsigned int height) {
        glLoadIdentity();
        glOrtho(0, width, 0, height, -100, 100);
        bgRenderer.update(width, height);
    }

    Context::~Context() {
        delete raster;
    }
} // view