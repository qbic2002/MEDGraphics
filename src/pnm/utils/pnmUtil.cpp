//
// Created by golov on 15.09.2022.
//

#include <string>
#include "pnmUtil.h"
#include "../../utils/file.h"

PNMMode readPnmMode(const char* fileData) {
    if (fileData[0] == 'P') {
        if (fileData[1] == '5')
            return PNMMode::P5;
        if (fileData[1] == '6')
            return PNMMode::P6;
    }

    throw std::exception();
}

inline bool isDigit(char c) {
    return '0' <= c && c <= '9';
}

inline bool isNewLine(char c) {
    return c == '\n' || c == '\r';
}

int parsePnmInt(const char* data, int& offset) {
    int num = 0;
    bool hasReadNum = false;

    while (true) {
        if (data[offset] == '#') {
            while (!isNewLine(data[offset])) {
                offset++;
            }
            continue;
        } else if (isDigit(data[offset])) {
            num = num * 10 + data[offset] - '0';
            hasReadNum = true;
        } else if (hasReadNum) {
            break;
        }
        offset++;
    }
    offset++;

    return num;
}

int pnm::parsePnmHeader(const char* fileData, PNMHeader& pnmHeader) {
    pnmHeader.pnmMode = readPnmMode(fileData);
    int offset = 2;
    pnmHeader.width = parsePnmInt(fileData, offset);
    pnmHeader.height = parsePnmInt(fileData, offset);
    pnmHeader.maxGray = parsePnmInt(fileData, offset);
    return offset;
}

AbstractRaster*
pnm::parseData(const unsigned char* fileData, int offset, const PNMHeader& pnmHeader, unsigned int length) {
    int i = offset;
    switch (pnmHeader.pnmMode) {
        case PNMMode::P5: {
            if (offset + pnmHeader.width * pnmHeader.height > length)
                throw std::exception();

            auto* raster = new Raster<PixelGray8>(pnmHeader.width, pnmHeader.height);
            for (int y = 0; y < pnmHeader.height; ++y) {
                for (int x = 0; x < pnmHeader.width; ++x) {
                    unsigned char color = fileData[i++];
                    if (color > pnmHeader.maxGray) {
                        throw std::exception();
                    }
                    raster->set(x, y, PixelGray8(color));
                }
            }

            return raster;
        }
        case PNMMode::P6: {
            if (offset + pnmHeader.width * pnmHeader.height * 3 > length)
                throw std::exception();

            auto* raster = new Raster<PixelRGB8>(pnmHeader.width, pnmHeader.height);

            for (int y = 0; y < pnmHeader.height; ++y) {
                for (int x = 0; x < pnmHeader.width; ++x) {
                    unsigned char r = fileData[i++];
                    unsigned char g = fileData[i++];
                    unsigned char b = fileData[i++];
                    if (r > pnmHeader.maxGray || g > pnmHeader.maxGray || b > pnmHeader.maxGray) {
                        throw std::exception();
                    }
                    raster->set(x, y, {r, g, b});
                }
            }

            return raster;
        }
    }

    throw std::exception();
}

PNMImage pnm::readPNMImageFromMemory(const char* data, unsigned int length) {
    PNMImage pnmImage{};

    int offset = pnm::parsePnmHeader(data, pnmImage.pnmHeader);

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

bool pnm::writePNMImage(const PNMImage& pnmImage, std::ofstream& os) {
    std::string mode;
    if (pnmImage.pnmHeader.pnmMode == PNMMode::P5) {
        mode = "P5";
    }
    if (pnmImage.pnmHeader.pnmMode == PNMMode::P6) {
        mode = "P6";
    }

    std::string width = std::to_string(pnmImage.pnmHeader.width);
    std::string height = std::to_string(pnmImage.pnmHeader.height);

    std::string maxGrey = std::to_string(pnmImage.pnmHeader.maxGray);

    std::string data;
    if (pnmImage.pnmHeader.pnmMode == PNMMode::P5) {
        for (int i = 0; i < pnmImage.pnmHeader.height; i++) {
            for (int j = 0; j < pnmImage.pnmHeader.width; ++j) {
                data.push_back(((Raster<PixelGray8>*) pnmImage.data)->get(j, i).grayScale);
            }
        }
    }
    if (pnmImage.pnmHeader.pnmMode == PNMMode::P6) {
        for (int i = 0; i < pnmImage.pnmHeader.height; i++) {
            for (int j = 0; j < pnmImage.pnmHeader.width; ++j) {
                data.push_back(((Raster<PixelRGBA8>*) pnmImage.data)->get(j, i).r);
                data.push_back(((Raster<PixelRGBA8>*) pnmImage.data)->get(j, i).g);
                data.push_back(((Raster<PixelRGBA8>*) pnmImage.data)->get(j, i).b);
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
    os.write(result.c_str(), result.length());
    os.flush();
    return true;
}

bool pnm::writePNMImage(const PNMImage& pnmImage, const char* filename) {
    std::ofstream fileStream(filename, std::ios::binary);
    if (!fileStream.is_open()) {
        throw std::exception();
    }
    return writePNMImage(pnmImage, fileStream);
}

PNMImage pnm::convertP6ToP5(const PNMImage& other) {
    PNMImage pnmImage;
    pnmImage.pnmHeader = other.pnmHeader;
    pnmImage.pnmHeader.pnmMode = PNMMode::P5;

    pnmImage.pnmMeta = other.pnmMeta;

    auto* grayRaster = new Raster<PixelGray8>(other.data->getWidth(), other.data->getHeight());
    for (unsigned int i = 0; i < other.data->getWidth() * other.data->getHeight(); ++i) {
        grayRaster->set(i, ((Raster<PixelRGBA8>*) other.data)->get(i).toGray());
    }

    pnmImage.data = grayRaster;
    return pnmImage;
}
