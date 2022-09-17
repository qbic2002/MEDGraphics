#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <vector>
#include "math/vec3.h"
#include "utils/measureFps.h"
#include "utils/windowSize.h"
#include "pnm/PNMImage.h"

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include "pnm/utils/pnmUtil.h"
#include "view/ShaderProgram.h"
#include "view/Context.h"

using namespace std;
using namespace utils;
using namespace view;

GLuint squareVao;
GLuint textureId;
ShaderProgram* shader;
float curRatio;

Context* context;

float squareData[] = {
        -1, -1, 1, 1, 1, 0, 0, 1,
        1, -1, 1, 1, 1, 0, 1, 1,
        1, 1, 1, 1, 1, 0, 1, 0,
        -1, 1, 1, 1, 1, 0, 0, 0
};

GLFWwindow* createWindow(int width, int height, const char* title, bool vsync) {
    GLFWwindow* window;

    if (!glfwInit())
        return nullptr;

    glfwWindowHint(GLFW_SAMPLES, 16);
    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    if (vsync)
        glfwSwapInterval(1);
    else
        glfwSwapInterval(0);
    return window;
}

void update() {
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureId);

    context->bgRenderer.render();

    shader->useProgram();
    glPushMatrix();
    {
        glLoadIdentity();
        float windowRatio = getWindowRatio();
        float ratio = curRatio / windowRatio;
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
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    glUseProgram(0);
}

void createSquareVao() {
    int vertexSize = 8 * sizeof(float);

    GLuint vboHandles[1];
    glGenBuffers(1, vboHandles);

    glBindBuffer(GL_ARRAY_BUFFER, vboHandles[0]);
    glBufferData(GL_ARRAY_BUFFER, 4 * vertexSize, squareData, GL_STATIC_DRAW);

    glGenVertexArrays(1, &squareVao);
    glBindVertexArray(squareVao);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, (void*) nullptr);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize, (void*) 12);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, vertexSize, (void*) 24);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void onWindowResize(unsigned width, unsigned height) {
    glLoadIdentity();
    glOrtho(0, width, 0, height, -100, 100);
//    glFrustum(0, width, 0, height, 1, 100);
    context->bgRenderer.update(width, height);
}

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
    curRatio = (float) width / height;
    return texture;
}

GLuint loadTexture(char const* filename) {
    int width, height, count;
    unsigned char* data = stbi_load(filename, &width, &height, &count, 0);
    GLuint texture = loadTexture(data, width, height);
    stbi_image_free(data);
    return texture;
}

GLuint loadTexturePNM(char const* filename) {
    PNMImage pnmImage = pnm::readPNMImage(filename);
    GLuint texture = loadTexture((unsigned char*) pnmImage.rgbaData.getRgbaRaster(), pnmImage.pnmHeader.width,
                                 pnmImage.pnmHeader.height);
    return texture;
}

void init(GLFWwindow* window, const string& fileName) {
    glewInit();

    glClearColor(0, 0, 0, 1);
    shader = new ShaderProgram("assets/shaders/default.vert", "assets/shaders/default.frag");

    context = new Context();

    createSquareVao();
    textureId = loadTexturePNM(fileName.c_str());

    utils::initTimer();

    utils::setOnWindowResize(onWindowResize);
    utils::checkWindowSize(window);
}

int main() {
    int width = 640;
    int height = 480;

    GLFWwindow* window = createWindow(width, height, "Hello World", true);
    if (window == nullptr) return -1;

    init(window, "assets/qbic.ppm");

    while (!glfwWindowShouldClose(window)) {
        utils::checkWindowSize(window);
        update();
        render();
        glfwSwapBuffers(window);
        utils::updateTimer();
        glfwPollEvents();
//        glfwWaitEvents();
    }

    glfwTerminate();
    delete context;
    return 0;
}