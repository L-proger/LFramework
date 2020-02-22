#pragma once

#include "../StreamWriter.h"

namespace LFramework::Terminal {
	class Stream : public OutputStream<char>{
	public:
		int write(const char* data, int length) override final;
	};
}

