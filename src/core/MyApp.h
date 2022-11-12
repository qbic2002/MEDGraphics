//
// Created by danil on 11.11.2022.
//

#ifndef MEDGRAPHICS_MYAPP_H
#define MEDGRAPHICS_MYAPP_H

#include "Context.h"
#include "ImageFileStorage.h"
#include <vector>
#include <string>
#include "../utils/R.h"

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

    /// Display message box with the message
    /// @param message Error message to be shown
    void showError(const String& message);

    bool grabbing = false;
    double prevX, prevY;
    double grabX, grabY;
    int grabWindowX, grabWindowY;

    void onMouseMove(double x, double y) override;

    void onMouseButton(ClickEvent& event) override;

    void onMouseLeave() override;

    void onScroll(double xOffset, double yOffset, double cursorX, double cursorY) override;

    void onKey(int key, int scancode, int action, int mods) override;

protected:
    ImageFileStorage imageFileStorage;
};


#endif //MEDGRAPHICS_MYAPP_H
