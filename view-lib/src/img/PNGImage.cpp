//
// Created by golov on 08.12.2022.
//

#include "img/PNGImage.h"
#include "utils/zlib_utils.h"
#include "img/pngUtils.h"

template<class SrcType>
void toUCHAR(SrcType src, unsigned char*& dest) {
//    unsigned char[sizeof(SrcType];
    for (int i = 0; i < sizeof(SrcType); ++i) {
        dest[sizeof(SrcType) - 1 - i] = ((unsigned char*) &src)[i];
    }
    dest += sizeof(SrcType);
}

PNGChunk::PNGChunk(unsigned int chunkDataSize_, unsigned int crc_) : chunkDataSize(chunkDataSize_), crc(crc_) {}


CHUNK_TYPE PNGChunkIHDR::getChunkType() {
    return CHUNK_TYPE_IHDR;
}

PNGChunkIHDR::PNGChunkIHDR(unsigned int chunkDataSize_, unsigned int crc_, unsigned int width_, unsigned int height_,
                           unsigned char bitDepth_, unsigned char colorType_, unsigned char compressionMethod,
                           unsigned char filterMethod_, unsigned char interlaceMethod_)
        : PNGChunk(chunkDataSize_, crc_), width(width_), height(height_), bitDepth(bitDepth_), colorType(colorType_),
          compressionMethod(compressionMethod),
          filterMethod(filterMethod_), interlaceMethod(interlaceMethod_) {}

std::vector<unsigned char> PNGChunkIHDR::getData() {
    unsigned char data[chunkDataSize];
    unsigned char* dataPtr = data;

    toUCHAR(width, dataPtr);
    toUCHAR(height, dataPtr);
    toUCHAR(bitDepth, dataPtr);
    toUCHAR(colorType, dataPtr);
    toUCHAR(compressionMethod, dataPtr);
    toUCHAR(filterMethod, dataPtr);
    toUCHAR(interlaceMethod, dataPtr);

    return std::vector<unsigned char>(data, data + chunkDataSize);
}

PNGChunkIHDR::PNGChunkIHDR(unsigned int width_, unsigned int height_,
                           unsigned char bitDepth_, unsigned char colorType_, unsigned char compressionMethod,
                           unsigned char filterMethod_, unsigned char interlaceMethod_)
        : PNGChunk(13, 0), width(width_), height(height_), bitDepth(bitDepth_), colorType(colorType_),
          compressionMethod(compressionMethod),
          filterMethod(filterMethod_), interlaceMethod(interlaceMethod_) {
}

PNGChunkPLTE::PNGChunkPLTE(unsigned int chunkDataSize_, unsigned int crc_, const std::vector<rgb>& palette_)
        : PNGChunk(chunkDataSize_, crc_),
          palette(palette_) {}

CHUNK_TYPE PNGChunkPLTE::getChunkType() {
    return CHUNK_TYPE_PLTE;
}

std::vector<unsigned char> PNGChunkPLTE::getData() {
    return {(unsigned char*) palette.data(), (unsigned char*) palette.data() + palette.size()};
}

PNGChunkPLTE::PNGChunkPLTE(const std::vector<rgb>& palette_) : PNGChunk(palette_.size() * 3, 0), palette(palette_) {

}

PNGChunkIDAT::PNGChunkIDAT(unsigned int chunkDataSize_, unsigned int crc_, unsigned char* data_) : PNGChunk(
        chunkDataSize_,
        crc_), data(data_) {}

CHUNK_TYPE PNGChunkIDAT::getChunkType() {
    return CHUNK_TYPE_IDAT;
}

PNGChunkIDAT::~PNGChunkIDAT() {
    delete[] data;
}

std::vector<unsigned char> PNGChunkIDAT::getData() {

    auto dData = zlib::deflate({data, data + chunkDataSize});
    chunkDataSize = dData.size();
    return dData;

}

PNGChunkIDAT::PNGChunkIDAT(unsigned int size, unsigned char* data_) : PNGChunk(size, 0), data(data_) {
}

PNGChunkIEND::PNGChunkIEND(unsigned int chunkDataSize_, unsigned int crc_) : PNGChunk(chunkDataSize_, crc_) {}

CHUNK_TYPE PNGChunkIEND::getChunkType() {
    return CHUNK_TYPE_IEND;
}

std::vector<unsigned char> PNGChunkIEND::getData() {
    return std::vector<unsigned char>(0);
}

