/*
 * BitField.h
 *
 *  Created on: 14 sept. 2017 ã.
 *      Author: l-pro
 */

#ifndef LFRAMEWORK_BITFIELD_H_
#define LFRAMEWORK_BITFIELD_H_

#include <cstdint>
#include <cstddef>
#include <type_traits>
#include <limits>

namespace LFramework {

	template<typename TBase, typename T>
	struct BitFieldValueConverter {
		static TBase toBase(T value) {
			return static_cast<TBase>(value);
		}
		static T fromBase(TBase value) {
			return static_cast<T>(value);
		}
	};

	template<typename TBase, size_t BitOffset, size_t BitCount = 1, typename T = TBase, typename Converter = BitFieldValueConverter<TBase, T>>
	struct BitFieldMember {
		static constexpr size_t FieldBitsCount = BitCount;
		static constexpr size_t TBitsCount = std::numeric_limits<unsigned char>::digits * sizeof(T);
		static_assert(std::numeric_limits<unsigned char>::digits == 8, "Only systems with 8 bit chars are supported for now");
		static_assert(std::is_standard_layout_v<TBase>, "Only standard layout types supported in unions (and so in BitFieldMember) without UB");
		static_assert(BitOffset + BitCount <= TBitsCount, "Base type bits overflow");

		TBase baseValue;

		static constexpr TBase LowMask = (static_cast<TBase>((~(TBase{ 0 }))) >> (TBitsCount - BitCount));
		static constexpr TBase HiMask = TBase(LowMask << BitOffset);

		template<typename TR = T, typename = std::enable_if_t<!std::is_same_v<TR, TBase>>>
		volatile BitFieldMember& operator =(TR value) volatile {
			baseValue = assign(baseValue, Converter::toBase(value));
			return *this;
		}

		template<typename TR = TBase>
		constexpr BitFieldMember& operator =(TR value) {
			if constexpr(std::is_same_v<TR, TBase>){
				baseValue = assign(baseValue, value);
			}else{
				baseValue = assign(baseValue, Converter::toBase(value));
			}
			return *this;
		}

		constexpr operator T() const {
			return Converter::fromBase((baseValue >> BitOffset) & LowMask);
		}
		static constexpr inline TBase assign(TBase left, TBase right) {
			return (left & ~(HiMask)) | ((right & LowMask) << BitOffset);
		}
	};


static_assert(std::is_standard_layout_v<BitFieldMember<uint32_t, 0, 1>>, "Only standard layout types supported in unions (and so in BitFieldMember) without UB");

}

#endif /* LFRAMEWORK_BITFIELD_H_ */
