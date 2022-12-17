//
// Created by danil on 28.10.2022.
//

#include "ImageFileStorage.h"
#include "CyclicImageFileSpan.h"

ImageFileStorage::ImageFileStorage() {
    serviceThread = std::thread(&ImageFileStorage::runService, this);
    serviceThread.detach();
}

void ImageFileStorage::open(const std::filesystem::path& file) {
    info() << "open " << file;
    {
        std::lock_guard lock(mutex);
        if (!exists(file)) {
            curPath = "";
            curDirImageFiles.clear();
            curIndex = -1;
            prevNotifiedIndex = -1;
            observableTexture.set(nullptr);
            observablePath.set("");
        } else {
            curPath = fs::canonical(file);
            setCurDir(fs::is_directory(curPath) ? curPath : curPath.parent_path());
            for (int i = 0; i < curDirImageFiles.size(); ++i) {
                if (curPath == curDirImageFiles[i].getPath()) {
                    curIndex = i;
                    break;
                }
            }
        }
    }
    notifier.notify_one();
}

void ImageFileStorage::update() {
    for (auto& imageFile: curDirImageFiles) {
        imageFile.verifyTextureLoaded();
    }
    auto* curImageFile = getCurImageFile();
    if (curImageFile == nullptr) {
        observableTexture.set(nullptr);
        observablePath.set("");
    } else {
        observableTexture.set(curImageFile->texture);
        observablePath.set(curImageFile->getPath());
    }
}

void ImageFileStorage::next() {
    chooseIndex(curIndex + 1);
}

void ImageFileStorage::prev() {
    chooseIndex(curIndex - 1);
}

void ImageFileStorage::chooseIndex(int index) {
    if (curDirImageFiles.empty())
        return;
    if (index < 0)
        index = curDirImageFiles.size() - 1;
    else if (index >= curDirImageFiles.size())
        index = 0;
    curIndex = index;

    for (auto& imageFile: notNearImageFiles()) {
        imageFile.shouldBeLoaded = false;
    }
    for (auto& imageFile: nearImageFiles()) {
        imageFile.shouldBeLoaded = true;
    }

    notifier.notify_one();
}

ImageFile* ImageFileStorage::getCurImageFile() {
    if (curDirImageFiles.empty())
        return nullptr;
    return &curDirImageFiles[curIndex];
}

ReadOnlyObservableValue<gl::Texture*>& ImageFileStorage::getObservableTexture() {
    return observableTexture;
}

ReadOnlyObservableValue<std::filesystem::path>& ImageFileStorage::getObservablePath() {
    return observablePath;
}

void ImageFileStorage::setCurDir(const fs::path& dir) {
    info() << "setCurDir " << dir;
    curDir = dir;
    curDirImageFiles.clear();

    for (const auto& dirEntry: fs::directory_iterator(curDir)) {
        const auto& file = dirEntry.path();
        if (fs::is_directory(file))
            continue;
        if (img::isImage(file)) {
            curDirImageFiles.emplace_back(file);
        }
    }
    info() << "setCurDir " << dir << " finished";
}

[[noreturn]] void ImageFileStorage::runService() {
    while (true) {
        std::unique_lock lock(mutex);
        if (prevNotifiedIndex == curIndex)
            notifier.wait(lock);
        prevNotifiedIndex = curIndex;

        for (auto& imageFile: curDirImageFiles) {
            imageFile.verifyRasterLoaded();
        }

        lock.unlock();
    }
}

int ImageFileStorage::getCurImageIndex() const {
    return curIndex;
}

CyclicImageFileSpan ImageFileStorage::nearImageFiles() {
    if (curDirImageFiles.empty()) {
        return {CyclicVectorIterator(curDirImageFiles, 0),
                CyclicVectorIterator(curDirImageFiles, 0)};
    } else {
        return {CyclicVectorIterator(curDirImageFiles, curIndex - IMG_LOAD_R),
                CyclicVectorIterator(curDirImageFiles, curIndex + IMG_LOAD_R + 1)};
    }
}

CyclicImageFileSpan ImageFileStorage::notNearImageFiles() {
    if (curDirImageFiles.size() <= (IMG_LOAD_R * 2 + 1)) {
        return {CyclicVectorIterator(curDirImageFiles, 0),
                CyclicVectorIterator(curDirImageFiles, 0)};
    } else {
        return {CyclicVectorIterator(curDirImageFiles, curIndex + IMG_LOAD_R + 1),
                CyclicVectorIterator(curDirImageFiles, curIndex - IMG_LOAD_R + curDirImageFiles.size())};
    }
}

