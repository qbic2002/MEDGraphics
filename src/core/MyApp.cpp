//
// Created by danil on 11.11.2022.
//

#include "MyApp.h"
#include "utils/explorer_utils.h"
#include "../widget/ImageView.h"
#include "MyLayout.h"
#include "../widget/EditText.h"
#include "../widget/SelectView.h"
#include "utils/SaveFormat.h"

MyApp* instance = nullptr;

view::View* viewerRootView = nullptr;

view::Dialog* messageDialog = nullptr;
view::TextView* messageDialogTxt = nullptr;

view::ImageView* imageView = nullptr;

bool isEditing = false;

ModernRaster* editedRaster;
gl::Texture* editedTexture;

int xBegin = 0;
int yBegin = 0;

struct {
    view::View* lay = nullptr;
    struct {
        view::TextView* widthTxt = nullptr;
        view::TextView* heightTxt = nullptr;
        view::TextView* channelsTxt = nullptr;
    } info;
    struct {
        view::SelectView* colorModel = nullptr;
        view::TextView* componentToggle[4] = {nullptr, nullptr, nullptr, nullptr};
    } colorModel;
    struct {
        view::EditText* gammaEdt = nullptr;
    } gamma;
    struct {
        view::SelectView* bitsSelect = nullptr;
        view::SelectView* modeSelect = nullptr;
    } dithering;
    struct {
        view::TextView* componentText[4] = {nullptr, nullptr, nullptr, nullptr};
        view::EditText* componentEdt[4] = {nullptr, nullptr, nullptr, nullptr};
        view::EditText* opacityEdt = nullptr;
        view::EditText* lineWidthEdt = nullptr;
    } paint;
} rightTool;

