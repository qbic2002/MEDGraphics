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

    void onKey(int key, int scancode, int action, int mods) override;

protected:
    ImageFileStorage imageFileStorage;
};


#endif //MEDGRAPHICS_MYAPP_H
