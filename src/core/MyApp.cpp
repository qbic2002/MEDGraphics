//
// Created by danil on 11.11.2022.
//

#include "MyApp.h"
#include "../widget/RootView.h"
#include "utils/explorer_utils.h"
#include "img/PNMImage.h"
#include "img/pnmUtils.h"
#include "../widget/MessageView.h"
#include <view/Dialog.h>

view::Dialog* messageDialog = nullptr;

void MyApp::onCreated(const std::vector<std::wstring>& args) {
    glClearColor(0, 0, 0, 1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    using namespace view;

    setRootView(new RootView(this, Style{{.width = FILL_PARENT, .height = FILL_PARENT}}));

    auto* messageView = new MessageView(
            this,
            StyleState{
                    .width = FILL_PARENT * 0.5,
                    .height = FILL_PARENT * 0.5,
                    .padding = padding(12),
                    .background = {
                            .color = rgba{COLOR_PRIMARY_LIGHT}
                    }
            }
    );
    messageView->setId(MESSAGE_VIEW_ID);
    messageDialog = createDialog(messageView, FILL_PARENT * 0.25, FILL_PARENT * 0.25);

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
    auto* msgView = (view::MessageView*) findViewById(MESSAGE_VIEW_ID);
    msgView->setMessage(message);
    messageDialog->show();
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
