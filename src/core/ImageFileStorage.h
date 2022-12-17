//
// Created by danil on 28.10.2022.
//

#ifndef MEDGRAPHICS_IMAGE_FILE_STORAGE_H
#define MEDGRAPHICS_IMAGE_FILE_STORAGE_H

#include "ImageFile.h"
#include "CyclicImageFileSpan.h"
#include "ObservableValue.h"
#include <thread>
#include <condition_variable>
#include <set>

#define IMG_LOAD_R 2
#define IMG_LOAD_D (IMG_LOAD_R + IMG_LOAD_R + 1)

class ImageFileStorage {
public:
    ImageFileStorage();

    void open(const fs::path& file);

    void update();

    void next();

    void prev();

    void chooseIndex(int index);

    ImageFile* getCurImageFile();

    ReadOnlyObservableValue<gl::Texture*>& getObservableTexture();

    ReadOnlyObservableValue<std::filesystem::path>& getObservablePath();

    int getCurImageIndex() const;

    CyclicImageFileSpan nearImageFiles();

    CyclicImageFileSpan notNearImageFiles();

private:
    void setCurDir(const fs::path& dir);

    [[noreturn]] void runService();

    fs::path curPath;
    fs::path curDir;
    int curIndex = 0;
    std::vector<ImageFile> curDirImageFiles;

    int prevNotifiedIndex = -1;
    std::thread serviceThread;
    std::mutex mutex;
    std::condition_variable notifier;

    ObservableValue<gl::Texture*> observableTexture;
    ObservableValue<std::filesystem::path> observablePath;
};

#endif //MEDGRAPHICS_IMAGE_FILE_STORAGE_H
