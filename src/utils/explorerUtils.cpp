//
// Created by golov on 13.10.2022.
//

#include <string>
#include "explorerUtils.h"
#include <codecvt>
#include <locale>

std::string utils::getOpenFileName() {
    OPENFILENAMEA ofn;
    char szFile[100];

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "All\0*.*\0PNM\0*.pnm; *.ppm; *pgm\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    bool status = GetOpenFileNameA(&ofn);
    if (!status) {
        throw std::exception();
    }

//    std::locale::global(std::locale("ru_RU.CP1251"));
    return ofn.lpstrFile;
}

std::string utils::getSaveFileName() {
    OPENFILENAMEA ofn;
    char szFile[100];

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = "All\0*.*\0PNM\0*.pnm; *.ppm; *pgm\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_EXPLORER;

    bool status = GetOpenFileNameA(&ofn);
    if (!status) {
        throw std::exception();
    }

    return ofn.lpstrFile;
}