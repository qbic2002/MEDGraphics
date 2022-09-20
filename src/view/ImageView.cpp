//
// Created by danil on 18.09.2022.
//

#include "ImageView.h"
#include "GL/glew.h"

namespace view {

    GLuint squareBuffer;
    GLuint squareVao;

    float squareData[] = {
            -1, -1, 1, 1, 1, 1, 1, 0, 1,
            1, -1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 0,
            -1, 1, 1, 1, 1, 1, 1, 0, 0
    };

    void createSquareVao() {
        int vertexSize = 9 * sizeof(float);

        glGenBuffers(1, &squareBuffer);

        glBindBuffer(GL_ARRAY_BUFFER, squareBuffer);
        glBufferData(GL_ARRAY_BUFFER, 4 * vertexSize, squareData, GL_STATIC_DRAW);

        glGenVertexArrays(1, &squareVao);
        glBindVertexArray(squareVao);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, (void*) nullptr);
        glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, vertexSize, (void*) 12);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertexSize, (void*) 28);

        glBindBuffer(GL_ARRAY_BUFFER, 0);

        glBindVertexArray(0);
    }

    ImageView::ImageView(Context* context, unsigned int x, unsigned int y, unsigned int width, unsigned int height)
            : View(context, x, y, width, height) {
        createSquareVao();
    }

    void ImageView::render() {
        shader->useProgram();
        glBindTexture(GL_TEXTURE_2D, context->textureId);
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
            glBindVertexArray(squareVao);
            glDrawArrays(GL_QUADS, 0, 4);
            glBindVertexArray(0);
        }
        glPopMatrix();
        glUseProgram(0);
    }

    ImageView::~ImageView() {
        glDeleteBuffers(1, &squareBuffer);
        glDeleteVertexArrays(1, &squareVao);
        delete shader;
    }

    ImageView::ImageView(Context* _context) : View(_context) {
        createSquareVao();
    }
} // view