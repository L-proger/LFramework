#pragma once

#include <string>
#include <LFramework/Guid.h>

namespace LFramework::GuidUtils {
	LFramework::Guid newGuid();
	LFramework::Guid fromString(const char* str);
	std::string toString(const LFramework::Guid& guid);
}
