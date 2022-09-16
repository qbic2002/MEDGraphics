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

class Rect;

GLuint squareVao;
vector<Rect> rects;
GLuint textureId;
ShaderProgram* shader;
float curRatio;

float squareData[] = {
        -1, -1, 1, 1, 1, 0, 0, 1,
        1, -1, 1, 1, 1, 0, 1, 1,
        1, 1, 1, 1, 1, 0, 1, 0,
        -1, 1, 1, 1, 1, 0, 0, 0
};


class Rect {
public:
    vec3 pos;
    vec3 velocity;
    vec3 rotation;
    vec3 rotationVel;
};

GLFWwindow *createWindow(int width, int height, const char *title, bool vsync) {
    GLFWwindow *window;

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

void clamp(float &value, float min, float max) {
    if (value > max)
        value = max;
    if (value < min)
        value = min;
}

void clamp(vec3 &value, vec3 const &min, vec3 const &max) {
    clamp(value.x, min.x, max.x);
    clamp(value.y, min.y, max.y);
    clamp(value.z, min.z, max.z);
}

void cycleValue(float &value, float min, float max) {
    if (value >= max)
        value = min;
    if (value < min)
        value = max - 1;
}

void update() {
    float dt = getDeltaT();
    for (auto &rect: rects) {
        rect.pos += rect.velocity * (dt / 2);
        cycleValue(rect.pos.x, -10, (float) getWindowWidth() + 10);
        cycleValue(rect.pos.y, -10, (float) getWindowHeight() + 10);
        //        clamp(rect.pos.z, -10, -1);
        clamp(rect.pos.z, 1, 10);
        rect.velocity.x += random(-100, 101) * dt;
        rect.velocity.y += random(-100, 101) * dt;
        rect.velocity.z += random(-100, 101) * dt / 10;
        clamp(rect.velocity.x, -50, 50);
        clamp(rect.velocity.y, -50, 50);
        clamp(rect.velocity.z, -0.2, 0.2);

        rect.rotation += rect.rotationVel * dt;
        rect.rotationVel += vec3(random(-90, 91), random(-90, 91), random(-90, 91)) * dt;
        clamp(rect.rotationVel, vec3(-180), vec3(180));
    }
}

void render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBindVertexArray(squareVao);
    for (auto &rect: rects) {
        glPushMatrix();
        glTranslatef(rect.pos.x, rect.pos.y, rect.pos.z);
        glRotatef(rect.rotation.x, 1, 0, 0);
        glRotatef(rect.rotation.y, 0, 1, 0);
        glRotatef(rect.rotation.z, 0, 0, 1);
        float k = (rect.pos.z) / 10;
        float color = 0.8 * k + 0.2 * (1 - k);
        glColor3f(color, color, color);
        float size = 5 * k + 3 * (1 - k);
        glScalef(size, size, 1);
        glDrawArrays(GL_QUADS, 0, 4);
        glPopMatrix();
    }
    glBindVertexArray(0);

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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertexSize, (void *) nullptr);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertexSize, (void *) 12);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, vertexSize, (void *) 24);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
}

void fillRects() {
    rects.clear();
    int width = getWindowWidth();
    int height = getWindowHeight();
    rects.resize(width * height / 6000);
    for (Rect &rect: rects) {
        rect.pos = vec3(random(width), random(height), random(1, 10));
        rect.velocity = vec3(random(-50, 50), random(-50, 50), 0);
        rect.rotation = vec3(random(360), random(360), random(360));
        rect.rotationVel = vec3(random(-180, 180), random(-180, 180), random(-180, 180));
    }
}

void onWindowResize(unsigned width, unsigned height) {
    glLoadIdentity();
    glOrtho(0, width, 0, height, -100, 100);
//    glFrustum(0, width, 0, height, 1, 100);
    fillRects();
}

GLuint loadTexture(unsigned char const *data, int width, int height) {
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

GLuint loadTexture(char const *filename) {
    int width, height, count;
    unsigned char *data = stbi_load(filename, &width, &height, &count, 0);
    GLuint texture = loadTexture(data, width, height);
    stbi_image_free(data);
    return texture;
}

GLuint loadTexturePNM(char const *filename) {
    PNMImage pnmImage = pnm::readPNMImage(filename);
    GLuint texture = loadTexture((unsigned char *) pnmImage.rgbaData.getPRgbaRaster(), pnmImage.pnmHeader.width,
                                 pnmImage.pnmHeader.height);
    return texture;
}

void init(GLFWwindow *window, const string &fileName) {
    glewInit();

    glClearColor(0, 0, 0, 1);
    shader = new ShaderProgram("assets/shaders/default.vert", "assets/shaders/default.frag");

    createSquareVao();
    textureId = loadTexturePNM(fileName.c_str());

    utils::initTimer();

    utils::setOnWindowResize(onWindowResize);
    utils::checkWindowSize(window);
}

int main() {
    int width = 640;
    int height = 480;

    GLFWwindow *window = createWindow(width, height, "Hello World", true);
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