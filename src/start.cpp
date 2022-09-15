#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <vector>
#include <random>
#include "utils/measureFps.h"
#include "utils/windowSize.h"
#include "math/vec3.h"

using namespace std;
using namespace utils;

class Rect;

GLuint squareVao;
vector<Rect> rects;

class Rect {
public:
    vec3 pos;
    vec3 velocity;
    vec3 rotation;
    vec3 rotationVel;
};

unsigned int random() {
    static random_device rd;
    static mt19937 gen(rd());
    return gen();
}

unsigned int random(unsigned int max) {
    return random() % max;
}

int random(int min, int max) {
    int range = max - min;
    if (range <= 0)
        return min;
    return min + (int) (random() % range);
}

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
    if (!vsync)
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
    glEnable(GL_DEPTH_TEST);
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
    glDisable(GL_DEPTH_TEST);
}

void checkGlError() {
    auto error = glGetError();
    if (error == GL_NO_ERROR)
        return;
    cout << glewGetErrorString(error) << endl;
}

void createSquareVao() {
    float square[]{-1, -1, -1, 1, 1, 1, 1, -1};
    GLuint vboHandles[1];
    glGenBuffers(1, vboHandles);

    glBindBuffer(GL_ARRAY_BUFFER, vboHandles[0]);
    glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(float), square, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenVertexArrays(1, &squareVao);
    glBindVertexArray(squareVao);

    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, vboHandles[0]);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
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

void init(GLFWwindow *window) {
    glewInit();
    createSquareVao();

    glClearColor(0, 0, 0, 1);
    utils::initTimer();

    utils::setOnWindowResize(onWindowResize);
    utils::checkWindowSize(window);
}

int main() {
    int width = 640;
    int height = 480;

    GLFWwindow *window = createWindow(width, height, "Hello World", true);
    if (window == nullptr) return -1;

    init(window);

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