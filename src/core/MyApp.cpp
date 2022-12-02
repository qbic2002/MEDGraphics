//
// Created by danil on 11.11.2022.
//

#include "MyApp.h"
#include "utils/explorer_utils.h"
#include "img/pnmUtils.h"
#include "../widget/ImageView.h"
#include "MyLayout.h"
#include "../widget/EditText.h"
#include "../widget/SelectView.h"

MyApp* instance = nullptr;

view::View* viewerRootView = nullptr;

view::Dialog* messageDialog = nullptr;
view::TextView* messageDialogTxt = nullptr;

view::ImageView* imageView = nullptr;

bool isEditing = false;

ModernRaster* editedRaster;
gl::Texture* editedTexture;

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

    imageFileStorage.open(args[1]);
    imageFileStorage.addImageChangedListener([&]() {
        auto* imageFile = imageFileStorage.getCurImageFile();
        imageView->setTexture(imageFile->textureId, imageFile->raster->getWidth(), imageFile->raster->getHeight());
        imageView->invalidate();
    });
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

void updateEditingImageView() {
    editedTexture->update(*editedRaster);
    imageView->setTexture(editedTexture->getTextureId(), editedTexture->getWidth(), editedTexture->getHeight());
}

void setToggleViewActive(int index, bool value) {
    rightTool.colorModel.componentToggle[index]->getParent()->setBackground(value
                                                                            ? view::theme.activeComponentBackground
                                                                            : view::theme.notActiveComponentBackground);
}

void updateColorModelUI() {
    if (!editedRaster)
        return;
    auto colorModel = editedRaster->getColorModel();
    int index = -1;
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

    for (int i = 0; i < componentsCount; i++) {
        rightTool.colorModel.componentToggle[i]->getParent()->setVisibility(view::VISIBLE);
        setToggleViewActive(i, editedRaster->getFilter(i));
    }
    for (int i = componentsCount; i < 4; i++) {
        rightTool.colorModel.componentToggle[i]->getParent()->setVisibility(view::INVISIBLE);
    }

    rightTool.info.widthTxt->setText(std::to_wstring(editedRaster->getWidth()));
    rightTool.info.heightTxt->setText(std::to_wstring(editedRaster->getHeight()));
    rightTool.info.channelsTxt->setText(std::to_wstring(editedRaster->getColorModel()->getComponentsCount()));
    rightTool.gamma.gammaEdt->setText(std::to_wstring(editedRaster->getGamma()));
    rightTool.dithering.bitsSelect->setSelectIndex(editedRaster->getDitheringBits() - 1);
    rightTool.dithering.modeSelect->setSelectIndex(editedRaster->getDitheringMethodEnum() + 1);

//        float col[] = {1, 1, 0};
//        editedRaster->drawLine({10, 35}, {500, 400}, col, 5, 0.7);

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
        imageView->setTexture(imageFile->textureId, imageFile->raster->getWidth(), imageFile->raster->getHeight());

        isEditing = false;
        viewerRootView->setBackground(view::ColorBackground(rgba{0, 0, 0, 255}));
    }
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

MyApp* getAppInstance() {
    return instance;
}