void MyApp::onCreated(const std::vector<std::wstring>& args) {
    instance = this;

    glClearColor(0, 0, 0, 1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    using namespace view;

    messageDialog = createMessageDialog(this);

    viewerRootView = createRootView(this);
    setRootView(viewerRootView);

    imageView = (ImageView*) findViewById(IMAGE_VIEW_ID);
    messageDialogTxt = (TextView*) findViewById(ID_DIALOG_MESSAGE_TXT);

    rightTool.lay = findViewById(ID_RIGHT_TOOL_LAY);
    rightTool.info.widthTxt = (TextView*) findViewById(ID_RIGHT_TOOL_INFO_WIDTH_TXT);
    rightTool.info.heightTxt = (TextView*) findViewById(ID_RIGHT_TOOL_INFO_HEIGHT_TXT);
    rightTool.info.channelsTxt = (TextView*) findViewById(ID_RIGHT_TOOL_INFO_CHANNELS_TXT);
    rightTool.colorModel.colorModel = (SelectView*) findViewById(ID_RIGHT_TOOL_COLOR_MODEL);
    rightTool.colorModel.componentToggle[0] = (TextView*) findViewById(ID_RIGHT_TOOL_COLOR_MODEL_COMP1);
    rightTool.colorModel.componentToggle[1] = (TextView*) findViewById(ID_RIGHT_TOOL_COLOR_MODEL_COMP2);
    rightTool.colorModel.componentToggle[2] = (TextView*) findViewById(ID_RIGHT_TOOL_COLOR_MODEL_COMP3);
    rightTool.colorModel.componentToggle[3] = (TextView*) findViewById(ID_RIGHT_TOOL_COLOR_MODEL_COMP4);
    rightTool.gamma.gammaEdt = (EditText*) findViewById(ID_RIGHT_TOOL_GAMMA_EDT);
    rightTool.dithering.bitsSelect = (SelectView*) findViewById(ID_RIGHT_TOOL_DITHER_BITS);
    rightTool.dithering.modeSelect = (SelectView*) findViewById(ID_RIGHT_TOOL_DITHER_MODE);
    for (int i = 0; i < 4; i++) {
        rightTool.paint.componentText[i] = (TextView*) findViewById(ID_RIGHT_TOOL_PAINT_COMP1_TXT + i);
        rightTool.paint.componentEdt[i] = (EditText*) findViewById(ID_RIGHT_TOOL_PAINT_COMP1_EDT + i);
    }
    rightTool.paint.opacityEdt = (EditText*) findViewById(ID_RIGHT_TOOL_PAINT_COMP_OPACITY_EDT);
    rightTool.paint.lineWidthEdt = (EditText*) findViewById(ID_RIGHT_TOOL_PAINT_LINE_WIDTH);

    imageFileStorage.open(args[1]);
    imageFileStorage.addImageChangedListener([&]() {
        auto* imageFile = imageFileStorage.getCurImageFile();
        imageView->setTexture(imageFile->textureId, imageFile->raster->getWidth(), imageFile->raster->getHeight());
        imageView->invalidate();
    });
}

void updateEditingImageView() {
    editedTexture->update(*editedRaster);
    imageView->setTexture(editedTexture->getTextureId(), editedTexture->getWidth(), editedTexture->getHeight());
}

void drawLine(int x1, int y1, int x2, int y2) {
    if (!isEditing)
        return;
    if (x1 == x2 && y1 == y2)
        return;
    float color[4] = {0, 0, 0, 0};
    for (int i = 0; i < editedRaster->getColorModel()->getComponentsCount(); i++) {
        float value = 1;
        try {
            value = std::stof(rightTool.paint.componentEdt[i]->getText());
        } catch (std::exception& e) {
        }
        if (value > 1)
            value = 1;
        if (value < 0)
            value = 0;
        color[i] = value;
    }
    float opacity = 1;
    try {
        opacity = std::stof(rightTool.paint.opacityEdt->getText());
    } catch (std::exception& e) {}
    if (opacity > 1)
        opacity = 1;
    if (opacity < 0)
        opacity = 0;

    float lineWidth = 1;
    try {
        lineWidth = std::stof(rightTool.paint.lineWidthEdt->getText());
    } catch (std::exception& e) {}
    if (lineWidth < 0)
        lineWidth = 0;

    info() << x1 << " " << y1 << " " << x2 << " " << y2;
    editedRaster->drawLine({x1, y1}, {x2, y2}, color, lineWidth, opacity);
    updateEditingImageView();
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
    try {
        const auto& saveFormat = utils::getSaveFormat(openFileInfo.filterIndex - 1);
        ModernRaster* rasterToSave = isEditing ? editedRaster : imageFileStorage.getCurImageFile()->raster;
        saveFormat.saveImage(rasterToSave, openFileInfo.filename);
    } catch (std::exception&) {
        showError(L"Error with saving =(");
    }
}

void MyApp::openImage() {
    std::wstring filename;
    if (!utils::getOpenFileName(filename))
        return;
    if (isEditing)
        toggleEdit();
    imageFileStorage.open(filename);
}

void MyApp::showError(const String& message) {
    messageDialogTxt->setText(message);
    messageDialog->show();
}

bool MyApp::onKey(int key, [[maybe_unused]] int scancode, int action, [[maybe_unused]] int mods) {
    if (RootViewManager::onKey(key, scancode, action, mods))
        return true;

    if (isEditing)
        return false;
    if (action == GLFW_RELEASE || action == GLFW_REPEAT) {
        switch (key) {
            case GLFW_KEY_LEFT:
                imageFileStorage.prev();
                return true;
            case GLFW_KEY_RIGHT:
                imageFileStorage.next();
                return true;
            default:
                break;
        }
    }

    return false;
}

void setToggleViewActive(int index, bool value) {
    rightTool.colorModel.componentToggle[index]->getParent()->setBackground(value
                                                                            ? view::theme.activeComponentBackground
                                                                            : view::theme.notActiveComponentBackground);
}

void updateInfoUI() {
    rightTool.info.widthTxt->setText(std::to_wstring(editedRaster->getWidth()));
    rightTool.info.heightTxt->setText(std::to_wstring(editedRaster->getHeight()));
    rightTool.info.channelsTxt->setText(std::to_wstring(editedRaster->getColorModel()->getComponentsCount()));
}

void updateColorModelUI() {
    if (!editedRaster)
        return;
    auto colorModel = editedRaster->getColorModel();
    int index;
    switch (colorModel->getEnum()) {
        case COLOR_MODEL_RGB:
            index = 0;
            break;
        case COLOR_MODEL_HSL:
            index = 1;
            break;
        case COLOR_MODEL_HSV:
            index = 2;
            break;
        case COLOR_MODEL_YCbCr601:
            index = 3;
            break;
        case COLOR_MODEL_YCbCr709:
            index = 4;
            break;
        case COLOR_MODEL_YCoCg:
            index = 5;
            break;
        case COLOR_MODEL_CMY:
            index = 6;
            break;
        case COLOR_MODEL_GRAY:
            index = 7;
            break;
        case COLOR_MODEL_RGBA:
            index = 8;
            break;
        default:
            return;
    }

    static const std::vector<std::vector<String>> componentNames = {
            {L"R", L"G",  L"B"},
            {L"H", L"S",  L"L"},
            {L"H", L"S",  L"V"},
            {L"Y", L"Cb", L"Cr"},
            {L"Y", L"Cb", L"Cr"},
            {L"Y", L"Co", L"Cg"},
            {L"C", L"M",  L"Y"},
            {L"Gray"},
            {L"R", L"G",  L"B", L"A"},
    };

    rightTool.colorModel.colorModel->setSelectIndex(index);
    for (int i = 0; i < componentNames[index].size(); i++) {
        rightTool.colorModel.componentToggle[i]->setText(componentNames[index][i]);
    }

    int componentsCount = colorModel->getComponentsCount();
    for (int i = 0; i < componentsCount; i++) {
        rightTool.colorModel.componentToggle[i]->getParent()->setVisibility(view::VISIBLE);
        setToggleViewActive(i, editedRaster->getFilter(i));
    }
    for (int i = componentsCount; i < 4; i++) {
        rightTool.colorModel.componentToggle[i]->getParent()->setVisibility(view::INVISIBLE);
    }

    for (int i = 0; i < componentsCount; i++) {
        rightTool.paint.componentText[i]->setVisibility(view::VISIBLE);
        rightTool.paint.componentEdt[i]->setVisibility(view::VISIBLE);
        rightTool.paint.componentText[i]->setText(componentNames[index][i]);
    }
    for (int i = componentsCount; i < 4; i++) {
        rightTool.paint.componentText[i]->setText(L"");
//        rightTool.paint.componentEdt[i]->setVisibility(view::INVISIBLE);
    }
}

void MyApp::convertColorModel(ColorModelEnum colorModel) {
    if (!isEditing) {
        return;
    }

    editedRaster->convertToColorModel(colorModel);
    updateEditingImageView();

    updateColorModelUI();
}

void MyApp::reinterpretColorModel(ColorModelEnum colorModel) {
    if (!isEditing) {
        return;
    }

    if (editedRaster->getColorModel()->getComponentsCount() != findColorModel(colorModel)->getComponentsCount()) {
        showError(L"Cannot reinterpret, there is another amount of components");
        return;
    }

    editedRaster->reinterpretColorModel(colorModel);
    updateEditingImageView();

    updateColorModelUI();
}

void MyApp::convertGamma(float gamma) {
    this->gamma = gamma;
    rightTool.gamma.gammaEdt->setText(std::to_wstring(gamma));
    if (isEditing) {
        editedRaster->convertToGamma(gamma);
        updateEditingImageView();
    }
}

void MyApp::reinterpretGamma(float gamma) {
    this->gamma = gamma;
    rightTool.gamma.gammaEdt->setText(std::to_wstring(gamma));
    if (isEditing) {
        editedRaster->reinterpretGamma(gamma);
        updateEditingImageView();
    }
}

void editRaster(ModernRaster* modernRaster) {
    int componentsCount = modernRaster->getColorModel()->getComponentsCount();

    editedRaster = new ModernRaster(*modernRaster);
    editedTexture = new gl::Texture(*editedRaster);

    imageView->setTexture(editedTexture->getTextureId(), editedTexture->getWidth(), editedTexture->getHeight());
    imageView->setShouldRenderGrid(true);

    for (int i = 0; i < componentsCount; i++) {
        rightTool.colorModel.componentToggle[i]->getParent()->setVisibility(view::VISIBLE);
        setToggleViewActive(i, editedRaster->getFilter(i));
    }
    for (int i = componentsCount; i < 4; i++) {
        rightTool.colorModel.componentToggle[i]->getParent()->setVisibility(view::INVISIBLE);
    }

    imageView->setOnMouseEventListener([](view::View* view, const MouseEvent& e) {
        if (!isEditing)
            return false;
        if (e.button != 1)
            return false;
        if (e.action == GLFW_PRESS) {
            xBegin = imageView->getPointerX();
            yBegin = imageView->getPointerY();
            return true;
        }
        if (e.action == GLFW_RELEASE) {
            drawLine(xBegin, yBegin, imageView->getPointerX(), imageView->getPointerY());
            return true;
        }
        return true;
    });
    updateInfoUI();
    rightTool.gamma.gammaEdt->setText(std::to_wstring(editedRaster->getGamma()));
    rightTool.dithering.bitsSelect->setSelectIndex(editedRaster->getDitheringBits() - 1);
    rightTool.dithering.modeSelect->setSelectIndex(editedRaster->getDitheringMethodEnum() + 1);

    updateColorModelUI();

    isEditing = true;

    rightTool.lay->setVisibility(isEditing ? view::VISIBLE : view::INVISIBLE);
    info() << "isEditing: " << isEditing;
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
        editRaster(imageFileStorage.getCurImageFile()->raster);
    } else {
        delete editedRaster;
        editedRaster = nullptr;
        delete editedTexture;
        editedTexture = nullptr;

        auto imageFile = imageFileStorage.getCurImageFile();
        imageView->setTexture(imageFile->textureId,
                              imageFile->raster->getWidth(),
                              imageFile->raster->getHeight());
        imageView->setShouldRenderGrid(false);

        isEditing = false;
        viewerRootView->setBackground(view::ColorBackground(rgba{0, 0, 0, 255})
        );
    }
    rightTool.lay->setVisibility(isEditing ? view::VISIBLE : view::INVISIBLE);
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

void MyApp::setDitheringBits(int bits) {
    if (!isEditing)
        return;
    editedRaster->setDitheringBits(bits);
    updateEditingImageView();
}

void MyApp::setDitheringMethod(DitheringMethodEnum method) {
    if (!isEditing)
        return;
    editedRaster->setDitheringMethod(method);
    updateEditingImageView();
}

void MyApp::openGrad(int width, int height) {
    auto* grad = new float[width * height];
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            grad[y * width + x] = (float) x / width;
        }
    }
    std::shared_ptr<float[]> raster(grad);
    ModernRaster gradientRaster(width, height, raster, COLOR_MODEL_GRAY);
    if (isEditing) {
        toggleEdit();
    }

    editRaster(&gradientRaster);
}

void MyApp::applyScale(const img::ScaleImageInfo&) {
    if (!isEditing)
        return;

}

MyApp* getAppInstance() {
    return instance;
}
