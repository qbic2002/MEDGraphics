#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include "utils/file.h"
#include "utils/ShaderProgram.h"
#include "utils/measureFps.h"
#include "img/ModernColorModel.h"
#include "img/pngUtils.h"

using namespace std;

GLuint textureId;
ShaderProgram* defaultShader = nullptr;
int textureWidth = 512;
int textureHeight = 512;

GLFWwindow* createWindow(int width, int height, const char* title, bool vsync) {
    GLFWwindow* window;

    if (!glfwInit())
        return nullptr;

//    glfwWindowHint(GLFW_SAMPLES, 16);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
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

void assertShaderCompiled2(GLuint shaderId, const std::string& label) {
    GLint compiled;
    glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compiled);
    if (compiled != GL_TRUE) {
        GLsizei log_length = 0;
        GLchar message[1024];
        glGetShaderInfoLog(shaderId, 1024, &log_length, message);
        std::cerr << "Couldn't compile shader '" << label << "': " << message << std::endl;
        throw std::exception();
    }
}

void assertProgramLinked2(GLuint programId) {
    GLint programLinked;
    glGetProgramiv(programId, GL_LINK_STATUS, &programLinked);
    if (programLinked != GL_TRUE) {
        GLsizei log_length = 0;
        char message[1024];
        glGetProgramInfoLog(programId, 1024, &log_length, message);
        std::cerr << "Couldn't link program: " << message << std::endl;
        throw std::exception();
    }
}

GLuint createComputeShader(const char* shaderCode) {
    unsigned int computeShaderId = glCreateShader(GL_COMPUTE_SHADER);
    glShaderSource(computeShaderId, 1, &shaderCode, nullptr);
    glCompileShader(computeShaderId);
    assertShaderCompiled2(computeShaderId, "compute");

    GLuint computeShaderProgramId = glCreateProgram();
    glAttachShader(computeShaderProgramId, computeShaderId);
    glLinkProgram(computeShaderProgramId);
    assertProgramLinked2(computeShaderProgramId);

    return computeShaderProgramId;
}

GLuint computeProgramId = 0;

GLuint createImage(char const* filename) {
    unsigned int textureId;

    glGenTextures(1, &textureId);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, textureWidth, textureHeight, 0, GL_RGBA, GL_FLOAT, nullptr);

    return textureId;
}

void drawCircle(int x, int y, float radius) {
    glBindImageTexture(0, textureId, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
    glUseProgram(computeProgramId);
    glUniform1f(1, radius);
    glUniform2i(2, x, y);
    glDispatchCompute(textureWidth, textureHeight, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    glUseProgram(0);
}

GLuint quadVbo = 0;

float square[] = {
        -1, 1, 0, 0,
        -1, -1, 0, 1,
        1, 1, 1, 0,
        1, -1, 1, 1,
};

GLuint createQuad() {
    GLuint verticesBufferId;
    glGenBuffers(1, &verticesBufferId);
    glBindBuffer(GL_ARRAY_BUFFER, verticesBufferId);
    glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);

    GLuint vertexArrayId;
    glGenVertexArrays(1, &vertexArrayId);
    glBindVertexArray(vertexArrayId);
    glBindBuffer(GL_ARRAY_BUFFER, verticesBufferId);
    glVertexAttribPointer(0, 2, GL_FLOAT, false, 16, NULL);
    glVertexAttribPointer(1, 2, GL_FLOAT, false, 16, (const void*) 8);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    return vertexArrayId;
}

void show() {
    glBindTexture(GL_TEXTURE_2D, textureId);

//    defaultShader->useProgram();
//    glBindVertexArray(quadVbo);
//    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
//    glBindVertexArray(0);
//    glUseProgram(0);

    glEnable(GL_TEXTURE_2D);
    glBegin(GL_TRIANGLE_STRIP);
    for (int i = 0; i < 4; i++) {
        glTexCoord2f(square[i * 4 + 2], square[i * 4 + 3]);
        glVertex2f(square[i * 4], square[i * 4 + 1]);
    }
    glEnd();
    glDisable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
}

int cursorX = 0, cursorY = 0;

void cursorPosCallback(GLFWwindow* window, double xPos, double yPos) {
    cursorX = (int) xPos;
    cursorY = (int) yPos;
}

void saveImage();

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key != GLFW_KEY_SPACE || action != GLFW_RELEASE)
        return;
    saveImage();
}

int main() {
    GLFWwindow* window = createWindow(textureWidth, textureHeight, "Compute Shader Test", true);
    if (window == nullptr) return -1;
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetKeyCallback(window, keyCallback);

    glewInit();

    auto gradientCompCode = utils::readAllText(
            R"(C:\Workspace\University\Semester-5\Graphics\cpp-experiments\assets\shaders\compute\gradient.comp)");
    computeProgramId = createComputeShader(gradientCompCode.c_str());
    defaultShader = new ShaderProgram("default4.3");

    quadVbo = createQuad();
    textureId = createImage("assets/test.png");

    utils::initTimer();

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        drawCircle(cursorX, cursorY, 100);
        show();
        utils::updateTimer();
        glfwSwapBuffers(window);
//        glfwWaitEvents();
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void saveImage() {
    auto* pixels = new float[textureWidth * textureHeight * 4];

    glBindTexture(GL_TEXTURE_2D, textureId);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, pixels);
    glBindTexture(GL_TEXTURE_2D, 0);

    auto* raster = new ModernRaster(textureWidth, textureHeight, std::shared_ptr<float[]>(pixels), COLOR_MODEL_RGBA);
    png::writePNGImage(*raster, R"(C:\Users\danil\Desktop\export.png)");
    delete raster;
    std::cout << "Could be saved, idk" << std::endl;
}
