//
// Created by golov on 15.09.2022.
//

#include <string>
#include "pnmUtil.h"
#include "fileUtil.h"
#include "../../utils/file.h"

int parsePnmMode(const char* fileData, PNMMode& pnmMode) {
    if (fileData[0] == 'P' && fileData[1] == '5') {
        pnmMode = PNMMode::P5;
        return 2;
    }

    if (fileData[0] == 'P' && fileData[1] == '6') {
        pnmMode = PNMMode::P6;
        return 2;
    }

    return -1;
}

int parsePnmWidth(const char* fileData, int offset, int& width) {
    int i = offset;

    std::string widthString;
    while (true) {
        char ch = fileData[i++];
        if (ch == '#') {
            while (fileData[i++] != '\n') {

            }
        }
        if (ch >= '0' && ch <= '9') {
            widthString.push_back(ch);
        } else if (widthString.length() != 0) {
            break;
        }
    }

    width = std::stoi(widthString);

    return i;
}

int parsePnmHeight(const char* fileData, int offset, int& height) {
    int i = offset;

    std::string heightString;
    while (true) {
        char ch = fileData[i++];
        if (ch == '#') {
            while (fileData[i++] != '\n') {

            }
        }
        if (ch >= '0' && ch <= '9') {
            heightString.push_back(ch);
        } else if (heightString.length() != 0) {
            break;
        }
    }

    height = std::stoi(heightString);

    return i;
}

int parsePnmMaxGrey(const char* fileData, int offset, int& maxGrey) {
    int i = offset;

    std::string maxGreyString;
    while (true) {
        char ch = fileData[i++];
        if (ch == '#') {
            while (fileData[i++] != '\n') {

            }
        }
        if (ch >= '0' && ch <= '9') {
            maxGreyString.push_back(ch);
        } else if (maxGreyString.length() != 0) {
            break;
        }
    }

    maxGrey = std::stoi(maxGreyString);

    return i;
}

int pnm::parsePnmHeader(const char* fileData, PNMHeader& pnmHeader) {
    PNMMode pnmMode;

    int offset;
    offset = parsePnmMode(fileData, pnmMode);
    if (offset == -1) {
        return -1;
    }

    int width;
    offset = parsePnmWidth(fileData, offset, width);

    int height;
    offset = parsePnmHeight(fileData, offset, height);

    int maxGrey;
    offset = parsePnmMaxGrey(fileData, offset, maxGrey);

    pnmHeader.pnmMode = pnmMode;
    pnmHeader.width = width;
    pnmHeader.height = height;
    pnmHeader.maxGrey = maxGrey;

    return offset;
}

AbstractRaster*
pnm::parseData(const unsigned char* fileData, int offset, const PNMHeader& pnmHeader, unsigned int length) {

    int k = 0;
    if (pnmHeader.pnmMode == PNMMode::P5) {
        if (offset + pnmHeader.width * pnmHeader.height > length)
            throw std::exception();

        auto* grayData = new Raster<GrayPixel>(pnmHeader.width, pnmHeader.height);
        for (int j = 0; j < pnmHeader.height; ++j) {
            for (int i = 0; i < pnmHeader.width; ++i) {
                unsigned char color = fileData[offset + k++];
                if (color > pnmHeader.maxGrey) {
                    throw std::exception();
                }
                grayData->set(i, j, GrayPixel(color));
            }
        }

        return grayData;
    }

    if (pnmHeader.pnmMode == PNMMode::P6) {
        if (offset + pnmHeader.width * pnmHeader.height * 3 > length)
            throw std::exception();

        auto* rgbaData = new Raster<RGBAPixel>(pnmHeader.width, pnmHeader.height);

        for (int j = 0; j < pnmHeader.height; ++j) {
            for (int i = 0; i < pnmHeader.width; ++i) {
                unsigned char r = fileData[offset + k++];
                unsigned char g = fileData[offset + k++];
                unsigned char b = fileData[offset + k++];
                if (r > pnmHeader.maxGrey || g > pnmHeader.maxGrey || b > pnmHeader.maxGrey) {
                    throw std::exception();
                }
                rgbaData->set(i, j, RGBAPixel(r, g, b, 255));
            }
        }

        return rgbaData;
    }

    throw std::exception();
}

