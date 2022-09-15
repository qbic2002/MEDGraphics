//
// Created by golov on 15.09.2022.
//

#include <string>
#include "pnmUtil.h"
#include "fileUtil.h"

int parsePnmMode(const char* fileData, PNMMode& pnmMode){
    if (fileData[0] == 'P' && fileData[1] == '5'){
        pnmMode = PNMMode::P5;
        return 2;
    }

    if (fileData[0] == 'P' && fileData[1] == '6'){
        pnmMode = PNMMode::P6;
        return 2;
    }

    return -1;
}

int parsePnmWidth(const char *fileData, int offset, int& width){
    int i = offset;

    std::string widthString;
    while(true){
        char ch = fileData[i++];
        if (ch >= '0' && ch <= '9') {
            widthString.push_back(ch);
        } else if (widthString.length() != 0){
            break;
        }
    }

    width = std::stoi(widthString);

    return i;
}

int parsePnmHeight(const char *fileData, int offset, int& height){
    int i = offset;

    std::string heightString;
    while(true){
        char ch = fileData[i++];
        if (ch >= '0' && ch <= '9') {
            heightString.push_back(ch);
        } else if (heightString.length() != 0){
            break;
        }
    }

    height = std::stoi(heightString);

    return i;
}

int parsePnmMaxGrey(const char *fileData, int offset, int& maxGrey){
    int i = offset;

    std::string maxGreyString;
    while(true){
        char ch = fileData[i++];
        if (ch >= '0' && ch <= '9') {
            maxGreyString.push_back(ch);
        } else if (maxGreyString.length() != 0){
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
    if (offset ==-1){
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

RGBAData pnm::parseData(const unsigned char *fileData, int offset, const PNMHeader& pnmHeader) {
    RGBAData rgbaData(pnmHeader.width, pnmHeader.height);

    int k = 0;
    for (int i = 0; i < pnmHeader.width; ++i){
        for (int j = 0; j < pnmHeader.height; ++j) {
            if (pnmHeader.pnmMode == PNMMode::P5){
                unsigned char color = fileData[offset + k++];
                if (color > pnmHeader.maxGrey){
                    throw -1;
                }
                rgbaData.set(i, j, RGBAPixel(color, color, color, 255));
            }

            if (pnmHeader.pnmMode == PNMMode::P6){
                unsigned char r = fileData[offset + k++];
                unsigned char g = fileData[offset + k++];
                unsigned char b = fileData[offset + k++];
                if (r > pnmHeader.maxGrey || g > pnmHeader.maxGrey || b > pnmHeader.maxGrey){
                    throw -1;
                }
                rgbaData.set(i, j, RGBAPixel(r, g, b, 255));
            }
        }
    }

    return rgbaData;
}

PNMImage pnm::readPNMImage(const char *fileName) {
    PNMImage pnmImage{};

    std::ifstream fileStream(fileName, std::ios::binary);
    int size = getFileSize(fileStream);

    char* fileData = new char[size];
    getFileContent(fileData, fileStream, size);

    int offset = pnm::parsePnmHeader(fileData, pnmImage.pnmHeader);
    if (offset == -1){
        throw -1;
    }

    pnmImage.rgbaData = pnm::parseData((unsigned char*)fileData, offset, pnmImage.pnmHeader);

    delete[] fileData;
    fileStream.close();

    return pnmImage;
}