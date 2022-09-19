//
// Created by danil on 18.09.2022.
//

#include "ImageView.h"
#include "GL/glew.h"
#include "../utils/windowSize.h"

namespace view {

    void ImageView::render() {
        shader->useProgram();
        glPushMatrix();
        {
            glLoadIdentity();
            float windowRatio = utils::getWindowRatio();
            float ratio = (float) context->raster->getWidth() / context->raster->getHeight() / windowRatio;
            if (ratio < 1) {
                glScalef(ratio, 1, 1);
            } else {
                glScalef(1, 1 / ratio, 1);
            }
            glScalef(0.9, 0.9, 1);
//            glBindVertexArray(squareVao);
//            glDrawArrays(GL_QUADS, 0, 4);
//            glBindVertexArray(0);
        }
        glPopMatrix();
        glUseProgram(0);
    }

    ImageView::~ImageView() {
        delete shader;
    }
} // view