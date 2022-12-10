//
// Created by golov on 09.12.2022.
//

#include <zlib.h>
#include <stdexcept>
#include "utils/zlib_utils.h"

#define CHUNK 16384

std::vector<unsigned char> zlib::inflate(const std::vector<unsigned char>& data) {
    z_stream dStream;

    dStream.zalloc = Z_NULL;
    dStream.zfree = Z_NULL;
    dStream.opaque = Z_NULL;
    dStream.avail_in = 0;
    dStream.next_in = Z_NULL;

    std::vector<unsigned char> result;

    int ret = inflateInit(&dStream);
    if (ret != Z_OK) {
        throw std::runtime_error("inflate error");
    }

    dStream.avail_in = data.size();
    dStream.next_in = (Bytef*) data.data();

    while (dStream.avail_in != 0) {
        unsigned char buff[CHUNK];
//        buff[CHUNK] = '\0';
        dStream.next_out = buff;
        dStream.avail_out = CHUNK;

        ret = inflate(&dStream, Z_NO_FLUSH);
        if (ret == Z_STREAM_END) {
            size_t len = CHUNK - dStream.avail_out;
            for (int i = 0; i < len; ++i) {
                result.push_back(buff[i]);
            }
            break;
        }

        if (ret != Z_OK) {
            inflateEnd(&dStream);
            throw std::runtime_error("inflate error");
        }

        for (unsigned char& i: buff) {
            result.push_back(i);
        }
    }

    inflateEnd(&dStream);
    return result;
}
