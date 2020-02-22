#pragma once

#include "Stream.h"

#include "../TypeTraits/Integral.h"

namespace LFramework {


template<typename T, typename = ::std::enable_if_t<::std::is_integral_v<T>>>
struct DecimalFormat {
	DecimalFormat(T value) :value(value) {}
	T value;
};



template<typename T, bool UpperCase = false, size_t MinWidth = 0, typename = ::std::enable_if_t<::std::is_integral_v<T>>>
struct HexFormat {
	HexFormat(T value) :value(value) {}
	T value;
};

template<bool UpperCase = false, size_t MinWidth = 0, typename T>
HexFormat<T, UpperCase, MinWidth> hex(T value) {
	return HexFormat<T, UpperCase, MinWidth>(value);
}


class OutputStreamFormatter {
public:

	static void writeFiller(OutputStream<char>& stream, size_t itemSize){
		auto fillSize = stream.format.fillWidth - itemSize;
		for(size_t i = 0; i < fillSize; ++i){
			stream.write(&stream.format.fillChar, 1);
		}
		stream.format.fillWidth = 0;
	}

	static void fieldBegin(OutputStream<char>& stream, size_t itemSize){

		if(stream.format.adjust == AdjustField::Right || stream.format.adjust == AdjustField::Internal){
			if(itemSize < stream.format.fillWidth){
				writeFiller(stream, itemSize);
			}
			stream.format.fillWidth = 0;
		}
	}
	static void fieldEnd(OutputStream<char>& stream, size_t itemSize){
		if(stream.format.adjust == AdjustField::Left){
			if(itemSize < stream.format.fillWidth){
				writeFiller(stream, itemSize);
			}
			stream.format.fillWidth = 0;
		}
	}
};

template<typename T>
size_t outputStreamWriteInteger (OutputStream<char>& stream, DecimalFormat<T> formatter){
	auto v = formatter.value;

	static constexpr size_t BufferSize = TypeTraits::Integral::maxDigitsCount<T>() + (::std::is_signed_v<T> ? 1 : 0); //Add space for sign
	char result[BufferSize];

	size_t writePos = BufferSize - 1;

	if constexpr(::std::is_signed_v<T>) {
		//print negative numbers
		if (v < 0) {
			do {
				auto next = v / 10;
				result[writePos--] = static_cast<char>('0' - (v - (next * 10)));
				v = next;
			} while (v != 0);
			result[writePos] = '-';

			auto fieldSize = BufferSize - writePos;

			if(fieldSize < stream.format.fillWidth){

				if(stream.format.adjust == AdjustField::Internal){
					stream.write(&result[writePos], 1);
					OutputStreamFormatter::writeFiller(stream, fieldSize);
					stream.write(&result[writePos + 1], fieldSize - 1);
				}else {
					OutputStreamFormatter::fieldBegin(stream, fieldSize);
					stream.write(&result[writePos], fieldSize);
					OutputStreamFormatter::fieldEnd(stream, fieldSize);
				}

			}else{
				stream.write(&result[writePos], fieldSize);
			}

			return fieldSize;
		}
	}

	//print positive numbers
	do {
		result[writePos--] = static_cast<char>('0' + (v % 10));
		v /= 10;
	} while (v != 0);

	auto fieldSize = BufferSize - writePos - 1;

	OutputStreamFormatter::fieldBegin(stream, fieldSize);
	stream.write(&result[writePos + 1], fieldSize);
	OutputStreamFormatter::fieldEnd(stream, fieldSize);

	return fieldSize;
}



template<typename T, bool UpperCase, size_t MinWidth>
OutputStream<char>& operator <<(OutputStream<char>& stream, const HexFormat<T, UpperCase, MinWidth>& formatter){
	auto v = static_cast<typename std::make_unsigned<T>::type>(formatter.value);

	const auto bufferSize = sizeof(T) * 2;
	char buffer[bufferSize];
	size_t end = bufferSize;

	char a = 'a';
	if constexpr(UpperCase) {
		a = 'A';
	}

	for(size_t i = 0; i < bufferSize; ++i) {
		auto digit = v & 0xf;
		if (digit < 10) {
			buffer[end - 1] = static_cast<char>('0' + digit);
		}
		else {
			buffer[end - 1] = static_cast<char>(a + (digit - 10));
		}
		v >>= 4;
		--end;
		if(v == 0 && (i + 1) >= MinWidth) {
			break;
		}
	}

	const auto dataSize = bufferSize - end;

	OutputStreamFormatter::fieldBegin(stream, dataSize);
	stream.write(&buffer[end], dataSize);
	OutputStreamFormatter::fieldEnd(stream, dataSize);

	return stream;
}



OutputStream<char>& operator <<(OutputStream<char>& stream, const char* value);
OutputStream<char>& operator <<(OutputStream<char>& stream, char value);
OutputStream<char>& operator <<(OutputStream<char>& stream, bool value);

template<typename T>
OutputStream<char>& operator <<(OutputStream<char>& stream, DecimalFormat<T> formatter){
	outputStreamWriteInteger(stream, formatter);
	return stream;
}


template<typename T, typename = std::enable_if_t<
		std::is_integral_v<T> &&
		std::is_arithmetic_v<T> &&
		!std::is_same_v<T, bool> &&
		!std::is_same_v<T, char>
>>
OutputStream<char>& operator <<(OutputStream<char>& stream, T value){
	return stream << DecimalFormat<T>(value);
}

}
