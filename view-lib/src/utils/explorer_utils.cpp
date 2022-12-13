//
// Created by golov on 13.10.2022.
//

#include "utils/explorer_utils.h"
#include <windows.h>
#include "utils/logging.h"
#include "utils/SaveFormat.h"
#include <regex>

bool utils::getOpenFileName(std::wstring& filename) {
    OPENFILENAMEW ofn;
    wchar_t szFile[300];

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = L"All\0*.*\0PNM\0*.pnm; *.ppm; *.pgm\0";
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = nullptr;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = nullptr;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    info("GetOpenFileName call");
    bool status = GetOpenFileNameW(&ofn);
    info("GetOpenFileName finished");
    filename = ofn.lpstrFile;
    return status;
}

utils::OpenFileInfo utils::getSaveFileName() {
    OPENFILENAMEW ofn;
    wchar_t szFile[300];

    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = nullptr;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = utils::getSaveFilterString();
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = nullptr;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = nullptr;
    ofn.Flags = OFN_EXPLORER;

    info("GetOpenFileName call");
    bool status = GetOpenFileNameW(&ofn);
    info("GetOpenFileName finished");

    return {.isCancelled = !status, .filename = ofn.lpstrFile, .filterIndex = (int) ofn.nFilterIndex};
}
