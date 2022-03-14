#pragma once
#include <string>
#include <msclr\marshal_cppstd.h>
#include <msclr\lock.h> 
#include <codecvt>
using namespace System;
using namespace System::Text;
using namespace System::Collections::Generic;
class Utils
{
public:
    static char * StringToUTF8CharPtr(String^ str) {
        if (str == nullptr) return nullptr;

        msclr::interop::marshal_context context;
        std::wstring utf16Str = context.marshal_as<std::wstring>(str);
        std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> convert;
        std::string utf8Str = convert.to_bytes(utf16Str);
        char *cstr = new char[utf8Str.length() + 1];
        strcpy_s(cstr, utf8Str.length() + 1, utf8Str.c_str());
        return cstr;
    }

    
    static String ^ CharPtrToString(const char* str) {
        if (str == nullptr) return nullptr;
        int nWide = ::MultiByteToWideChar(CP_UTF8, 0, str, (int)strlen(str), NULL, 0);

        WCHAR* buffer = new WCHAR[nWide + 1];
        if (NULL == buffer) {
            return L"";
        }
        ::MultiByteToWideChar(CP_UTF8, 0, str, (int)strlen(str), buffer, nWide);
        buffer[nWide] = L'\0';
        std::wstring strWide(buffer);

        delete[] buffer;
        buffer = nullptr;

        String ^ s = msclr::interop::marshal_as<String ^>(strWide);
        return s;
    }
};

