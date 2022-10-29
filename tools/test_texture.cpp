#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION

#include "../src/stb_image.h"

using namespace std;

GLuint textureId;

GLFWwindow* createWindow(int width, int height, const char* title, bool vsync) {
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

float square[] = {
        -1, -1, 0, 1,
        1, -1, 1, 1,
        1, 1, 1, 0,
        -1, 1, 0, 0
};

GLuint loadTexture(char const *filename) {
    int width, height, count;
    GLuint texture;
    unsigned char *data = stbi_load(filename, &width, &height, &count, 0);
//    int width = 2;
//    int height = 2;
//    struct {unsigned char r,g,b,a;} data[2][2];
//    memset(data, 0, sizeof(data));
//    data[0][0].r = 255;
//    data[1][0].g = 255;
//    data[1][1].b = 255;
//    data[0][1].r = 255;
//    data[0][1].g = 255;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
    return texture;
}

void show() {
    glColor3f(1, 1, 1);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glPushMatrix();
    {
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);

        glVertexPointer(2, GL_FLOAT, 4 * sizeof(float), square);
        glTexCoordPointer(2, GL_FLOAT, 4 * sizeof(float), square + 2);
        glDrawArrays(GL_QUADS, 0, 4);

        glDisableClientState(GL_TEXTURE_COORD_ARRAY);
        glDisableClientState(GL_VERTEX_ARRAY);
    }
    glPopMatrix();
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}

int main() {
    int width = 500;
    int height = 500;

    GLFWwindow *window = createWindow(width, height, "Texture test", true);
    if (window == nullptr) return -1;

    glewInit();
    textureId = loadTexture("assets/test.png");

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        show();
        glfwSwapBuffers(window);
        glfwWaitEvents();
    }

    glfwTerminate();
    return 0;
}