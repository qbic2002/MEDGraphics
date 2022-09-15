//
// Created by golov on 15.09.2022.
//

#include <string>
#include "pnmUtil.h"

int parsePnmMode(const char* fileData, PNMMode& pnmMode){
    if (fileData[0] == 'P' || fileData[1] == '5'){
        pnmMode = PNMMode::P5;
        return 2;
    }

    if (fileData[0] == 'P' || fileData[1] == '6'){
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

int pnm::parsePnmHeader(const char *fileData, PNMImage &pnmImage) {
    PNMMode pnmMode;

    int offset;
    offset = parsePnmMode(fileData, pnmMode);
    if (offset ==-1){
        return false;
    }

    int width;
    offset = parsePnmWidth(fileData, offset, width);

    int height;
    offset = parsePnmHeight(fileData, offset, height);

    int maxGrey;
    offset = parsePnmMaxGrey(fileData, offset, maxGrey);

    pnmImage.pnmMode = pnmMode;
    pnmImage.width = width;
    pnmImage.height = height;
    pnmImage.maxGrey = maxGrey;

    return offset;
}
