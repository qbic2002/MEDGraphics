//
// Created by danil on 17.09.2022.
//

#include "Context.h"
#include "ImageView.h"
#include "BgRenderer.h"
#include "../img/imageLoader.h"
#include "../img/Raster.h"

namespace view {

    GLuint loadTexture(AbstractRaster* raster) {
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, raster->getWidth(), raster->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
                     raster->getRgbaData());
        glBindTexture(GL_TEXTURE_2D, 0);
        return texture;
    }

    BgRenderer* bgRenderer;

    Context::Context(const std::string& fileName) {
        bgRenderer = new BgRenderer(this);
        views.push_back(bgRenderer);
        views.push_back(new ImageView(this, 0u, 0u, 0u, 0u));
        raster = img::loadImageData(fileName);
        textureId = loadTexture(raster);
    }

    void Context::update() {

    }

    void Context::render() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureId);

        for (auto* view: views) {
            view->render();
        }

//        bgRenderer.render();
//        imageView.render();

        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);
    }

    void Context::onWindowResize(unsigned int width, unsigned int height) {
        glLoadIdentity();
        glOrtho(0, width, 0, height, -100, 100);
//        bgRenderer.update(width, height);
        bgRenderer->update(width, height);
    }

    Context::~Context() {
//        delete bgRenderer;
//        delete imageView;
        for (auto* view: views) {
            delete view;
        }
        delete raster;
    }
} // view