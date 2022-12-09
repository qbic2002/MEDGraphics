//
// Created by golov on 08.12.2022.
//

#include "img/PNGImage.h"
#include "utils/zlib_utils.h"

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

PNGChunkPLTE::PNGChunkPLTE(unsigned int chunkDataSize_, unsigned int crc_, const std::vector<rgb>& palette_)
        : PNGChunk(chunkDataSize_, crc_),
          palette(palette_) {}

CHUNK_TYPE PNGChunkPLTE::getChunkType() {
    return CHUNK_TYPE_PLTE;
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

PNGChunkIEND::PNGChunkIEND(unsigned int chunkDataSize_, unsigned int crc_) : PNGChunk(chunkDataSize_, crc_) {}

CHUNK_TYPE PNGChunkIEND::getChunkType() {
    return CHUNK_TYPE_IEND;
}

PNGImage::PNGImage(const std::vector<PNGChunk*>& pngChunks_) : pngChunks(pngChunks_) {
    setModernRaster();
}

PNGImage::~PNGImage() {
    for (auto& item: pngChunks) {
        delete item;
    }
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

    size_t size = 0;
    std::vector<unsigned char> compressed;
    for (const auto& chunk: pngChunks) {
        if (chunk->getChunkType() == CHUNK_TYPE_IDAT) {
            auto* pngChunkIdat = (PNGChunkIDAT*) chunk;
            for (int i = 0; i < pngChunkIdat->chunkDataSize; ++i) {
                compressed.push_back(pngChunkIdat->data[i]);
            }
            size += pngChunkIdat->chunkDataSize;
        }
    }

    switch (pngChunkIhdr->colorType) {
        case 0:
            colorModel = COLOR_MODEL_GRAY;
            break;
        case 2:
            colorModel = COLOR_MODEL_RGB;
            break;
        case 3:
            colorModel = COLOR_MODEL_RGB;
            break;
        case 6:
            colorModel = COLOR_MODEL_RGBA;
            break;

    }
    int componentsCount = findColorModel(colorModel)->getComponentsCount();
    std::vector<unsigned char> decompressed = zlib::inflate(compressed.data(), size);
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
        int i = 0;
        for (int h = 0; h < height; ++h) {
            ++i;
            for (int w = 0; w < width; ++w) {
                for (int c = 0; c < componentsCount; ++c) {
                    raster_data[(width * componentsCount) * h + (w * componentsCount) + c] = decompressed[i++] / 255.0;
                    if (c == 3)
                        raster_data[(width * componentsCount) * h + (w * componentsCount) + c] = 1;
                }
            }
        }
    }


    modernRaster = ModernRaster(width, height, raster_data, colorModel);

}

const ModernRaster& PNGImage::getModernRaster() const {
    return modernRaster;
}

rgb::rgb() {}
