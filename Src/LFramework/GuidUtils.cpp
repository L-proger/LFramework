#ifdef WIN32
#include <LFramework/GuidUtils.h>
#include <LFramework/Text/Encoding.h>
#include <Windows.h>
#include <stdexcept>

namespace LFramework::GuidUtils {
	LFramework::Guid newGuid() {
		LFramework::Guid result;
		if (CoCreateGuid((GUID*)&result) != S_OK) {
			throw std::runtime_error("Failed to create GUID");
		}

		return result;
	}

	LFramework::Guid fromString(const char* str) {
		LFramework::Guid iid;
		if (IIDFromString(LFramework::Text::Encoding::u8StringToWString(str).c_str(), (GUID*)&iid) != S_OK) {
			throw std::runtime_error("Failed to parse GUID");
		}
		return iid;
	}

	std::string toString(const LFramework::Guid& guid) {
		LPOLESTR guid_str = nullptr;
		if (StringFromIID(*((const GUID*)&guid), &guid_str) != S_OK) {
			throw std::runtime_error("Failed to convert GUID to string");
		}
		std::wstring result(guid_str);
		CoTaskMemFree(guid_str);
		return LFramework::Text::Encoding::wStringToU8String(result);
	}
}

#endif