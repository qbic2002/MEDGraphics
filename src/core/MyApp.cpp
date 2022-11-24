//
// Created by danil on 11.11.2022.
//

#include "MyApp.h"
#include "../widget/RootView.h"
#include "utils/explorer_utils.h"
#include "img/PNMImage.h"
#include "img/pnmUtils.h"
#include "../widget/IconView.h"
#include "../widget/TextView.h"
#include <view/Dialog.h>

view::Dialog* messageDialog = nullptr;
view::TextView* messageDialogTxt = nullptr;

void MyApp::onCreated(const std::vector<std::wstring>& args) {
    glClearColor(0, 0, 0, 1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    using namespace view;

    setRootView(new RootView(this, {.width = FILL_PARENT, .height = FILL_PARENT}));

    auto* messageLay = new LinearLayout(this, {
            .width = FILL_PARENT / 2,
            .height = FILL_PARENT / 2,
            .background = ColorBackground{rgba{COLOR_PRIMARY_LIGHT}}
    });

    messageLay->addChild(new LinearLayout(this, {
            .width = FILL_PARENT,
            .height = WRAP_CONTENT,
            .children = {
                    new TextView(this, {
                            .width = FILL_SPARE,
                            .height = WRAP_CONTENT,
                            .padding = Padding(12),
                            .text = L"Error",
                            .fontSize = 18}),
                    &((new IconView(this, {
                            .width = 32,
                            .height = 32,
                            .background = StateBackground{
                                    Background{},
                                    ColorBackground{rgba{COLOR_PRIMARY}},
                                    ColorBackground{rgba{COLOR_PRIMARY_DARK}}},
                            .imageFile = getAppDir() / "assets/icons/ic_close.png",
                            .imageWidth = 16,
                            .imageHeight = 16
                    }))->setOnClickListener([&]() {
                        messageDialog->hide();
                    }))},
            .orientation = HORIZONTAL,
            .gravity = RIGHT}));

    messageDialogTxt = new TextView(this, {
            .width = FILL_SPARE,
            .height = FILL_SPARE,
            .padding = Padding(12),
            .text = L"{message text}",
            .fontSize = 14
    });
    messageLay->addChild(messageDialogTxt);

    messageDialog = createDialog(messageLay, FILL_PARENT * 0.25, FILL_PARENT * 0.25);

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
    messageDialogTxt->setText(message);
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
