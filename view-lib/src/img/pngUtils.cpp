//
// Created by golov on 08.12.2022.
//

#include "img/pngUtils.h"
#include "utils/logging.h"

const char pngSignature[8] = {(char) 137, 80, 78, 71, 13, 10, 26, 10};

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

void assertSignature(const char* data) {
    for (int i = 0; i < 8; ++i) {
        if (pngSignature[i] != data[i]) {
            throw std::runtime_error("Wrong PNG signature");
        }
    }
}

template<class DestType>
DestType read(const char*& data) {
    char d[sizeof(DestType)];
    for (int i = 0; i < sizeof(DestType); ++i) {
        d[sizeof(DestType) - 1 - i] = *data++;
    }
    return *((DestType*) d);
}

PNGChunk* readChunk(const char*& data) {
    auto chunkDataSize = read<unsigned int>(data);
    std::string chunkTypeString(data, 4);
    data += 4;
    CHUNK_TYPE chunkType = getChunkType(chunkTypeString);
    switch (chunkType) {
        case CHUNK_TYPE_IHDR: {
            auto width = read<unsigned int>(data);
            auto height = read<unsigned int>(data);
            auto bitDepth = read<unsigned char>(data);
            auto colorType = read<unsigned char>(data);
            auto compressionMethod = read<unsigned char>(data);

            auto filterMethod = read<unsigned char>(data);
            auto interlaceMethod = read<unsigned char>(data);
            auto crc = read<unsigned int>(data);

            auto* pngChunkIHDR = new PNGChunkIHDR(chunkDataSize, crc, width, height, bitDepth,
                                                  colorType, compressionMethod, filterMethod, interlaceMethod);
            return pngChunkIHDR;
        }
        case CHUNK_TYPE_PLTE: {
            std::vector<rgb> palette;
            for (int i = 0; i < chunkDataSize / 3; ++i) {
                rgb rgb_;
                for (unsigned char& component: rgb_.components) {
                    component = read<unsigned char>(data);
                }
                palette.push_back(rgb_);
            }
            auto crc = read<unsigned int>(data);
            auto* pngChunkPTLE = new PNGChunkPLTE(chunkDataSize, crc, palette);

            return pngChunkPTLE;
        }
        case CHUNK_TYPE_IDAT: {
            auto* imgData = new unsigned char[chunkDataSize];
            for (int i = 0; i < chunkDataSize; ++i) {
                imgData[i] = read<unsigned char>(data);
            }

            auto crc = read<unsigned int>(data);
            auto* pngChunkIDAT = new PNGChunkIDAT(chunkDataSize, crc, imgData);

            return pngChunkIDAT;
        }
        case CHUNK_TYPE_IEND: {
            auto crc = read<unsigned int>(data);
            auto* pngChunkIEND = new PNGChunkIEND(chunkDataSize, crc);

            return pngChunkIEND;
        }
        case CHUNK_TYPE_GAMMA: {
            auto gamma = read<unsigned int>(data);
            auto crc = read<unsigned int>(data);

            auto* pngChunkGamma = new PNGChunkGAMMA(chunkDataSize, crc, gamma);

            return pngChunkGamma;
        }
        case CHUNK_TYPE_UNKNOWN: {
            char* imgData = new char[chunkDataSize];
            for (int i = 0; i < chunkDataSize; ++i) {
                imgData[i] = read<char>(data);
            }

            auto crc = read<unsigned int>(data);
            auto* pngChunkUnknown = new PNGChunkUNKNOWN(chunkDataSize, crc, imgData);

            return pngChunkUnknown;
        }
    }
}

std::vector<PNGChunk*> readPngChunks(const char* data) {
    std::vector<PNGChunk*> chunks;
    PNGChunk* chunk;
    do {
        chunk = readChunk(data);
        chunks.push_back(chunk);
    } while (chunk->getChunkType() != CHUNK_TYPE_IEND);

    return chunks;
}

PNGImage png::readPNGImageFromMemory(const char* data, unsigned int length) {
    assertSignature(data);

    std::vector<PNGChunk*> chunks = readPngChunks(data + 8);
    PNGImage pngImage(chunks);

    return pngImage;
}
