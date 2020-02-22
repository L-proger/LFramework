#ifndef DetectEndianness_h__
#define DetectEndianness_h__

#define LF_ENDIANNESS_UNKNOWN 0
#define LF_ENDIANNESS_LITTLE 1
#define LF_ENDIANNESS_BIG 2

#ifndef LF_ENDIANNESS

#if defined(__BIG_ENDIAN__) || defined(__ARMEB__) || defined(__THUMBEB__) \
|| defined(__AARCH64EB__) || defined(_MIPSEB) || defined(__MIPSEB) || defined(__MIPSEB__) \
|| (defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)) \
|| (defined(__FLOAT_WORD_ORDER__) && (__FLOAT_WORD_ORDER__ == __ORDER_BIG_ENDIAN__))
#define LF_ENDIANNESS LF_ENDIANNESS_BIG

#elif defined(__LITTLE_ENDIAN__) || defined(__ARMEL__) || defined(__THUMBEL__) \
|| defined(__AARCH64EL__) || defined(_MIPSEL) || defined(__MIPSEL) || defined(__MIPSEL__) \
|| (defined(__BYTE_ORDER__) && (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)) \
|| (defined(__FLOAT_WORD_ORDER__) && (__FLOAT_WORD_ORDER__ == __ORDER_LITTLE_ENDIAN__))
#define LF_ENDIANNESS LF_ENDIANNESS_LITTLE

#else
#define LF_ENDIANNESS LF_ENDIANNESS_UNKNOWN
#endif

#endif

namespace LFramework {

	enum class Endianness {
		Unknown = LF_ENDIANNESS_UNKNOWN,
		Little = LF_ENDIANNESS_LITTLE,
		Big = LF_ENDIANNESS_BIG
	};

	namespace Detect {

		static constexpr Endianness endianness() {
			return static_cast<Endianness>(LF_ENDIANNESS);
		}

		static Endianness endiannessRuntime() {
			union {
				uint32_t value;
				uint8_t data[sizeof(uint32_t)];
			} number;

			number.data[0] = 0x00;
			number.data[1] = 0x01;
			number.data[2] = 0x02;
			number.data[3] = 0x03;

			switch (number.value){
			case 0x00010203U: return Endianness::Big;
			case 0x03020100U: return Endianness::Little;
			default:          return Endianness::Unknown;
			}
		}
	}
}


#endif // DetectEndianness_h__
