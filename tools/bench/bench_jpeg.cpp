//
// Created by danil on 23.12.2022.
//

#include "utils/encoding.h"
#include "utils/file.h"
#include "img/format/jpeg_utils.h"
#include "utils/measureTime.h"

int main() {
    utils::configureUtf8();
    auto args = utils::readArgs();
    if (args.size() <= 1) {
        std::cout << "provide image file path" << std::endl;
        return 1;
    }
    auto is = utils::openFileIStream(args[1]);
    try {
        utils::TimeStamp timeStamp;
        auto* raster = img::readJpegImage(is);
        if (raster == nullptr) {
            std::cout << "WARN - Failed to read jpeg image, returned nullptr" << std::endl;
        }
        delete raster;
        timeStamp.report("read JPEG file");
    } catch (const std::exception& e) {
        std::cout << "WARN - Could not read jpeg image: " << e.what() << std::endl;
    }
    is.close();
    return 0;
}