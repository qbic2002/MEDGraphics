//
// Created by danil on 17.09.2022.
//

#include "Context.h"
#include "ImageView.h"
#include "BgRenderer.h"
#include "../img/imageLoader.h"
#include "../gl/utils.h"

namespace view {

    View* closeView = nullptr;
    View* maximizeView = nullptr;
    View* iconifyView = nullptr;

    bool isMaximized = false;

    void onMaximize(GLFWwindow* _window, int maximized) {
        isMaximized = maximized;
        if (maximizeView != nullptr) {
            if (maximized)
                maximizeView->getBackground().setImage("assets/icons/ic_maximized.png");
            else
                maximizeView->getBackground().setImage("assets/icons/ic_minimized.png");
        }
    }

    Context::Context(GLFWwindow* _window, const std::string& fileName) {
        window = _window;
        glfwSetWindowMaximizeCallback(window, onMaximize);

        raster = img::loadImageData(fileName);
        textureId = gl::loadTexture(raster, GL_CLAMP, GL_LINEAR, GL_NEAREST);

        views.push_back(bgRenderer = new BgRenderer(this));
        views.push_back(new ImageView(this));

        closeView = new View(this, 0, 0, 0, 0);
        closeView->getBackground()
                .setImage("assets/icons/ic_close.png")
                .setColorOnHover({196, 43, 28, 255})
                .setColorOnPress({198, 99, 99, 255});
        closeView->setOnClickListener([_window]() {
            glfwSetWindowShouldClose(_window, 1);
        });
        views.push_back(closeView);

        maximizeView = new View(this, 0, 0, 0, 0);
        maximizeView->getBackground()
                .setImage("assets/icons/ic_minimized.png")
                .setColorOnHover({127, 127, 127, 127})
                .setColorOnPress({63, 63, 63, 127});
        maximizeView->setOnClickListener([_window]() {
            if (isMaximized)
                glfwRestoreWindow(_window);
            else
                glfwMaximizeWindow(_window);
        });
        views.push_back(maximizeView);

        iconifyView = new View(this, 0, 0, 0, 0);
        iconifyView->getBackground()
                .setImage("assets/icons/ic_iconify.png")
                .setColorOnHover({127, 127, 127, 127})
                .setColorOnPress({63, 63, 63, 127});
        iconifyView->setOnClickListener([_window]() {
            glfwIconifyWindow(_window);
        });
        views.push_back(iconifyView);
    }

    void Context::update() {

    }

    void Context::render() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_TEXTURE_2D);

        glBindTexture(GL_TEXTURE_2D, textureId);

        for (auto* view: views) {
            view->render();
        }

        glBindTexture(GL_TEXTURE_2D, 0);

        glDisable(GL_TEXTURE_2D);
    }

    void Context::onWindowResize(unsigned int width, unsigned int height) {
        glLoadIdentity();
        glOrtho(0, width, height, 0, -100, 100);
        bgRenderer->update(width, height);

        unsigned int iconSize = 16;
        unsigned int btnWidth = 46;
        unsigned int btnHeight = 28;

        int leftEdge = width;
        leftEdge -= btnWidth;
        closeView->setPosition(leftEdge, 0, btnWidth, btnHeight);
        leftEdge -= btnWidth;
        maximizeView->setPosition(leftEdge, 0, btnWidth, btnHeight);
        leftEdge -= btnWidth;
        iconifyView->setPosition(leftEdge, 0, btnWidth, btnHeight);

        closeView->setPadding(padding(((float) (btnWidth - iconSize) / 2), (float) (btnHeight - iconSize) / 2));
        maximizeView->setPadding(padding(((float) (btnWidth - iconSize) / 2), (float) (btnHeight - iconSize) / 2));
        iconifyView->setPadding(padding(((float) (btnWidth - iconSize) / 2), (float) (btnHeight - iconSize) / 2));
    }

    Context::~Context() {
        for (auto* view: views) {
            delete view;
        }
        delete raster;
    }

    bool isInside(View* view, double x, double y) {
        return view->getX() <= x && x <= (view->getX() + view->getWidth())
               && view->getY() <= y && y <= (view->getY() + view->getHeight());
    }

    bool grabbing = false;
    double prevX, prevY;
    double grabX, grabY;
    int grabWindowX, grabWindowY;

    void Context::onMouseMove(double x, double y) {
        prevX = x;
        prevY = y;
        for (View* view: views) {
            bool isInsideFlag = isInside(view, x, y);
            switch (view->state) {
                case DEFAULT:
                    if (isInsideFlag) {
                        view->state = HOVERED;
                    }
                    break;
                case HOVERED:
                case PRESSED:
                    if (!isInsideFlag) {
                        view->state = DEFAULT;
                    }
                    break;
            }
        }
        if (grabbing) {
            glfwGetWindowPos(window, &grabWindowX, &grabWindowY);
            glfwSetWindowPos(window, grabWindowX + x - grabX, grabWindowY + y - grabY);
        }
    }

    void Context::onMouseButton(int button, int action, int mods) {
        if (button != 0)
            return;
        if (action == 0 && grabbing)
            grabbing = false;
        bool pressedSomething = false;
        for (View* view: views) {
            switch (view->state) {
                case DEFAULT:
                    break;
                case HOVERED:
                    if (action == 1) {
                        view->state = PRESSED;
                        pressedSomething = true;
                    }
                    break;
                case PRESSED:
                    if (action == 0) {
                        view->onClick();
                        view->state = HOVERED;
                    }
                    break;
            }
        }
        if (action != 1 || pressedSomething)
            return;

        if (!isMaximized) {
            grabbing = true;
            grabX = prevX;
            grabY = prevY;
            glfwGetWindowPos(window, &grabWindowX, &grabWindowY);
        }
    }

    void Context::onMouseLeave() {
        for (View* view: views) {
            switch (view->state) {
                case DEFAULT:
                    break;
                case HOVERED:
                case PRESSED: {
                    view->state = DEFAULT;
                }
                    break;
            }
        }
        grabbing = false;
    }
} // view