PNGChunkIEND::PNGChunkIEND() : PNGChunk(0, 0) {}

PNGImage::PNGImage(const std::vector<PNGChunk*>& pngChunks_) : pngChunks(pngChunks_) {
    setModernRaster();
}

PNGImage::~PNGImage() {
    for (auto& item: pngChunks) {
        delete item;
    }
}

PNGChunkGAMMA::PNGChunkGAMMA(unsigned int chunkDataSize_, unsigned int crc_, unsigned int gamma_) : PNGChunk(
        chunkDataSize_, crc_),
                                                                                                    gamma(gamma_) {}

PNGChunkGAMMA::PNGChunkGAMMA(unsigned int gamma_) : PNGChunk(4, 0), gamma(gamma_) {

}

CHUNK_TYPE PNGChunkGAMMA::getChunkType() {
    return CHUNK_TYPE_GAMMA;
}

std::vector<unsigned char> PNGChunkGAMMA::getData() {
    unsigned char data[chunkDataSize];
    unsigned char* dataPtr = data;
    toUCHAR(gamma, dataPtr);
    return {data, data + chunkDataSize};
}

PNGChunkUNKNOWN::PNGChunkUNKNOWN(unsigned int chunkDataSize_, unsigned int crc_, char* data_) : PNGChunk(chunkDataSize_,
                                                                                                         crc_),
                                                                                                data(data_) {}

CHUNK_TYPE PNGChunkUNKNOWN::getChunkType() {
    return CHUNK_TYPE_UNKNOWN;
}

PNGChunkUNKNOWN::~PNGChunkUNKNOWN() {
    delete[] data;
}

std::vector<unsigned char> PNGChunkUNKNOWN::getData() {
    return {data, data + chunkDataSize};
}


PNGChunk* PNGImage::findChunkByType(CHUNK_TYPE chunkType) {
    auto iter = std::find_if(pngChunks.begin(), pngChunks.end(),
                             [&](PNGChunk* chunk) { return chunk->getChunkType() == chunkType; });
    if (iter == pngChunks.end()) {
        return nullptr;
    }

    return *iter;
}

void PNGImage::setModernRaster(ColorModelEnum colorModel) {
    auto* pngChunkIhdr = (PNGChunkIHDR*) findChunkByType(CHUNK_TYPE_IHDR);
    unsigned int width = pngChunkIhdr->width;
    unsigned int height = pngChunkIhdr->height;
    unsigned char interlace = pngChunkIhdr->interlaceMethod;
    if (interlace != 0) {
        throw std::runtime_error("Interlace not supported");
    }
    if (pngChunkIhdr->bitDepth != 8) {
        return;
    }

    std::vector<unsigned char> compressed;
    for (const auto& chunk: pngChunks) {
        if (chunk->getChunkType() == CHUNK_TYPE_IDAT) {
            auto* pngChunkIdat = (PNGChunkIDAT*) chunk;
            for (int i = 0; i < pngChunkIdat->chunkDataSize; ++i) {
                compressed.push_back(pngChunkIdat->data[i]);
            }
        }
    }

    switch (pngChunkIhdr->colorType) {
        case 0:
            colorModel = COLOR_MODEL_GRAY;
            break;
        case 2:
        case 3:
            colorModel = COLOR_MODEL_RGB;
            break;
        case 6:
            colorModel = COLOR_MODEL_RGBA;
            break;
    }
    int componentsCount = findColorModel(colorModel)->getComponentsCount();
    std::vector<unsigned char> decompressed = zlib::inflate(compressed);
    std::shared_ptr<float[]> raster_data(new float[width * height * componentsCount]);
    if (pngChunkIhdr->colorType == 3) {
        auto* pngChunkPlte = (PNGChunkPLTE*) findChunkByType(CHUNK_TYPE_PLTE);
        int i = 0;
        for (int h = 0; h < height; ++h) {
            ++i;
            for (int w = 0; w < width; ++w) {
                for (int c = 0; c < componentsCount; ++c) {
                    raster_data[(width * componentsCount) * h + (w * componentsCount) + c] =
                            pngChunkPlte->palette[decompressed[i]].components[c] / 255.0;
                }
                ++i;
            }
        }
    } else {
        auto raster_data_ptr = raster_data.get();
        auto decompressed_ptr = decompressed.data();

        unsigned char prevLine[width * componentsCount];
        for (auto& v: prevLine)
            v = 0;
        unsigned char prevPixel[componentsCount];
        unsigned char prevUpPixel[componentsCount];

        for (int y = 0; y < height; ++y) {
            auto filter = *decompressed_ptr++;
            for (auto& v: prevPixel)
                v = 0;
            for (auto& v: prevUpPixel)
                v = 0;
            for (int x = 0; x < width; ++x) {
                for (int c = 0; c < componentsCount; ++c) {
                    unsigned char value = *decompressed_ptr++;

                    switch (filter) {
                        case 1: // Sub
                            value += prevPixel[c];
                            break;
                        case 2: // Up
                            value += prevLine[x * componentsCount + c];
                            break;
                        case 3: // Average
                            value += (prevLine[x * componentsCount + c] + prevPixel[c]) / 2;
                            break;
                        case 4: // Paeth
                        {
                            typedef unsigned char uchar;
                            int p = (int) prevPixel[c] + prevLine[x * componentsCount + c] - prevUpPixel[c];
                            int pa = std::abs(p - prevPixel[c]);
                            int pb = std::abs(p - prevLine[x * componentsCount + c]);
                            int pc = std::abs(p - prevUpPixel[c]);
                            if (pa <= pb && pa <= pc)
                                value += prevPixel[c];
                            else if (pb <= pc)
                                value += prevLine[x * componentsCount + c];
                            else
                                value += prevUpPixel[c];
                            break;
                        }
                        case 0:
                        default:
                            break;
                    }

                    *raster_data_ptr++ = value / 255.0;
                    prevUpPixel[c] = prevLine[x * componentsCount + c];
                    prevLine[x * componentsCount + c] = prevPixel[c] = value;
                }
            }
        }
    }

    float gamma = 0;
    auto* pngChunkGamma = (PNGChunkGAMMA*) findChunkByType(CHUNK_TYPE_GAMMA);
    if (pngChunkGamma) {
        gamma = pngChunkGamma->gamma / 100000.0;
    }

    if (gamma == 0) {
        modernRaster = ModernRaster(width, height, raster_data, colorModel, 0);
    } else {
        modernRaster = ModernRaster(width, height, raster_data, colorModel, 1 / gamma);
    }
}

