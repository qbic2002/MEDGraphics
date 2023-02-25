//
// Created by danil on 16.09.2022.
//

#ifndef MEDGRAPHICS_FILE_H
#define MEDGRAPHICS_FILE_H

#include <fstream>
#include <filesystem>

namespace utils {
    std::ofstream
    openFileOStream(const std::filesystem::path& file, const std::ios_base::openmode& mode = std::ios::binary);

    std::ifstream openFileIStream(const std::filesystem::path& file,
                                  const std::ios_base::openmode& mode = std::ios::binary);

    std::string readAllText(const std::filesystem::path& file);

    std::vector<char> readAllBytes(const std::filesystem::path& file);

    /// @return nullptr if file is less than n, bytes array otherwise (delete is on caller responsibility)
    unsigned char* readNBytes(const std::filesystem::path& file, int n);
}

#endif //MEDGRAPHICS_FILE_H
