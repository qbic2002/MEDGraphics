//
// Created by golov on 15.09.2022.
//

#include <string>
#include "img/pnmUtils.h"
#include "utils/file.h"
#include "utils/logging.h"

PNMMode readPnmMode(const char* data, int& offset) {
    if (data[0] == 'P') {
        if (data[1] == '5') {
            offset += 2;
            return PNMMode::P5;
        }
        if (data[1] == '6') {
            offset += 2;
            return PNMMode::P6;
        }
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

PNMHeader readPnmHeader(const char* data, int& offset) {
    return {.mode = readPnmMode(data, offset),
            .width = parsePnmInt(data, offset),
            .height = parsePnmInt(data, offset),
            .maxValue = parsePnmInt(data, offset)};
}

PNMMeta parseMeta(const char* fileData, int headerSize) {
    PNMMeta pnmMeta;
    for (int i = 0; i < headerSize; ++i) {
        char ch = fileData[i];
        if (ch == '#') {
            std::string metaInf;
            int offset = i;
            metaInf.push_back(ch);
            ch = fileData[++i];
            while (!isNewLine(ch)) {
                metaInf.push_back(ch);
                ch = fileData[++i];
            }

            pnmMeta.addMeta(metaInf, offset);
        }
    }

    return pnmMeta;
}

ModernRaster* parseData(const unsigned char* data, int offset, const PNMHeader& header, unsigned int length) {
    data += offset;

    const ColorModel* colorModel = findColorModel(header.mode == PNMMode::P5 ? COLOR_MODEL_GRAY : COLOR_MODEL_RGB);
    int values = colorModel->getComponentsCount() * header.width * header.height;
    int bytesPerValue = (header.maxValue <= 255) ? 1 : 2;
    int bytesToRead = values * bytesPerValue;

    if (offset + bytesToRead < length)
        throw std::runtime_error("more bytes expected than provided");

    auto maxValue = (float) header.maxValue;
    std::shared_ptr<float[]> rasterData = std::shared_ptr<float[]>(new float[bytesToRead]);
    if (bytesPerValue == 1) {
        for (int i = 0; i < values; i++) {
            rasterData[i] = data[i] / maxValue;
        }
    } else {
        for (int i = 0; i < values; i++) {
            float value = (data[i * 2] << 8) | data[i * 2 + 1];
            rasterData[i] = value / maxValue;
        }
    }

    return new ModernRaster(header.width, header.height, rasterData, colorModel);
}

namespace pnm {
    PNMImage readPNMImageFromMemory(const char* data, unsigned int length) {
        int offset = 0;
        auto header = readPnmHeader(data, offset);
        auto meta = parseMeta(data, offset);
        auto raster = parseData((unsigned char*) data, offset, header, length);

        for (const auto& item: meta.getMeta()) {
            info() << item.first;
        }
        return PNMImage(header, meta, *raster);
    }

    void writePNMImage(const PNMImage& pnmImage, std::ofstream& os) {
        std::string mode;
        if (pnmImage.header.mode == PNMMode::P5) {
            mode = "P5";
        }
        if (pnmImage.header.mode == PNMMode::P6) {
            mode = "P6";
        }

        std::string width = std::to_string(pnmImage.header.width);
        std::string height = std::to_string(pnmImage.header.height);

        std::string maxGray = std::to_string(pnmImage.header.maxValue);

        std::string data;

        auto& raster = pnmImage.raster;
        int length = raster.getWidth() * raster.getHeight()
                     * raster.getColorModel()->getComponentsCount();
        const float* dataF = raster.getData();
        for (int i = 0; i < length; i++) {
            data.push_back((unsigned char) (dataF[i] * pnmImage.header.maxValue));
        }

        std::string result;
        result
                .append(mode)
                .append("\n")
                .append(width).append(" ").append(height)
                .append("\n")
                .append(maxGray)
                .append("\n")
                .append(data);

        for (const auto& item: pnmImage.meta.getMeta()) {
            std::string curr_meta = item.first + '\n';
            result.insert(item.second, curr_meta);
        }
        os.write(result.c_str(), result.length());
        os.flush();
    }

    void writePNMImage(const PNMImage& pnmImage, const std::filesystem::path& filename) {
        std::ofstream fileStream = utils::openFileOStream(filename);
        writePNMImage(pnmImage, fileStream);
    }
}