const ModernRaster& PNGImage::getModernRaster() const {
    return modernRaster;
}

PNGImage::PNGImage(const ModernRaster& modernRaster_) : modernRaster(modernRaster_) {
    unsigned int width = modernRaster_.getWidth();
    unsigned int height = modernRaster_.getHeight();
    unsigned char colorType;
    switch (modernRaster_.getColorModel()->getComponentsCount()) {
        case 1:
            colorType = 0;
            break;
        case 3:
            colorType = 2;
            break;
        case 4:
            colorType = 6;
            break;
        default:
            throw std::runtime_error("supports only 1, 3, 4 channels images");
    }
    unsigned char bidDepth = 8;
    unsigned char compressionMethod = 0;
    unsigned char filter = 0;
    unsigned char interlace = 0;
    auto* pngChunkIhdr = new PNGChunkIHDR(width, height, bidDepth, colorType, compressionMethod, filter, interlace);
    pngChunks.push_back(pngChunkIhdr);
    if (modernRaster_.getGamma() != 0) {
        auto* pngChunkGamma = new PNGChunkGAMMA(1 / modernRaster_.getGamma() * 100000);
        pngChunks.push_back(pngChunkGamma);
    }

    size_t size =
            modernRaster_.getWidth() * modernRaster_.getHeight() * modernRaster_.getColorModel()->getComponentsCount() +
            modernRaster_.getHeight();

    auto* data = new unsigned char[size];

    size_t rasterDataPtr = 0;
    for (size_t i = 0; i < size; ++i) {
        if (i % (modernRaster_.getWidth() * modernRaster_.getColorModel()->getComponentsCount() + 1) == 0) {
            data[i] = 0;
        } else {
            data[i] = modernRaster_.getFilter(rasterDataPtr % modernRaster_.getColorModel()->getComponentsCount()) ?
                      modernRaster_.getData()[rasterDataPtr] * 255 : 0;
            rasterDataPtr++;
        }
    }
    auto* pngChunkIdat = new PNGChunkIDAT(size, data);
    pngChunks.push_back(pngChunkIdat);
    auto* pngChunkIend = new PNGChunkIEND();
    pngChunks.push_back(pngChunkIend);
}

const std::vector<PNGChunk*>& PNGImage::getPngChunks() const {
    return pngChunks;
}

rgb::rgb() {}
