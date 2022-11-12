//
// Created by danil on 11.11.2022.
//

#include "MyApp.h"
#include "../widget/RootView.h"
#include "../utils/explorerUtils.h"
#include "../pnm/PNMImage.h"
#include "../pnm/pnmUtils.h"
#include "../widget/MessageView.h"

void MyApp::onCreated(const std::vector<std::wstring>& args) {
    info() << "onCreated";
    glClearColor(0, 0, 0, 1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    using namespace view;
    setRootView(new RootView(this, Style{{.position = {0, 0, FILL_PARENT, FILL_PARENT}}}));

    imageFileStorage.open(args[1]);
}

void MyApp::update() {
    imageFileStorage.update();
}

ImageFileStorage& MyApp::getImageFileStorage() {
    return imageFileStorage;
}

void MyApp::saveImage() {
    if (imageFileStorage.getCurImageFile() == nullptr) {
        showError(L"File is not opened");
        return;
    }
    if (imageFileStorage.getCurImageFile()->raster == nullptr) {
        showError(L"Image has not been load yet");
        return;
    }
    std::wstring filename;
    if (!utils::getSaveFileName(filename))
        return;
    try {
        pnm::writePNMImage(PNMImage(imageFileStorage.getCurImageFile()->raster), filename);
    } catch (std::exception&) {
        showError(L"Error with saving =(");
    }
}

void MyApp::openImage() {
    std::wstring filename;
    if (!utils::getOpenFileName(filename))
        return;
    imageFileStorage.open(filename);
}

void MyApp::showError(const String& message) {
    auto* msgView = (view::MessageView*) rootView->findViewById(MESSAGE_VIEW_ID);
    msgView->showMessage(message);
}

void MyApp::onMouseMove(double x, double y) {
    double dx = x - prevX;
    double dy = y - prevY;
    prevX = x;
    prevY = y;
    rootView->onMouseMove(x, y);
    if (grabbing) {
        if (rootView->onDrag(x, y, dx, dy)) {
            grabX = prevX;
            grabY = prevY;
            return;
        }
        if (!windowWrapper->isMaximized()) {
            windowWrapper->getWindowPos(&grabWindowX, &grabWindowY);
            windowWrapper->setWindowPos(grabWindowX + x - grabX, grabWindowY + y - grabY);
        }
    }
}

void MyApp::onMouseButton(ClickEvent& event) {
    if (event.action == GLFW_RELEASE && grabbing)
        grabbing = false;

    if (rootView->onClick(event))
        return;

    if (event.action != GLFW_PRESS)
        return;

    grabbing = true;
    grabX = prevX;
    grabY = prevY;
    windowWrapper->getWindowPos(&grabWindowX, &grabWindowY);
}

void MyApp::onMouseLeave() {
    rootView->onMouseLeave();
    grabbing = false;
}

void MyApp::onScroll(double xOffset, double yOffset, double cursorX, double cursorY) {
    rootView->onScroll(xOffset, yOffset, cursorX, cursorY);
}

void MyApp::onKey(int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods) {
    if (action == GLFW_RELEASE || action == GLFW_REPEAT) {
        switch (key) {
            case GLFW_KEY_LEFT:
                imageFileStorage.prev();
                break;
            case GLFW_KEY_RIGHT:
                imageFileStorage.next();
                break;
            default:
                break;
        }
    }
}
