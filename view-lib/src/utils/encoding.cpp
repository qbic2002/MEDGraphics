//
// Created by danil on 15.10.2022.
//

#include <utils/encoding.h>
#include <codecvt>
#include <locale>
#include <windows.h>

namespace utils {
    void configureUtf8() {
        SetConsoleOutputCP(CP_UTF8);
    }

    std::vector<std::wstring> readArgs() {
        int count;
        /**
         * GetCommandLineW - return value is managed by system
         * CommandLineToArgvW - application must free the memory used by the argument list with LocalFree()
         * */
        auto** argsChars = CommandLineToArgvW(GetCommandLineW(), &count);
        std::vector<std::wstring> args;
        args.reserve(count);
        for (int i = 0; i < count; i++) {
            args.emplace_back(argsChars[i]);
        }
        LocalFree(argsChars);
        return args;
    }

    std::vector<std::wstring> readArgs(int argc, char** argv) {
        return readArgs();
    }
}

std::ostream& operator<<(std::ostream& os, const wchar_t* wstr) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>, wchar_t> convert;
    std::string dest = convert.to_bytes(wstr);
    return os << dest;
}

std::ostream& operator<<(std::ostream& os, const std::wstring& wstr) {
    return os << wstr.c_str();
}
