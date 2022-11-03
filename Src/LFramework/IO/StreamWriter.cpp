#include "StreamWriter.h"

namespace LFramework {

OutputStream<char>& operator <<(OutputStream<char>& stream, const char* value){
	auto len = strlen(value);
	OutputStreamFormatter::fieldBegin(stream, len);
	stream.write(value, static_cast<int>(len));
	OutputStreamFormatter::fieldEnd(stream, len);
	return stream;
}

OutputStream<char>& operator <<(OutputStream<char>& stream, char value){
	OutputStreamFormatter::fieldBegin(stream, 1);
	stream.write(&value, 1);
	OutputStreamFormatter::fieldEnd(stream, 1);
	return stream;
}

OutputStream<char>& operator <<(OutputStream<char>& stream, bool value){
	return stream << (value ? "true" : "false");
}


}
