#include <string>
#include <iostream>
#include "../src/utils/encoding.h"

using namespace std;

template<typename char_t>
void showString(const char_t* str) {
    while (*str)
        cout << (int) *str++ << " ";
    cout << endl;
}

template<typename char_t>
void showString(const basic_string<char_t>& str) {
    showString(str.c_str());
}

int main(int argc, char** args) {
    utils::configureUtf8();

    cout << "Санкт-Петербург in cout" << endl;
    cout << L"Санкт-Петербург in WCHAR in cout" << endl;
    /// wcout prints nothing on my machine
    /// Danilaa56 21.10.22
    wcout << "Санкт-Петербург in wcout" << endl;
    wcout << L"Санкт-Петербург WCHAR in wcout" << endl;
    wcout << L"Nice localization WCHAR in wcout" << endl;

    cout << "string: \t";
    showString("Санкт-Петербург");
    cout << "wstring: \t";
    showString(L"Санкт-Петербург");
    cout << "u8string: \t";
    showString(u8"Санкт-Петербург");
    cout << "u16string: \t";
    showString(u"Санкт-Петербург");
    cout << "u32string: \t";
    showString(U"Санкт-Петербург");
}