PNMImage pnm::readPNMImageFromMemory(const char* data, unsigned int length) {
    PNMImage pnmImage{};

    int offset = pnm::parsePnmHeader(data, pnmImage.pnmHeader);
    if (offset == -1) {
        throw std::exception();
    }

    pnmImage.pnmMeta = pnm::parseMeta(data, offset);


    pnmImage.data = pnm::parseData((unsigned char*) data, offset, pnmImage.pnmHeader, length);

    for (const auto& item: pnmImage.pnmMeta.getMeta()) {
        std::cout << item.first;
    }
    return std::move(pnmImage);
}

PNMImage pnm::readPNMImage(const char* fileName) {
    std::vector<char> data = utils::readAllBytes(fileName);
    return readPNMImageFromMemory(data.data(), data.size());
}

PNMMeta pnm::parseMeta(const char* fileData, int headerSize) {
    PNMMeta pnmMeta;
    for (int i = 0; i < headerSize; ++i) {
        char ch = fileData[i];
        if (ch == '#') {
            std::string metaInf;
            int offset = i;
            metaInf.push_back(ch);
            ch = fileData[++i];
            while (ch != '\n') {
                metaInf.push_back(ch);
                ch = fileData[++i];
            }
            metaInf.push_back(ch);
            pnmMeta.addMeta(metaInf, offset);
        }
    }

    return pnmMeta;
}

bool pnm::writePNMImage(const PNMImage& pnmImage, const char* filename) {
    std::string mode;
    if (pnmImage.pnmHeader.pnmMode == PNMMode::P5) {
        mode = "P5";
    }
    if (pnmImage.pnmHeader.pnmMode == PNMMode::P6) {
        mode = "P6";
    }

    std::string width = std::to_string(pnmImage.pnmHeader.width);
    std::string height = std::to_string(pnmImage.pnmHeader.height);

    std::string maxGrey = std::to_string(pnmImage.pnmHeader.maxGrey);

    std::string data;
    if (pnmImage.pnmHeader.pnmMode == PNMMode::P5) {
        for (int i = 0; i < pnmImage.pnmHeader.height; i++) {
            for (int j = 0; j < pnmImage.pnmHeader.width; ++j) {
                data.push_back(((Raster<GrayPixel>*) pnmImage.data)->get(j, i).grayScale);
            }
        }
    }
    if (pnmImage.pnmHeader.pnmMode == PNMMode::P6) {
        for (int i = 0; i < pnmImage.pnmHeader.height; i++) {
            for (int j = 0; j < pnmImage.pnmHeader.width; ++j) {
                data.push_back(((Raster<RGBAPixel>*) pnmImage.data)->get(j, i).r);
                data.push_back(((Raster<RGBAPixel>*) pnmImage.data)->get(j, i).g);
                data.push_back(((Raster<RGBAPixel>*) pnmImage.data)->get(j, i).b);
            }
        }
    }

    std::string result;
    result
            .append(mode)
            .append("\n")
            .append(width).append(" ").append(height)
            .append("\n")
            .append(maxGrey)
            .append("\n")
            .append(data);

    for (const auto& item: pnmImage.pnmMeta.getMeta()) {
        result.insert(item.second, item.first);
    }
    std::ofstream fileStream(filename, std::ios::binary);
    writeToFile(result.c_str(), fileStream, result.length());
    fileStream.close();

    return true;
}

PNMImage pnm::convertP6ToP5(const PNMImage& other) {
    PNMImage pnmImage;
    pnmImage.pnmHeader = other.pnmHeader;
    pnmImage.pnmHeader.pnmMode = PNMMode::P5;

    pnmImage.pnmMeta = other.pnmMeta;

    auto* grayRaster = new Raster<GrayPixel>(other.data->getWidth(), other.data->getHeight());
    for (unsigned int i = 0; i < other.data->getWidth() * other.data->getHeight(); ++i) {
        grayRaster->set(i, ((Raster<RGBAPixel>*) other.data)->get(i).toGray());
    }

    pnmImage.data = grayRaster;
    return pnmImage;
}
