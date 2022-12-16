//
// Created by danil on 11.11.2022.
//

#ifndef MEDGRAPHICS_MYAPP_H
#define MEDGRAPHICS_MYAPP_H

#include <view/core/Context.h>
#include "ImageFileStorage.h"
#include <vector>
#include <utils/String.h>
#include "R.h"
#include "img/ImageProcessing.h"

class MyApp : public Context {
public:
    void onCreated(const std::vector<std::wstring>& args) override;

    void update() override;

    /// @return current context's ImageFileStorage
    ImageFileStorage& getImageFileStorage();

    /// Run save image operation of the opened image
    void saveImage();

    /// Run open image operation from filesystem
    void openImage();

    void openGrad(int width, int height);

    /// Display message box with the message
    /// @param message Error message to be shown
    static void showError(const String& message);

    bool onKey(int key, int scancode, int action, int mods) override;

    void selectColorModel(ColorModelEnum colorModelEnum);

    void convertColorModel(ColorModelEnum colorModel);

    void reinterpretColorModel(ColorModelEnum colorModel);

    void convertGamma(float gamma);

    void reinterpretGamma(float gamma);

    void toggleEdit();

    void toggleComponent(int index);

    void setDitheringBits(int bits);

    void setDitheringMethod(DitheringMethodEnum method);

    void applyScale(const img::ScaleImageInfo& scaleImageInfo);

    void resetAllHistograms();

    void printRBGHistogram();

    void printHistograms();

    void drawLine(int x1, int y1, int x2, int y2);

    void editRaster(ModernRaster* modernRaster);

    void updateEditingImageView();

protected:
    ImageFileStorage imageFileStorage;
//    ColorModelEnum colorModelEnum = COLOR_MODEL_RGB;
    float gamma = 1;
};

MyApp* getAppInstance();

#endif //MEDGRAPHICS_MYAPP_H
