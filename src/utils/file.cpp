//
// Created by danil on 16.09.2022.
//

#include <iostream>
#include <fstream>
#include <winnls.h>
#include <filesystem>
#include "file.h"
#include "R.h"

namespace utils {
    std::wstring toUtf16(const std::string& str) {
        std::wstring res;
        int len = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), nullptr, 0);
        if (len > 0) {
            res.resize(len);
            MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), &res[0], len);
        }
        return res;
    }

    std::ifstream openFileStream(const std::string& fileName, const std::ios_base::openmode& mode) {
        std::ifstream ifs(toUtf16(fileName).c_str(), mode);
        if (!ifs.is_open()) {
            std::cerr << "Could not open file '" << fileName << "'" << std::endl;
            std::wcerr << toUtf16(fileName) << "\n";
            throw std::exception();
        }
        return ifs;
    }

    std::string readAllText(const std::string& fileName) {
        std::ifstream ifs = openFileStream(fileName, std::ios::in);
        std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
        return content;
    }

    std::vector<char> readAllBytes(const std::string& fileName) {
        std::ifstream ifs = openFileStream(fileName, std::ios::binary | std::ios::ate);
        std::ifstream::pos_type length = ifs.tellg();

        std::vector<char> content(length);

        ifs.seekg(0, std::ios::beg);
        ifs.read(&content[0], length);

        return content;
    }

    unsigned char* readNBytes(const std::string& fileName, int n) {
        std::ifstream ifs = openFileStream(fileName, std::ios::binary | std::ios::ate);
        std::ifstream::pos_type length = ifs.tellg();

        if (length < n) return nullptr;

        auto* bytes = new unsigned char[n];
        ifs.seekg(0, std::ios::beg);
        ifs.read((char*) bytes, n);

        return bytes;
    }
}