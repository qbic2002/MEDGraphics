//
// Created by danil on 11.11.2022.
//

#ifndef MEDGRAPHICS_WINDOW_WRAPPER_H
#define MEDGRAPHICS_WINDOW_WRAPPER_H

class WindowWrapper {
public:
    virtual void swapBuffers() = 0;

    virtual bool shouldClose() = 0;

    virtual void pollEvents() = 0;

    virtual void waitEvents() = 0;

    virtual void waitEvents(double timeout) = 0;

    virtual void setShouldClose(int value) = 0;

    virtual void toggleMaximized() = 0;

    virtual bool isMaximized() = 0;

    virtual void iconify() = 0;

    virtual void getWindowPos(int* x, int* y) = 0;

    virtual void setWindowPos(int x, int y) = 0;

    virtual void getWindowSize(int* width, int* height) = 0;

    virtual void getPointerPos(double& x, double& y) = 0;

    virtual double getPointerX() = 0;

    virtual double getPointerY() = 0;
};

#endif //MEDGRAPHICS_WINDOW_WRAPPER_H
