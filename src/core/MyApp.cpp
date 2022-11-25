//
// Created by danil on 11.11.2022.
//

#include "MyApp.h"
#include "../widget/RootView.h"
#include "utils/explorer_utils.h"
#include "img/pnmUtils.h"
#include "../widget/IconView.h"
#include "../widget/ImageView.h"
#include <view/Dialog.h>

view::View* viewerRootView = nullptr;

view::Dialog* messageDialog = nullptr;
view::TextView* messageDialogTxt = nullptr;

view::ImageView* imageView = nullptr;

bool isEditing = false;

ModernRaster* editedRaster;
gl::Texture* editedTexture;

view::TextView* modeTxt;
view::TextView* componentToggles[3];

void createMessageDialog(Context* context) {
    using namespace view;
    auto* dialogLay = new LinearLayout(context, {
            .width = FILL_PARENT / 2,
            .height = FILL_PARENT / 2,
            .background = ColorBackground{rgba{COLOR_PRIMARY_LIGHT}}
    });

    auto* titleLay = new LinearLayout(context, {
            .width = FILL_PARENT,
            .height = WRAP_CONTENT,
            .orientation = HORIZONTAL,
            .gravity = RIGHT});
    dialogLay->addChild(titleLay);

    auto* dialogTitleTxt = new TextView(context, {
            .width = FILL_SPARE,
            .height = WRAP_CONTENT,
            .padding = Padding(12),
            .text = L"Error",
            .fontSize = 18});
    titleLay->addChild(dialogTitleTxt);

    auto* closeBtn = new IconView(context, {
            .width = 32,
            .height = 32,
            .background = StateBackground{
                    Background{},
                    ColorBackground{rgba{COLOR_PRIMARY}},
                    ColorBackground{rgba{COLOR_PRIMARY_DARK}}},
            .imageFile = context->getAppDir() / "assets/icons/ic_close.png",
            .imageWidth = 16,
            .imageHeight = 16
    });
    closeBtn->setOnClickListener([&]() {
        messageDialog->hide();
    });
    titleLay->addChild(closeBtn);

    messageDialogTxt = new TextView(context, {
            .width = FILL_SPARE,
            .height = FILL_SPARE,
            .padding = Padding(12),
            .text = L"{message text}",
            .fontSize = 14
    });
    dialogLay->addChild(messageDialogTxt);

    messageDialog = context->createDialog(dialogLay, FILL_PARENT * 0.25, FILL_PARENT * 0.25);
}

