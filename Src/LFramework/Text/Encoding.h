#pragma once

#include <codecvt>
#include <string>

namespace LFramework::Text {
	class Encoding {
	public:
#if defined(_MSC_VER) && _MSC_VER >= 1900
        // https://social.msdn.microsoft.com/Forums/en-US/8f40dcd8-c67f-4eba-9134-a19b9178e481/vs-2015-rc-linker-stdcodecvt-error?forum=vcgeneral
#define _MSVC_CONVERT_WORKAROUND
#endif


#if defined(_MSVC_CONVERT_WORKAROUND)
        static inline std::wstring_convert<std::codecvt_utf8<int32_t>, int32_t> s_u32Converter;
        static inline std::wstring_convert<std::codecvt_utf8_utf16<int16_t>, int16_t> s_u16Converter;
#else
        static inline std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> s_u32Converter;
        static inline std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> s_u16Converter;
#endif

        static std::wstring u8StringToWString(const std::string& src) {
            std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
            return converter.from_bytes(src);
        }

        static std::string wStringToU8String(const std::wstring& src) {
            std::wstring_convert<std::codecvt_utf8<wchar_t>> convert;
            return convert.to_bytes(src);
        }

        static std::u32string u16StringToU32String(const std::u16string& u16String)
        {
            std::string temp{ u16StringToU8String(u16String) };

            return u8StringToU32String(temp);
        }

        static std::string u16StringToU8String(const std::u16string& u16String)
        {
#if defined(_MSVC_CONVERT_WORKAROUND)
            using workaround_str = std::basic_string<int16_t, std::char_traits<int16_t>, std::allocator<int16_t> >;
            workaround_str workaround(reinterpret_cast<const int16_t*>(u16String.c_str()));
            return s_u16Converter.to_bytes(workaround);
#else
            return s_u16Converter.to_bytes(u16String);
#endif
        }

        static std::string u32StringToU8String(const std::u32string& u32String)
        {
#if defined(_MSVC_CONVERT_WORKAROUND)
            using workaround_str = std::basic_string<int32_t, std::char_traits<int32_t>, std::allocator<int32_t> >;
            workaround_str workaround(reinterpret_cast<const int32_t*>(u32String.c_str()));
            return s_u32Converter.to_bytes(workaround);
#else
            return s_u32Converter.to_bytes(u32String);
#endif
        }

        static std::u16string u8StringToU16String(const std::string& u8String)
        {
#if defined(_MSVC_CONVERT_WORKAROUND)
            auto&& workaround = s_u16Converter.from_bytes(u8String);
            return std::u16string(reinterpret_cast<const char16_t*>(workaround.c_str()));
#else
            return s_u16Converter.from_bytes(u8String);
#endif
        }

        static std::u32string u8StringToU32String(const std::string& u8String)
        {
#if defined(_MSVC_CONVERT_WORKAROUND)
            auto&& workaround = s_u32Converter.from_bytes(u8String);
            return std::u32string(reinterpret_cast<const char32_t*>(workaround.c_str()));
#else
            return s_u32Converter.from_bytes(u8String);
#endif
        }
	};
}


#ifdef _MSVC_CONVERT_WORKAROUND
#undef _MSVC_CONVERT_WORKAROUND
#endif