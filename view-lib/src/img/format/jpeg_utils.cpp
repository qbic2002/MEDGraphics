//
// Created by danil on 16.12.2022.
//

#include "img/format/jpeg_utils.h"
#include "utils/logging.h"
#include <iostream>
#include <iomanip>

namespace img {
    class mem_buffer : public std::basic_streambuf<char> {
    public:
        mem_buffer(char* pointer, int length) {
            setg(pointer, pointer, pointer + length);
        }
    };

    class mem_stream : public std::istream {
    public:
        mem_stream(char* pointer, int length) : std::istream(&_buffer), _buffer(pointer, length) {
            rdbuf(&_buffer);
        }

    private:
        mem_buffer _buffer;
    };

    typedef unsigned char uchar;

    template<typename TValue>
    TValue read(std::istream& istream) {
        char c[sizeof(TValue)];
        for (int i = 0; i < sizeof(TValue); i++) {
            istream.read(&c[sizeof(TValue) - i - 1], 1);
        }
        return *((TValue*) c);
    }

    template<typename T>
    std::string int_to_hex(T i) {
        std::stringstream stream;
        stream << "0x" << std::setfill('0') << std::setw(sizeof(T) * 2) << std::hex << i;
        return stream.str();
    }

    std::string readString(std::istream& stream) {
        std::string str;
        char c = 0;
        stream.read(&c, 1);
        while (c != 0) {
            str.push_back(c);
            stream.read(&c, 1);
        }
        return str;
    }

    class JpegReader {
    public:
        explicit JpegReader(std::istream& stream) : stream(stream) {}

        void readJpeg() {
            assertSignature();
            while (readChunk()) {

            }
        }

    private:
        template<typename TValue>
        TValue read() {
            char c[sizeof(TValue)];
            for (int i = 0; i < sizeof(TValue); i++) {
                stream.read(&c[sizeof(TValue) - i - 1], 1);
            }
            return *((TValue*) c);
        }

        inline uchar readByte() {
            return read<uchar>();
        }

        inline short readShort() {
            return read<short>();
        }

        inline short readLength() {
            return readShort();
        }

        inline std::string readString() {
            return img::readString(stream);
        }

        void assertSignature() {
            auto byte1 = readByte();
            auto byte2 = readByte();
            if ((byte1 != 0xFF) || (byte2 != 0xD8))
                throw std::runtime_error("wrong jpeg signature");
        }

        bool readChunk() {
            auto ffByte = readByte();
            auto markerByte = readByte();

            if (ffByte != 0xFF)
                throw std::runtime_error("0xFF marker start was expected");
            switch (markerByte) {
                case 0xE0:
                    readChunkAPP0();
                    return true;
            }

            return false;
        }

        void readChunkAPP0() {
            auto length = readLength();
            auto identifier = readString();
            auto versionMajor = readByte();
            auto versionMinor = readByte();
            auto densityUnits = readByte();
            auto xDensity = readShort();
            auto yDensity = readShort();
            auto xThumbnail = readByte();
            auto yThumbnail = readByte();
            for (int i = identifier.size() + 1 + 11; i < length; i++)
                readByte();
            info() << "APPO - " << "length: " << length << ", identifier: " << identifier
                   << ", version: " << (int) versionMajor << "." << (int) versionMinor
                   << ", density: " << xDensity << "x" << yDensity
                   << (densityUnits == 0 ? " no units" : densityUnits == 1 ? " pixels per inch" : " pixels per cm")
                   << ", thumbnail size: " << (int) xThumbnail << "x" << (int) yThumbnail;
        }

        std::istream& stream;
    };

    ModernRaster* readJpegImage(std::istream& stream) {
        JpegReader jpegReader(stream);
        jpegReader.readJpeg();
        return nullptr;
    }

    ModernRaster* readJpegImage(const char* data, int length) {
        mem_stream is((char*) data, length);
        return readJpegImage(is);
    }
}
