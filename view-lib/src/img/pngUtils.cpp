//
// Created by golov on 08.12.2022.
//

#include "img/pngUtils.h"
#include "utils/logging.h"

const unsigned char pngSignature[8] = {137, 80, 78, 71, 13, 10, 26, 10};

CHUNK_TYPE getChunkType(const std::string& chunkTypeString_) {
    std::string chunkTypeString = chunkTypeString_;
    std::transform(chunkTypeString.begin(), chunkTypeString.end(), chunkTypeString.begin(), ::toupper);
    if (chunkTypeString == "IHDR") {
        return CHUNK_TYPE_IHDR;
    }
    if (chunkTypeString == "PLTE") {
        return CHUNK_TYPE_PLTE;
    }
    if (chunkTypeString == "IDAT") {
        return CHUNK_TYPE_IDAT;
    }
    if (chunkTypeString == "IEND") {
        return CHUNK_TYPE_IEND;
    }
    if (chunkTypeString == "GAMA") {
        return CHUNK_TYPE_GAMMA;
    }

    return CHUNK_TYPE_UNKNOWN;

}

bool checkSignature(const unsigned char* data) {
    for (int i = 0; i < 8; ++i) {
        if (pngSignature[i] != data[i]) {
            return false;
        }
    }
    return true;
}

template<class DestType>
DestType getFromDataWithSkip(const char*& data) {
    char d[sizeof(DestType)];
    for (int i = 0; i < sizeof(DestType); ++i) {
        d[i] = data[sizeof(DestType) - 1 - i];
    }
    DestType result = *((DestType*) d);
    data += sizeof(DestType);
    return result;
}

PNGChunk* readChunk(const char*& data) {
    auto chunkDataSize = getFromDataWithSkip<unsigned int>(data);
    std::string chunkTypeString(data, 4);
    data += 4;
    CHUNK_TYPE chunkType = getChunkType(chunkTypeString);
    switch (chunkType) {
        case CHUNK_TYPE_IHDR: {
            auto width = getFromDataWithSkip<unsigned int>(data);
            auto height = getFromDataWithSkip<unsigned int>(data);
            auto bitDepth = getFromDataWithSkip<unsigned char>(data);
            auto colorType = getFromDataWithSkip<unsigned char>(data);
            auto compressionMethod = getFromDataWithSkip<unsigned char>(data);

            auto filterMethod = getFromDataWithSkip<unsigned char>(data);
            auto interlaceMethod = getFromDataWithSkip<unsigned char>(data);
            auto crc = getFromDataWithSkip<unsigned int>(data);

            auto* pngChunkIHDR = new PNGChunkIHDR(chunkDataSize, crc, width, height, bitDepth,
                                                  colorType, compressionMethod, filterMethod, interlaceMethod);
            return pngChunkIHDR;
        }
        case CHUNK_TYPE_PLTE: {
            std::vector<rgb> palette;
            for (int i = 0; i < chunkDataSize / 3; ++i) {

                rgb rgb_;
                for (int c = 0; c < 3; ++c) {
                    rgb_.components[c] = getFromDataWithSkip<unsigned char>(data);
                }
                palette.push_back(rgb_);
            }
            auto crc = getFromDataWithSkip<unsigned int>(data);
            auto* pngChunkPTLE = new PNGChunkPLTE(chunkDataSize, crc, palette);

            return pngChunkPTLE;
        }
        case CHUNK_TYPE_IDAT: {
            unsigned char* imgData = new unsigned char[chunkDataSize];
            for (int i = 0; i < chunkDataSize; ++i) {
                imgData[i] = getFromDataWithSkip<unsigned char>(data);
            }

            auto crc = getFromDataWithSkip<unsigned int>(data);
            auto* pngChunkIDAT = new PNGChunkIDAT(chunkDataSize, crc, imgData);

            return pngChunkIDAT;
        }
        case CHUNK_TYPE_IEND: {
            auto crc = getFromDataWithSkip<unsigned int>(data);
            auto* pngChunkIEND = new PNGChunkIEND(chunkDataSize, crc);

            return pngChunkIEND;
        }
        case CHUNK_TYPE_GAMMA: {
            auto gamma = getFromDataWithSkip<unsigned int>(data);
            auto crc = getFromDataWithSkip<unsigned int>(data);

            auto* pngChunkGamma = new PNGChunkGAMMA(chunkDataSize, crc, gamma);

            return pngChunkGamma;
        }
        case CHUNK_TYPE_UNKNOWN: {
            char* imgData = new char[chunkDataSize];
            for (int i = 0; i < chunkDataSize; ++i) {
                imgData[i] = getFromDataWithSkip<char>(data);
            }

            auto crc = getFromDataWithSkip<unsigned int>(data);
            auto* pngChunkUnknown = new PNGChunkUNKNOWN(chunkDataSize, crc, imgData);

            return pngChunkUnknown;
        }

    }
}

std::vector<PNGChunk*> readPngChunks(const char* data) {
    std::vector<PNGChunk*> chunks;
    PNGChunk* chunk = readChunk(data);
    chunks.push_back(chunk);
    while (chunk->getChunkType() != CHUNK_TYPE_IEND) {
        chunk = readChunk(data);
        chunks.push_back(chunk);
    }

    return chunks;
}

PNGImage png::readPNGImageFromMemory(const char* data, unsigned int length) {
    std::cout << "IT WORKS!!!\n";
    if (!checkSignature((unsigned char*) data)) {
        throw std::runtime_error("Wrong signature");
    }

    std::vector<PNGChunk*> chunks = readPngChunks(data + 8);
    PNGImage pngImage(chunks);

    return pngImage;
}
