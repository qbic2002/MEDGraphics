#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <vector>
#include "math/vec3.h"
#include "utils/measureFps.h"
#include "utils/random.h"
#include "utils/windowSize.h"
#include "pnm/PNMImage.h"

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include "pnm/utils/pnmUtil.h"
#include "view/ShaderProgram.h"

using namespace std;
using namespace utils;

GLuint squareVao;
GLuint textureId;
ShaderProgram* shader;
float curRatio;

float squareData[] = {
        -1, -1, 1, 1, 1, 0, 0, 1,
        1, -1, 1, 1, 1, 0, 1, 1,
        1, 1, 1, 1, 1, 0, 1, 0,
        -1, 1, 1, 1, 1, 0, 0, 0
};

class RectVertex {
public:
    vec3 pos;
    vec3 velocity;
    struct rotation {
        vec3 axis;
        float angle;
        float velocity;
    } rotation;
    unsigned char index = 0;
};

void renderRects();

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

ShaderProgram* rectsShader;

void update() {
}

GLuint rectsBufferId;
GLuint rectsVaoId;
int rectsCount = 0;

float angle(float sinValue, float cosValue) {
    float a = acos(cosValue);
    if (sinValue < 0) {
        return -a;
    }

    return a;
}

vec3 randomVec3(float minR, float maxR) {
    float r = random(minR, maxR);
    float a = random() * M_PI * 2;
    return vec3(cos(a), sin(a), 0) * r;
}

void fillRects() {
    if (rectsVaoId != 0) glDeleteVertexArrays(1, &rectsVaoId);
    if (rectsBufferId != 0) glDeleteBuffers(1, &rectsBufferId);

    int width = getWindowWidth();
    int height = getWindowHeight();
    rectsCount = width * height / 200;

    auto* vertices = new RectVertex[rectsCount * 4];

    for (int i = 0; i < rectsCount; i++) {
        RectVertex vert;
        vert.pos = vec3(random(width), random(height), pow(randomFloat(), 2) * 90);
        vert.velocity = randomVec3(10, 20);

        float u = randomFloat();
        float v = randomFloat();
        float w = randomFloat();

        float sinHalfA = sqrt(1 - u + u * pow(sin(2 * M_PI * w), 2));
        float cosHalfA = sqrt(u) * cos(2 * M_PI * w);
        float angleValue = angle(sinHalfA, cosHalfA) * 2;

        float x = sqrt(1 - u) * sin(2 * M_PI * v) / sinHalfA;
        float y = sqrt(1 - u) * cos(2 * M_PI * v) / sinHalfA;
        float z = sqrt(u) * sin(2 * M_PI * w) / sinHalfA;

        vert.rotation = {vec3(x, y, z), angleValue, M_PI * 2};
        for (int index = 0; index < 4; index++) {
            vertices[i * 4 + index] = vert;
            vertices[i * 4 + index].index = index;
        }
    }

    glGenBuffers(1, &rectsBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, rectsBufferId);

    glBufferData(GL_ARRAY_BUFFER, rectsCount * sizeof(RectVertex) * 4, vertices, GL_STATIC_DRAW);

    glGenVertexArrays(1, &rectsVaoId);
    glBindVertexArray(rectsVaoId);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);
    glEnableVertexAttribArray(4);
    glEnableVertexAttribArray(5);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(RectVertex), (void*) nullptr);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(RectVertex), (void*) 12);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(RectVertex), (void*) 24);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(RectVertex), (void*) 36);
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(RectVertex), (void*) 40);
    glVertexAttribIPointer(5, 1, GL_UNSIGNED_BYTE, sizeof(RectVertex), (void*) 44);

    glBindVertexArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    delete[] vertices;
}

void renderRects() {
    glEnable(GL_DEPTH_TEST);
    glUseProgram(rectsShader->getProgramId());
    int thetaPos = glGetUniformLocation(rectsShader->getProgramId(), "theta");
    int widthPos = glGetUniformLocation(rectsShader->getProgramId(), "width");
    int heightPos = glGetUniformLocation(rectsShader->getProgramId(), "height");
    glUniform1f(thetaPos, getCurrentTime());
    glUniform1i(widthPos, getWindowWidth());
    glUniform1i(heightPos, getWindowHeight());
    glBindVertexArray(rectsVaoId);
    glDrawArrays(GL_QUADS, 0, rectsCount * 4);
    glBindVertexArray(0);
    glUseProgram(0);
    glDisable(GL_DEPTH_TEST);
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderRects();

    glUseProgram(shader->getProgramId());
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureId);
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
    fillRects();
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
    GLuint texture = loadTexture((unsigned char*) pnmImage.rgbaData.getPRgbaRaster(), pnmImage.pnmHeader.width,
                                 pnmImage.pnmHeader.height);
    return texture;
}

void init(GLFWwindow* window, const string& fileName) {
    glewInit();

    glClearColor(0, 0, 0, 1);
    shader = new ShaderProgram("assets/shaders/default.vert", "assets/shaders/default.frag");
    rectsShader = new ShaderProgram("assets/shaders/rects.vert", "assets/shaders/rects.frag");

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
    return 0;
}