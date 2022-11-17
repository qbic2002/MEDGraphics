//
// Created by danil on 15.10.2022.
//

#ifndef MEDGRAPHICS_ENCODING_H
#define MEDGRAPHICS_ENCODING_H

#include <iostream>
#include <string>
#include <vector>

namespace utils {
    void configureUtf8();

    std::vector<std::wstring> readArgs(int argc, char** argv);
}

std::ostream& operator<<(std::ostream& os, const wchar_t* wstr);

std::ostream& operator<<(std::ostream& os, const std::wstring& wstr);

#endif //MEDGRAPHICS_ENCODING_H
