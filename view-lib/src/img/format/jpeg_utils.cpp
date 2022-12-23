//
// Created by danil on 16.12.2022.
//

#include "img/format/jpeg_utils.h"
#include "utils/logging.h"
#include "../../../../src/core/R.h"
#include <iostream>
#include <iomanip>
#include <bitset>

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
    typedef unsigned short ushort;

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
        stream << "x" << std::setfill('0') << std::setw(sizeof(T) * 2) << std::hex << i;
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

        ~JpegReader() {
            for (int i = 0; i < 8; i++) {
                delete huffTables[i / 4][i % 4];
            }
        }

        ModernRaster* readJpeg() {
            assertSignature();
            while (readChunk()) {}
            auto* raster = new ModernRaster(x, y, std::shared_ptr<float[]>(data),
                                            components.size() == 3 ? COLOR_MODEL_YCbCr601 : COLOR_MODEL_GRAY);
            data = nullptr;
            return raster;
        }

        struct Component {
            uchar id = 0;
            uchar horSamplingFactor = 0;
            uchar vertSamplingFactor = 0;
            uchar quantTable = 0;
            uchar dcTable = 0;
            uchar acTable = 0;
        };

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

        inline void readByteHalves(uchar& dst1, uchar& dst2) {
            dst1 = readByte();
            dst2 = dst1 & 0x0f;
            dst1 >>= 4;
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
                case 0xDB:
                    readChunkDQT();
                    return true;
                case 0xC0:
                    readChunkBaselineDCT();
                    return true;
                case 0xC4:
                    readChunkHuffmanTableSpec();
                    return true;
                case 0xDA:
                    readChunkStartOfScan();
                    return true;
                case 0xD9:
                    return false;
                case 0xE1:  // Application
                case 0xE2:
                case 0xE3:
                case 0xE4:
                case 0xE5:
                case 0xE6:
                case 0xE7:
                case 0xE8:
                case 0xE9:
                case 0xEA:
                case 0xEB:
                case 0xEC:
                case 0xED:
                case 0xEE:
                case 0xEF:
                case 0xFE:  // Comment
                    auto length = readLength();
                    for (int i = 2; i < length; i++) {
                        readByte();
                    }
                    return true;
            }

            info() << "read chunk marker: " << std::hex << (int) ffByte << " " << (int) markerByte << std::dec;
            throw std::runtime_error("Unexpected chunk marker: " + int_to_hex(ffByte));
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

        const char normToZigZag[64] = {0, 1, 5, 6, 14, 15, 27, 28,
                                       2, 4, 7, 13, 16, 26, 29, 42,
                                       3, 8, 12, 17, 25, 30, 41, 43,
                                       9, 11, 18, 24, 31, 40, 44, 53,
                                       10, 19, 23, 32, 39, 45, 52, 54,
                                       20, 22, 33, 38, 46, 51, 55, 60,
                                       21, 34, 37, 47, 50, 56, 59, 61,
                                       35, 36, 48, 49, 57, 58, 62, 63};

        short quantizationTable[4][64];

        void readChunkDQT() {
            auto length = readLength();
            uchar pq, tq;
            readByteHalves(pq, tq);
            if (tq > 3)
                throw std::runtime_error("Unexpected DQT tq value: " + std::to_string(tq) + ", expected: 0-3");
            if (pq == 0) {
                for (short& quantizationTableElement: quantizationTable[tq])
                    quantizationTableElement = readByte();
            } else if (pq == 1) {
                for (short& quantizationTableElement: quantizationTable[tq])
                    quantizationTableElement = readShort();
            } else {
                throw std::runtime_error("Unexpected DQT pq value: " + std::to_string(pq));
            }
            {
                auto& os = info() << "DQT - " << "length: " << length << ", pq: " << (int) pq << ", tq: " << (int) tq;
                for (int i = 0; i < 64; i++) {
                    os << quantizationTable[tq][normToZigZag[i]] << '\t';
                    if (i % 8 == 7)
                        os << '\n';
                }
            }
        }

        void readChunkBaselineDCT() {
            auto length = readLength();
            auto p = readByte();    // Sample precision
            y = readShort();   // Number of lines
            x = readShort();   // Number of samples per line
            nf = readByte();   // Number of image components in frame
            delete data;
            data = new float[x * y * nf];
            maxHorSamplingFactor = maxVertSamplingFactor = 0;
            components.resize(nf);
            {
                auto& os = info() << "BaselineDCT - " << "length: " << length << ", sample precision: " << (int) p
                                  << ", number of lines: " << (int) y << ", number of samples per lines: " << (int) x
                                  << ", number of image components in frame: " << (int) nf;
                os << std::hex;
                for (int i = 0; i < nf; i++) {
                    components[i].id = readByte();
                    readByteHalves(components[i].horSamplingFactor, components[i].vertSamplingFactor);
                    if (maxHorSamplingFactor < components[i].horSamplingFactor) {
                        maxHorSamplingFactor = components[i].horSamplingFactor;
                    }
                    if (maxVertSamplingFactor < components[i].vertSamplingFactor) {
                        maxVertSamplingFactor = components[i].vertSamplingFactor;
                    }
                    components[i].quantTable = readByte();
                    os << components[i] << '\n';
                }
                os << std::dec;
            }
        }

        static void generateSizeTable(const uchar* bits, uchar* huffSize, uchar& lastK) {
            uchar k = 0;
            int i = 1;
            int j = 1;
            do {
                while (j <= bits[i - 1]) {
                    huffSize[k] = i;
                    k += 1;
                    j += 1;
                }
                i += 1;
                j = 1;
            } while (i <= 16);
            huffSize[k] = 0;
            lastK = k;
        }

        static void generateCodeTable(const uchar* huffSize, ushort* huffCode) {
            int k = 0;
            int code = 0;
            int sl = huffSize[0];

            while (true) {
                do {
                    huffCode[k] = code;
                    code += 1;
                    k += 1;
                } while (huffSize[k] == sl);

                if (huffSize[k] == 0)
                    return;

                do {
                    code <<= 1;
                    sl += 1;
                } while (huffSize[k] != sl);
            }
        }

        class JpegBitReader {
        public:
            explicit JpegBitReader(std::istream& is) : is(is) {}

            uchar readBit() {
                if (bufferedBitsCount > 0) {
                    bufferedBitsCount--;
                    return (bufferedByte >> bufferedBitsCount) & 1;
                } else {
                    is.read((char*) &bufferedByte, 1);
//                    info() << "readByte: x" << std::hex << (int) bufferedByte << std::dec;
                    if (bufferedByte == 0xFF) {
                        uchar nextByte;
                        is.read((char*) &nextByte, 1);
                        if (nextByte != 0x00) {
                            throw std::runtime_error("Unexpected marker in image data: " +
                                                     std::to_string(nextByte)); // NOLINT(hicpp-exception-baseclass)
                        }
                    }
                    bufferedBitsCount = 7;
                    return (bufferedByte >> 7) & 1;
                }
            }

        private:
            std::istream& is;
            uchar bufferedByte = 0;
            uchar bufferedBitsCount = 0;
        };

        class HuffTable {
        public:
            HuffTable(const uchar* huffLengths, const uchar* huffVal) {
                generateSizeTable(huffLengths, huffSizes, lastK);
                generateCodeTable(huffSizes, huffCodes);
                int index = 0;
                for (int i = 0; i < 16; i++) {
//                    valPtr[i] = index;
//                    int smallestCode = -1;
//                    int largestCode = -1;
//                    for (int j = 0; j < huffLengths[i]; j++) {
//                        if (huffCodes[index] < smallestCode || smallestCode == -1) {
//                            smallestCode = huffCodes[index];
//                        }
//                        if (largestCode < huffCodes[index] || largestCode == -1) {
//                            largestCode = huffCodes[index];
//                        }
//                        index++;
//                    }
//                    minCode[i] = smallestCode;
//                    maxCode[i] = largestCode;
                    index += huffLengths[i];
                }
                this->huffVal = new uchar[index];
                for (int i = 0; i < index; i++) {
                    this->huffVal[i] = huffVal[i];
                }
                decoderTables(huffLengths);
            }

            HuffTable(const HuffTable& other) = delete;

            HuffTable(HuffTable&& other) = delete;

            ~HuffTable() {
                delete huffVal;
            }

            void decoderTables(const uchar* huffLengths) {
                int i = 0;
                int j = 0;
                while (true) {
                    while (true) {
                        i += 1;
                        if (i > 16)
                            return;
                        if (huffLengths[i - 1] == 0) {
                            maxCode[i - 1] = -1;
                            continue;
                        }
                        break;
                    }
                    valPtr[i - 1] = j;
                    minCode[i - 1] = huffCodes[j];
                    j += huffLengths[i - 1] - 1;
                    maxCode[i - 1] = huffCodes[j];
                    j += 1;
                }
            }

            uchar decode(JpegBitReader& bitReader) {
                int i = 1;
                short code = bitReader.readBit();
                while (code > maxCode[i - 1]) {
                    i += 1;
                    code = (code << 1) | bitReader.readBit();
                }
                int j = valPtr[i - 1];
                j += code - minCode[i - 1];
//                info() << "Code of length " << i << ": " << std::bitset<16>(code) << ", value: x" << std::hex
//                       << (int) huffVal[j] << std::dec << ", value code: " << std::bitset<16>(huffCodes[j]);
//                std::cout << "Huffman read byte: " << (int) huffVal[j] << '\n';
                return huffVal[j];
            }

            uchar huffSizes[256]{};
            ushort huffCodes[256]{};
            uchar* huffVal = nullptr;
            uchar lastK = 0;
            short minCode[16]{};
            short maxCode[16]{};
            int valPtr[16]{};
        };

        HuffTable* huffTables[2][4] = {{nullptr, nullptr, nullptr, nullptr},
                                       {nullptr, nullptr, nullptr, nullptr}};

        void readChunkHuffmanTableSpec() {
            auto length = readLength();
            uchar tc, th;
            readByteHalves(tc, th);

            uchar lengths[16];                      // Number of Huffman codes of length i
            int huffmanCodesCount = 0;
            for (auto& i: lengths) {
                i = readByte();
                huffmanCodesCount += i;
            }
            uchar huffVal[huffmanCodesCount];       // Values associated with each Huffman code
            for (int i = 0; i < huffmanCodesCount; i++) {
                huffVal[i] = readByte();
            }

            delete huffTables[tc][th];
            huffTables[tc][th] = new HuffTable(lengths, huffVal);

            {
                auto& os = info() << "Huffman Table Spec - " << "length: " << length << ", table class: " << (int) tc
                                  << ", table id.: " << (int) th;
                int valueIndex = 0;
                for (int i = 0; i < 16; i++) {
                    os << "codes of length " << i + 1 << " - " << (int) lengths[i] << ": [";
                    os << std::hex;
                    for (int j = 0; j < lengths[i]; j++) {
                        os << (int) huffTables[tc][th]->huffVal[valueIndex++] << '\t';
                    }
                    os << std::dec << ']' << '\n';
                }
            }
        }

        uchar ns = 0;                   // Number of components in scan
        std::unique_ptr<uchar[]> cs;    // Scan component selector

        void readChunkStartOfScan() {
            auto length = readLength();
            ns = readByte();   // Number of components in scan
            cs.reset(new uchar[ns]);
            for (int i = 0; i < ns; i++) {
                cs[i] = readByte();
                for (auto& component: components) {
                    if (component.id == cs[i]) {
                        readByteHalves(component.dcTable, component.acTable);
                        break;
                    }
                }
            }
            auto ss = readByte();   // Start of spectral or predictor selection
            auto se = readByte();   // End of spectral selection
            uchar ah, al;   // Successive approx. bit pos. high, Successive approx. bit pos. low or point transform
            readByteHalves(ah, al);
            {
                auto& os = info() << "Start Of Scan - " << "length: " << length
                                  << ", number of components: " << (int) ns
                                  << ", Start of spectral or predictor selection: " << (int) ss
                                  << ", End of spectral selection: " << (int) se
                                  << ", Successive approx. bit pos. high: " << (int) ah
                                  << ", Successive approx. bit pos. low or point transform: " << (int) al;
                for (int i = 0; i < ns; i++) {
                    for (auto& component: components) {
                        if (component.id == cs[i]) {
                            os << "component id.: " << (int) cs[i] << ", dc selector: " << (int) component.dcTable
                               << ", ac selector: " << (int) component.acTable << '\n';
                            break;
                        }
                    }
                }
            }
            scan(ss, se, ah, al);
        }

        static void fdct(const float* syx, float* svu) {
            for (int v = 0; v < 7; v++) {
                for (int u = 0; u < 7; u++) {
                    float value = 0;
                    for (int y = 0; y < 7; y++) {
                        for (int x = 0; x < 7; x++) {
                            value += syx[y * 8 + x] * std::cos(((2 * x + 1) * u * M_PI) / 16) *
                                     std::cos(((2 * y + 1) * v * M_PI) / 16);
                        }
                    }
                    value *= 0.25;
                    if (v == 0)
                        value *= 1 / std::sqrt(2);
                    if (u == 0)
                        value *= 1 / std::sqrt(2);
                    svu[v * 8 + u] = value;
                }
            }
        }

        static void idct(const float* svu, float* syx) {
            static const long double oneBySqrt2 = 1 / std::sqrt((long double) 2);
            for (int y = 0; y < 8; y++) {
                for (int x = 0; x < 8; x++) {
                    long double value = 0;
                    for (int v = 0; v < 8; v++) {
                        for (int u = 0; u < 8; u++) {
                            value += svu[v * 8 + u]
                                     * std::cos((long double) (((2 * x + 1) * u * M_PI) / 16))
                                     * std::cos((long double) (((2 * y + 1) * v * M_PI) / 16))
                                     * (v == 0 ? oneBySqrt2 : 1)
                                     * (u == 0 ? oneBySqrt2 : 1);
                        }
                    }
                    syx[y * 8 + x] = value / 4;
                }
            }
        }

        static int receive(uchar ssss, JpegBitReader& bitReader) {
//            auto& os = info() << "receive " << (int) ssss << " bits";
            int i = 0;
            int v = 0;
            while (i != ssss) {
                i += 1;
                v = (v << 1) | bitReader.readBit();
//                os << (int)(v & 1);
            }
//            os << '\n';
            return v;
        }

        static int extend(int v, uchar t) {
//            int vt = (1 << t);
//            if (v < vt) {
//                vt = (-1 << t) + 1;
//                v = v + vt;
//            }
//            return v;
            if (!(v >> (t - 1))) {
                v += ((-1 << t) + 1);
            }
            return v;
        }

        static short readQValue(HuffTable* huffTable, JpegBitReader& bitReader) {
            uchar ssss = huffTable->decode(bitReader);
            return readQValue(ssss, bitReader);
        }

        static short readQValue(uchar ssss, JpegBitReader& bitReader) {
            int input = receive(ssss, bitReader);
            int v = extend(input, ssss);
            return v;

//            std::cout << "my extend receive length: " << (int) ssss << "\n";
//            if (ssss == 0)
//                return 0;
//            int i = 1;
//            int v = bitReader.readBit();
//            bool isNegative = v == 1;
//            while (i != ssss) {
//                i += 1;
//                v = (v << 1) | bitReader.readBit();
//            }
//            if (isNegative) {
//                v |= (-1 << ssss);
//            }
////            std::cout << "my extend receive: " << v << "\n";
//            return v;
        }

        static void decodeACCoefficients(short* zz, HuffTable* huffTable, JpegBitReader& bitReader) {
            int k = 1;
            for (int i = 1; i < 64; i++) {
                zz[i] = 0;
            }

            while (true) {
                uchar rs = huffTable->decode(bitReader);
                uchar ssss = rs % 16;
                uchar rrrr = (rs >> 4) & 0x0f;
                uchar r = rrrr;
                if (ssss == 0) {
                    if (r == 15) {
                        k += 16;
                        continue;
                    } else {
                        break;
                    }
                } else {
                    k += r;
                    zz[k] = readQValue(ssss, bitReader);
                    if (k == 63)
                        break;
                    else
                        k += 1;
                }
            }
        }

        template<typename TType>
        void unZigZagData(const TType* src, TType* dst) {
            for (int i = 0; i < 64; i++) {
                dst[i] = src[normToZigZag[i]];
            }
        }

        std::vector<Component> components;
        short y = 0;
        short x = 0;
        uchar nf = 0;
        float* data = nullptr;
        uchar maxHorSamplingFactor = 0;
        uchar maxVertSamplingFactor = 0;

        template<typename TType>
        void printMCUArray(const TType* ar) {
            for (int i = 0; i < 64; i++) {
                std::cout << ar[i] << "\t\t\t";
                if (i % 8 == 7)
                    std::cout << '\n';
            }
        }

        int module(int value, int divider) {
            return ((value % divider) + divider) % divider;
        }

        void readMCU(short& predDc, float* samples, const Component& component, JpegBitReader& bitReader) {
//            info() << "readMCU";
            short zz[64]{};
            zz[0] = predDc = predDc + readQValue(huffTables[0][component.dcTable], bitReader);

            decodeACCoefficients(zz, huffTables[1][component.acTable], bitReader);

//            printMCUArray(zz);

            float R[64];
            for (int i = 0; i < 64; i++) {
                R[i] = (float) zz[i] * quantizationTable[component.quantTable][i];
            }
//            printMCUArray(R);
            float RUnZZ[64];
            unZigZagData(R, RUnZZ);

//            printMCUArray(RUnZZ);

            idct(RUnZZ, samples);

            printMCUArray(samples);

            for (int i = 0; i < 64; i++) {
//                samples[i] = std::clamp(samples[i] + 128, 0.0f, 255.0f) / 255.0f;
                samples[i] = module((samples[i] + 128), 256) / 255.0f;
            }

//            printMCUArray(samples);
//            info() << "readMCU finished";
        }

        Component& getComponentById(uchar id) {
            for (auto& component: components) {
                if (component.id == id)
                    return component;
            }
            throw std::runtime_error("Could not find component with identifier: " + std::to_string((int) id));
        }

        void placeMCU(const float* samples, int mcuX, int mcuY, int component, int xScale, int yScale) {
//            HANDLE_SIGSEGV
//            info() << "PlaceMCU " << mcuX << ", " << mcuY << " " << component << ", scale: " << xScale << "x" << yScale;
            for (int j = 0; j < 8; j++) {
                for (int i = 0; i < 8; i++) {
                    float value = samples[j * 8 + i];
                    for (int yy = 0; yy < yScale; yy++) {
                        for (int xx = 0; xx < xScale; xx++) {
                            int _y = ((mcuY * 8 + j) * yScale + yy);
                            int _x = ((mcuX * 8 + i) * xScale + xx);
                            if ((_x < 0) || (x <= _x) || (_y < 0) || (y <= _y))
                                continue;
                            data[(_y * x + _x) * nf + component] = value;
                        }
                    }
                }
            }
//            info() << "PlaceMCU " << mcuX << ", " << mcuY << " " << component << ", scale: " << xScale << "x" << yScale << " finished";
        }

        void scan(uchar ss, uchar se, uchar ah, uchar al) {
            JpegBitReader bitReader(stream);

            short predDc[ns];
            for (int i = 0; i < ns; i++) {
                short pred = 0;
                float samples[64];
                auto& component = getComponentById(cs[i]);

                int horPixelScale = maxHorSamplingFactor / component.horSamplingFactor;
                int vertPixelScale = maxVertSamplingFactor / component.vertSamplingFactor;
                int horMCUCount = x / horPixelScale / 8;
                int vertMCUCount = y / vertPixelScale / 8;
                if (horMCUCount * horPixelScale * 8 < x)
                    horMCUCount += 1;
                if (vertMCUCount * vertPixelScale * 8 < y)
                    vertMCUCount += 1;
                for (int yMCU = 0; yMCU < vertMCUCount; yMCU++) {
                    for (int xMCU = 0; xMCU < horMCUCount; xMCU++) {
                        readMCU(pred, samples, component, bitReader);
                        placeMCU(samples, xMCU, yMCU, component.id - 1, horPixelScale, vertPixelScale);
                    }
                }
            }


//            for (int c = 0; c < ns; c++) {
//                short zz[64]{};
////                uchar t = huffTables[0][td[c]]->decode(bitReader);
////                uchar diff = receive(t, bitReader);
////                diff = extend(diff, t);
//                auto diff = readQValue(huffTables[0][td[c]], bitReader);
//
//                decodeACCoefficients(zz, huffTables[1][ta[c]], bitReader);
//
////                uchar sq[64];
////                unZigZagData(zz, sq);
//
//                float R[64];
//                for (int i = 0; i < 64; i++) {
//                    R[i] = (float) zz[i] * quantizationTable[components[c].quantTable][i];
//                }
//                float RUnZZ[64];
//                unZigZagData(R, RUnZZ);
//
//                float samples[64];
//                idct(RUnZZ, samples);
//
//                for (float& sample: samples)
//                    sample = (uchar) (sample + 128);
//
//                {
//                    auto& os = info() << "Component " << c << " samples:";
//                    for (int j = 0; j < 64; j++) { // NOLINT(modernize-loop-convert)
//                        os << (int) samples[j] << '\t';
//                        if (j % 8 == 7)
//                            os << '\n';
//                    }
//                }
//            }
        }

        std::istream& stream;
    };

    std::ostream& operator<<(std::ostream& os, const JpegReader::Component& component) {
        return os << "Component{id: " << (int) component.id
                  << ", hor. sampling factor: " << (int) component.horSamplingFactor
                  << ", vert. sampling factor: " << (int) component.vertSamplingFactor
                  << ", quant. table: " << (int) component.quantTable
                  << "}";
    }

    ModernRaster* readJpegImage(std::istream& stream) {
        JpegReader jpegReader(stream);
        return jpegReader.readJpeg();
    }

    ModernRaster* readJpegImage(const char* data, int length) {
        mem_stream is((char*) data, length);
        return readJpegImage(is);
    }
}
