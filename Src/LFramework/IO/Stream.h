#pragma once

#include <cstdint>
#include <cstring>

namespace LFramework {


template<typename T>
class OutputStream {
public:

};

enum class AdjustField {
	Left,
	Right,
	Internal
};

template<typename TChar>
class OutputStreamFormat {
public:
	size_t fillWidth = 0;
	TChar fillChar{};
	AdjustField adjust = AdjustField::Right;
};

template<>
class OutputStream<char> {
public:
	OutputStreamFormat<char> format;

	virtual ~OutputStream() = default;
	virtual int write(const char* data, int length) = 0;

};

}
