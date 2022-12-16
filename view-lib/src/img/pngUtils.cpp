//
// Created by golov on 08.12.2022.
//

#include "img/pngUtils.h"
#include "utils/logging.h"
#include "utils/file.h"

unsigned int CRC32_function(unsigned char* buf, unsigned long len) {
    unsigned long crc_table[256];
    unsigned long crc;
    for (int i = 0; i < 256; i++) {
        crc = i;
        for (int j = 0; j < 8; j++)
            crc = crc & 1 ? (crc >> 1) ^ 0xEDB88320UL : crc >> 1;
        crc_table[i] = crc;
    };
    crc = 0xFFFFFFFFUL;
    while (len--)
        crc = crc_table[(crc ^ *buf++) & 0xFF] ^ (crc >> 8);
    return crc ^ 0xFFFFFFFFUL;
}

unsigned int CRC32_count(char* filename) {
    char buf[4096 * 64];
    std::ifstream f(filename, std::ios::binary);
    f.read(buf, 4096 * 64);
    return CRC32_function((unsigned char*) buf, f.gcount());
}

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

ModernRaster png::readPNGImageFromMemory(const char* data, unsigned int length) {
    assertSignature(data);

    std::vector<PNGChunk*> chunks = readPngChunks(data + 8);
    PNGImage pngImage(chunks);

    return pngImage.getModernRaster();
}

std::string getChunkName(CHUNK_TYPE chunkType) {
    switch (chunkType) {
        case CHUNK_TYPE_IHDR:
            return "IHDR";
        case CHUNK_TYPE_PLTE:
            return "PLTE";
        case CHUNK_TYPE_GAMMA:
            return "gAMA";
        case CHUNK_TYPE_IDAT:
            return "IDAT";
        case CHUNK_TYPE_IEND:
            return "IEND";
        case CHUNK_TYPE_UNKNOWN:
            throw std::runtime_error("Unknown chunk");
    }
}

void png::writePNGImage(const ModernRaster& modernRaster, const std::filesystem::path& filename) {
    PNGImage pngImage(modernRaster);
    std::vector<unsigned char> resultData;

    resultData.insert(resultData.end(), pngSignature, pngSignature + 8);
    for (auto& chunk: pngImage.getPngChunks()) {
        auto data = chunk->getData();

        for (int i = 0; i < 4; ++i) {
            resultData.push_back(((unsigned char*) &chunk->chunkDataSize)[4 - 1 - i]);
        }

        std::string name = getChunkName(chunk->getChunkType());

        for (const auto& item: name) {
            resultData.push_back(item);
        }

        resultData.insert(resultData.end(), data.begin(), data.end());

        chunk->crc = CRC32_function((unsigned char*) resultData.data() + resultData.size() - data.size() - 4,
                                    data.size() + 4);
        for (int i = 0; i < 4; ++i) {
            resultData.push_back(((unsigned char*) &chunk->crc)[4 - 1 - i]);
        }

    }
    std::ofstream fileStream = utils::openFileOStream(filename);
    fileStream.write((char*) resultData.data(), resultData.size());
    fileStream.flush();
}

