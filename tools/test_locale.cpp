#include <iostream>
#include <windows.h>

using namespace std;

#define tryLocale(x) std::cout << #x": " << setlocale(x, nullptr) << endl

void checkLocale() {
    tryLocale(LC_ALL);
    tryLocale(LC_COLLATE);
    tryLocale(LC_CTYPE);
    tryLocale(LC_MONETARY);
    tryLocale(LC_NUMERIC);
    tryLocale(LC_TIME);
    tryLocale(LC_MIN);
    tryLocale(LC_MAX);
}

template<typename char_t>
void showString(const char_t* str) {
    while (*str)
        cout << (int) *(str++) << " ";
    cout << endl;
}

template<typename char_t>
void showString(const basic_string<char_t>& str) {
    for (char_t i: str)
        cout << (int) i << " ";
    cout << endl;
}

int main(int argc, char** argv) {
    checkLocale();
    cout << GetKBCodePage() << endl;
    cout << GetConsoleCP() << endl;
    cout << GetConsoleOutputCP() << endl;
    cout << GetACP() << endl;
    cout << cout.getloc().name() << endl;

    for (int i = 1; i < argc; i++) {
        showString(argv[i]);
    }

    auto clStr = wstring(GetCommandLineW());
    showString(clStr);
    int count;
    auto** args = CommandLineToArgvW(GetCommandLineW(), &count);

    for (int i = 1; i < count; i++) {
        cout << "wstring: ";
        showString(wstring(args[i]));
    }
    return 0;
}