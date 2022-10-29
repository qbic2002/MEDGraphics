//
// Created by danil on 28.10.2022.
//

#include "ImageFileStorage.h"
#include "../utils/logging.h"
#include "../gl/utils.h"
#include "../img/imageLoader.h"
#include "../utils/measureTime.h"
#include "CyclicImageFileSpan.h"

ImageFileStorage::ImageFileStorage() {
    serviceThread = std::thread(&ImageFileStorage::runService, this);
    serviceThread.detach();
}

void ImageFileStorage::open(const std::filesystem::path& file) {
    info() << "open " << file << std::endl;
    {
        std::lock_guard lock(mutex);
        if (!exists(file)) {
            curPath = "";
            curDirImageFiles.clear();
            curIndex = -1;
            prevNotifiedIndex = -1;
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
    for (auto& imageFile: nearImageFiles()) {
        if (imageFile.raster != nullptr && imageFile.textureId == 0) {
            info() << "load texture " << imageFile.getPath().filename().string() << std::endl;
            imageFile.textureId = gl::loadTexture(imageFile.raster, GL_CLAMP, GL_LINEAR, GL_NEAREST);
        }
    }

    for (auto& imageFile: notNearImageFiles()) {
        if (imageFile.raster != nullptr) {
            info() << "unload texture " << imageFile.getPath().filename().string() << std::endl;
            imageFile.deleteRaster();
            imageFile.deleteTexture();
        }
    }

    if (curIndex != prevAnnouncedIndex && (curDirImageFiles.empty() || curDirImageFiles[curIndex].textureId != 0)) {
        for (auto& listener: onImageChangedListeners) {
            listener();
        }
        prevAnnouncedIndex = curIndex;
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

    notifier.notify_one();
}

ImageFile* ImageFileStorage::getCurImageFile() {
    if (curDirImageFiles.empty())
        return nullptr;
    return &curDirImageFiles[curIndex];
}

void ImageFileStorage::addImageChangedListener(const std::function<void()>& listener) {
    onImageChangedListeners.push_back(listener);
}

void ImageFileStorage::setCurDir(const fs::path& dir) {
    info() << "setCurDir " << dir << std::endl;
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
    info() << "setCurDir " << dir << " finished" << std::endl;
}

void ImageFileStorage::runService() {
    while (true) {
        std::unique_lock lock(mutex);
        if (prevNotifiedIndex == curIndex)
            notifier.wait(lock);
        prevNotifiedIndex = curIndex;

        for (auto& imageFile: nearImageFiles()) {
            if (imageFile.raster == nullptr) {
                utils::TimeStamp loadImageTs;
                imageFile.raster = img::loadImageData(imageFile.getPath());
                loadImageTs.report("load image data " + imageFile.getPath().filename().string());
            }
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

