//
// Created by danil on 21.10.2022.
//

#include "../view-lib/include/utils/encoding.h"
#include "../view-lib/include/utils/file.h"

int main() {
    utils::configureUtf8();
    auto fos = utils::openFileOStream(std::wstring(L"тест.txt"), std::ios::out);
    fos << "hello" << std::endl;
    fos.flush();
    return 0;
}