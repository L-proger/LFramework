#include <LFrameworkConfig.h>
#if defined(LF_UNIT_TEST) && defined(LF_SELF_TEST)

#include "UnitTest/UnitTest.h"
#include "BitField.h"

using namespace LFramework;

union Test8BitField {
	constexpr Test8BitField():value(0){}
	uint8_t value;
	BitFieldMember<uint8_t, 0, 1> bit_0_1;
	BitFieldMember<uint8_t, 1, 3> bit_1_3;
	BitFieldMember<uint8_t, 4, 4> bit_4_4;
};

TEST(BitField_8bit){
	Test8BitField bitfield;
	TEST_TRUE(bitfield.value == 0);

	bitfield.bit_0_1 = 1;
	TEST_TRUE(bitfield.value == 1);
	TEST_TRUE(bitfield.bit_0_1 == 1);

	bitfield.bit_1_3 = 4;
	TEST_TRUE(bitfield.bit_1_3 == 4);
	TEST_TRUE(bitfield.value == 9);
	bitfield.bit_1_3 = 0;
	TEST_TRUE(bitfield.value == 1);

	bitfield.bit_4_4 = 1;
	TEST_TRUE(bitfield.value == 17);

	bitfield.bit_4_4 = 15;
	TEST_TRUE(bitfield.value == 241);

	bitfield.bit_1_3 = 7;
	TEST_TRUE(bitfield.value == 0xff);

	bitfield.bit_0_1 = 0;
	TEST_TRUE(bitfield.value == 0xfe);

	bitfield.bit_4_4 = 0;
	TEST_TRUE(bitfield.value == 0x0e);

	bitfield.bit_1_3 = 0;
	TEST_TRUE(bitfield.value == 0x00);
}

#endif