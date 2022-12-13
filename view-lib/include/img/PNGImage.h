//
// Created by golov on 08.12.2022.
//

#ifndef MEDGRAPHICS_PNG_IMAGE_H
#define MEDGRAPHICS_PNG_IMAGE_H


#include "ModernColorModel.h"

struct rgb {
    rgb();

    union {
        struct {
            unsigned char r = 0, g = 0, b = 0;
        };
        unsigned char components[3];
    };
};

enum CHUNK_TYPE {
    CHUNK_TYPE_IHDR = 0,
    CHUNK_TYPE_PLTE = 1,
    CHUNK_TYPE_GAMMA = 2,
    CHUNK_TYPE_IDAT = 3,
    CHUNK_TYPE_IEND = 4,
    CHUNK_TYPE_UNKNOWN = 5
};

class PNGChunk {
public:
    PNGChunk(unsigned int chunkDataSize_, unsigned int crc_);

    PNGChunk() = default;

    virtual CHUNK_TYPE getChunkType() = 0;

    virtual std::vector<unsigned char> getData() = 0;

    virtual ~PNGChunk() = default;

    unsigned int chunkDataSize;
    unsigned int crc;
};

class PNGChunkIHDR : public PNGChunk {
public:
    PNGChunkIHDR(unsigned int chunkDataSize_, unsigned int crc_, unsigned int width_, unsigned int height_,
                 unsigned char bitDepth_, unsigned char colorType_, unsigned char compressionMethod,
                 unsigned char filterMethod_, unsigned char interlaceMethod_);

    PNGChunkIHDR(unsigned int width_, unsigned int height_,
                 unsigned char bitDepth_, unsigned char colorType_, unsigned char compressionMethod,
                 unsigned char filterMethod_, unsigned char interlaceMethod_);

    CHUNK_TYPE getChunkType() override;

    std::vector<unsigned char> getData() override;

    unsigned int width;
    unsigned int height;
    unsigned char bitDepth;
    unsigned char colorType;
    unsigned char compressionMethod;
    unsigned char filterMethod;
    unsigned char interlaceMethod;
};

class PNGChunkPLTE : public PNGChunk {
public:
    PNGChunkPLTE(unsigned int chunkDataSize_, unsigned int crc_, const std::vector<rgb>& palette_);

    PNGChunkPLTE(const std::vector<rgb>& palette_);

    CHUNK_TYPE getChunkType() override;

    std::vector<unsigned char> getData() override;

    std::vector<rgb> palette;
};

class PNGChunkIDAT : public PNGChunk {
public:
    PNGChunkIDAT(unsigned int chunkDataSize_, unsigned int crc_, unsigned char* data_);

    PNGChunkIDAT(unsigned int size, unsigned char* data_);

    CHUNK_TYPE getChunkType() override;

    std::vector<unsigned char> getData() override;

    unsigned char* data;

    ~PNGChunkIDAT() override;
};

class PNGChunkIEND : public PNGChunk {
public:
    PNGChunkIEND(unsigned int chunkDataSize_, unsigned int crc_);

    PNGChunkIEND();

    std::vector<unsigned char> getData() override;

    CHUNK_TYPE getChunkType() override;
};

class PNGChunkGAMMA : public PNGChunk {
public:
    PNGChunkGAMMA(unsigned int chunkDataSize_, unsigned int crc_, unsigned int gamma_);

    explicit PNGChunkGAMMA(unsigned int gamma_);

    CHUNK_TYPE getChunkType() override;

    std::vector<unsigned char> getData() override;

    unsigned int gamma;
};

class PNGChunkUNKNOWN : public PNGChunk {
public:
    PNGChunkUNKNOWN(unsigned int chunkDataSize_, unsigned int crc_, char* data_);

    CHUNK_TYPE getChunkType() override;

    std::vector<unsigned char> getData() override;

    char* data;

    ~PNGChunkUNKNOWN() override;
};

class PNGImage {
public:
    PNGImage(const std::vector<PNGChunk*>& pngChunks_);

    PNGImage(const ModernRaster& modernRaster_);

    PNGImage(const PNGImage& pngImage) = default;

    ~PNGImage();

    PNGChunk* findChunkByType(CHUNK_TYPE chunkType);

    const std::vector<PNGChunk*>& getPngChunks() const;

    const ModernRaster& getModernRaster() const;

private:
    std::vector<PNGChunk*> pngChunks;
    ModernRaster modernRaster;

    void setModernRaster(ColorModelEnum colorModel = COLOR_MODEL_RGBA);
};


#endif //MEDGRAPHICS_PNG_IMAGE_H
