#include <Windows.h>

#include <iostream>
#include <string>
#include <vector>
#include <ostream>
#include <tchar.h>

using namespace std;

struct locale_info {
    wstring name;
    DWORD flags;
};
vector<locale_info> locales;

BOOL CALLBACK MyFuncLocaleEx(const LPWSTR localeName, DWORD flags, LPARAM lparam) {
    locales.push_back({localeName, flags});
    return TRUE;
}

int _tmain(int argc, _TCHAR* argv[]) {
    EnumSystemLocalesEx(MyFuncLocaleEx, LOCALE_ALL, NULL, nullptr);

    int index = 0;
    for (const auto& localeInfo: locales) {
        wcout << ++index << ": " << localeInfo.name << " \t" << localeInfo.flags << endl;
    }

    wcout << "Total " << locales.size() << " locales found." << endl;

    return 0;
}