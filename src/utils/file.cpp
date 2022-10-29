//
// Created by danil on 16.09.2022.
//

#include "file.h"
#include <iostream>

namespace utils {
    std::ofstream openFileOStream(const std::filesystem::path& file, const std::ios_base::openmode& mode) {
        std::ofstream ofs(file, mode);
        if (!ofs.is_open()) {
            std::cerr << "Could not open file " << file << std::endl;
            throw std::exception();
        }
        return ofs;
    }

    std::ifstream openFileIStream(const std::filesystem::path& file, const std::ios_base::openmode& mode) {
        std::ifstream ifs(file, mode);
        if (!ifs.is_open()) {
            std::cerr << "Could not open file " << file << std::endl;
            throw std::exception();
        }
        return ifs;
    }

    std::string readAllText(const std::filesystem::path& file) {
        std::ifstream ifs = openFileIStream(file, std::ios::in);
        std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));
        return content;
    }

    std::vector<char> readAllBytes(const std::filesystem::path& file) {
        std::ifstream ifs = openFileIStream(file, std::ios::binary | std::ios::ate);
        std::ifstream::pos_type length = ifs.tellg();

        std::vector<char> content(length);

        ifs.seekg(0, std::ios::beg);
        ifs.read(&content[0], length);

        return content;
    }

    unsigned char* readNBytes(const std::filesystem::path& file, int n) {
        std::ifstream ifs = openFileIStream(file, std::ios::binary | std::ios::ate);
        std::ifstream::pos_type length = ifs.tellg();

        if (length < n) return nullptr;

        auto* bytes = new unsigned char[n];
        ifs.seekg(0, std::ios::beg);
        ifs.read((char*) bytes, n);

        return bytes;
    }
}