void MyApp::onCreated(const std::vector<std::wstring>& args) {
    glClearColor(0, 0, 0, 1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    using namespace view;

    createMessageDialog(this);

    viewerRootView = new RootView(this, {.width = FILL_PARENT, .height = FILL_PARENT});
    setRootView(viewerRootView);

    imageView = (ImageView*) findViewById(IMAGE_VIEW_ID);
    modeTxt = (view::TextView*) findViewById(MODE_TEXT_VIEW_ID);
    componentToggles[0] = (view::TextView*) findViewById(COMPONENT_1_TEXT_VIEW_ID);
    componentToggles[1] = (view::TextView*) findViewById(COMPONENT_2_TEXT_VIEW_ID);
    componentToggles[2] = (view::TextView*) findViewById(COMPONENT_3_TEXT_VIEW_ID);

    imageFileStorage.open(args[1]);
    imageFileStorage.addImageChangedListener([&]() {
        auto* imageFile = imageFileStorage.getCurImageFile();
        imageView->setTexture(imageFile->textureId, imageFile->raster->getWidth(), imageFile->raster->getHeight());
        imageView->invalidate();
    });
    setColorModel(colorModelEnum);
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


    utils::OpenFileInfo openFileInfo = utils::getSaveFileName();
    if (openFileInfo.isCancelled)
        return;

    std::wstring filename = utils::fixFileName(openFileInfo.filename, openFileInfo.filterIndex);

    try {
        if (isEditing) {
            pnm::writePNMImage(PNMImage(*editedRaster), filename);
        } else {
            pnm::writePNMImage(PNMImage(*imageFileStorage.getCurImageFile()->raster), filename);
        }
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
    if (isEditing)
        return;
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

void updateEditingImageView() {
    editedTexture->update(*editedRaster);
    imageView->setTexture(editedTexture->getTextureId(), editedTexture->getWidth(), editedTexture->getHeight());
}

void setModeTexts(const std::wstring& listText, const std::wstring& comp0, const std::wstring& comp1,
                  const std::wstring& comp2) {
    modeTxt->setText(listText);
    componentToggles[0]->setText(comp0);
    componentToggles[1]->setText(comp1);
    componentToggles[2]->setText(comp2);
}

void MyApp::setColorModel(ColorModelEnum colorModelEnum) {
    this->colorModelEnum = colorModelEnum;
    info() << "color model changed: " << colorModelEnum;

    switch (colorModelEnum) {
        case COLOR_MODEL_RGB:
            setModeTexts(L"RGB", L"R", L"G", L"B");
            break;
        case COLOR_MODEL_HSL:
            setModeTexts(L"HSL", L"H", L"S", L"L");
            break;
        case COLOR_MODEL_HSV:
            setModeTexts(L"HSV", L"H", L"S", L"V");
            break;
        case COLOR_MODEL_YCbCr601:
            setModeTexts(L"YCbCr601", L"Y", L"Cb", L"Cr");
            break;
        case COLOR_MODEL_YCbCr709:
            setModeTexts(L"YCbCr709", L"Y", L"Cb", L"Cr");
            break;
        case COLOR_MODEL_YCoCg:
            setModeTexts(L"YCoCg", L"Y", L"Co", L"Cg");
            break;
        case COLOR_MODEL_CMY:
            setModeTexts(L"CMY", L"C", L"M", L"Y");
            break;
    }

    if (isEditing) {
        editedRaster->convertToColorModel(colorModelEnum);
        updateEditingImageView();
    }
}

void setToggleViewActive(int index, bool value) {
    componentToggles[index]->getParent()->setBackground(value
                                                        ? view::theme.activeComponentBackground
                                                        : view::theme.notActiveComponentBackground);
}

void MyApp::toggleEdit() {
    if (!isEditing) {
        auto imageFile = imageFileStorage.getCurImageFile();
        if (imageFile == nullptr) {
            showError(L"Open image to enter editing mode");
            return;
        }
        if (imageFile->raster == nullptr) {
            showError(L"Wait image loading");
            return;
        }
        if (imageFile->raster->getColorModel()->getComponentsCount() != 3) {
            showError(L"Only 3-component images can be edited");
            return;
        }

        editedRaster = new ModernRaster(*imageFile->raster);
        editedRaster->reinterpretColorModel(colorModelEnum);

        editedTexture = new gl::Texture(*editedRaster);

        imageView->setTexture(editedTexture->getTextureId(), editedTexture->getWidth(), editedTexture->getHeight());

        for (int i = 0; i < 3; i++)
            setToggleViewActive(i, editedRaster->getFilter(i));

        isEditing = true;
        viewerRootView->setBackground(view::ColorBackground(rgba{60, 63, 65, 255}));
    } else {
        delete editedRaster;
        editedRaster = nullptr;
        delete editedTexture;
        editedTexture = nullptr;

        auto imageFile = imageFileStorage.getCurImageFile();
        imageView->setTexture(imageFile->textureId, imageFile->raster->getWidth(), imageFile->raster->getHeight());

        for (auto& componentToggle: componentToggles)
            componentToggle->getParent()->setBackground({});

        isEditing = false;
        viewerRootView->setBackground(view::ColorBackground(rgba{0, 0, 0, 255}));
    }
    info() << "isEditing: " << isEditing;
}

void MyApp::toggleComponent(int index) {
    if (!isEditing)
        return;
    bool newValue = !editedRaster->getFilter(index);
    editedRaster->setFilter(index, newValue);
    updateEditingImageView();
    setToggleViewActive(index, newValue);
}
