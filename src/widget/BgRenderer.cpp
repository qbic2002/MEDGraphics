//
// Created by danil on 17.09.2022.
//

#include <cmath>
#include "BgRenderer.h"
#include "GL/glew.h"
#include "../utils/random.h"
#include "../utils/measureFps.h"

using namespace utils;

namespace view {
    float calcAngle(float sinValue, float cosValue) {
        float a = std::acos(cosValue);
        if (sinValue < 0) {
            return -a;
        }

        return a;
    }

    vec3 randomVec3(float minR, float maxR) {
        float r = random(minR, maxR);
        float a = random() * M_PI * 2;
        return vec3(std::cos(a), std::sin(a), 0) * r;
    }

    rotation randomRotation() {
        float u = randomFloat();
        float v = randomFloat();
        float w = randomFloat();

        float sinHalfA = std::sqrt(1 - u + u * std::pow(std::sin(2 * M_PI * w), 2));
        float cosHalfA = std::sqrt(u) * std::cos(2 * M_PI * w);
        float angle = calcAngle(sinHalfA, cosHalfA) * 2;

        float x = std::sqrt(1 - u) * std::sin(2 * M_PI * v) / sinHalfA;
        float y = std::sqrt(1 - u) * std::cos(2 * M_PI * v) / sinHalfA;
        float z = std::sqrt(u) * std::sin(2 * M_PI * w) / sinHalfA;

        return {.axis= vec3(x, y, z),
                .angle = angle};
    }

    rectVertex* randomRectVertices(int rectsCount, unsigned int width, unsigned int height) {
        auto* rectVertices = new rectVertex[rectsCount * 4];

        for (int i = 0; i < rectsCount; i++) {
            rectVertex vert{
                    .pos = vec3(random(width), random(height), pow(randomFloat(), 2) * 90),
                    .velocity = randomVec3(10, 20),
                    .rotation = randomRotation(),
                    .rotationSpeed = randomFloat(1, 2)
            };
            for (int index = 0; index < 4; index++) {
                rectVertices[i * 4 + index] = vert;
                rectVertices[i * 4 + index].index = index;
            }
        }

        return rectVertices;
    }

    BgRenderer::BgRenderer(Context* context) : View(context) {}

    void BgRenderer::onWindowResize(unsigned int _width, unsigned int _height) {
        clear();
        width = _width;
        height = _height;
        rectsCount = width * height / 5000;
        rectVertices = randomRectVertices(rectsCount, width, height);
        uploadVertexData();
    }

    void BgRenderer::render() {
        glEnable(GL_DEPTH_TEST);
        glBindTexture(GL_TEXTURE_2D, context->getCurrentImageData().textureId);
        rectsShader.useProgram();
        glUniform1f(uniformThetaLoc, getCurrentTime());
        glUniform1i(uniformWidthLoc, width);
        glUniform1i(uniformHeightLoc, height);
        glBindVertexArray(rectsVaoId);
        glDrawArrays(GL_QUADS, 0, rectsCount * 4);
        glBindVertexArray(0);
        glUseProgram(0);
        glDisable(GL_DEPTH_TEST);
    }

    void BgRenderer::uploadVertexData() {
        glGenBuffers(1, &rectsBufferId);
        glBindBuffer(GL_ARRAY_BUFFER, rectsBufferId);

        glBufferData(GL_ARRAY_BUFFER, rectsCount * sizeof(rectVertex) * 4, rectVertices, GL_STATIC_DRAW);

        glGenVertexArrays(1, &rectsVaoId);
        glBindVertexArray(rectsVaoId);

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        glEnableVertexAttribArray(3);
        glEnableVertexAttribArray(4);
        glEnableVertexAttribArray(5);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(rectVertex), (void*) nullptr);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(rectVertex), (void*) 12);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(rectVertex), (void*) 24);
        glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(rectVertex), (void*) 36);
        glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(rectVertex), (void*) 40);
        glVertexAttribIPointer(5, 1, GL_UNSIGNED_BYTE, sizeof(rectVertex), (void*) 44);

        glBindVertexArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void BgRenderer::clear() {
        if (rectsVaoId != 0) {
            glDeleteVertexArrays(1, &rectsVaoId);
            rectsVaoId = 0;
        }
        if (rectsBufferId != 0) {
            glDeleteBuffers(1, &rectsBufferId);
            rectsBufferId = 0;
        }
        if (rectVertices != nullptr) {
            delete[] rectVertices;
            rectVertices = nullptr;
        }
    }

    BgRenderer::~BgRenderer() {
        clear();
    }
}
