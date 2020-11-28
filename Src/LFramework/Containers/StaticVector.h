#pragma once

#include <cstdint>
#include <cstddef>
#include <type_traits>
#include <new>
#include <algorithm>
#include <initializer_list>

namespace LFramework {

template<class ValueType, size_t Capacity>
class StaticVector {
public:
	typedef ValueType& ValueReference;
	typedef const ValueReference& ConstValueReference;

	StaticVector() = default;

	~StaticVector() {
	    clear();
	}

	constexpr std::size_t capacity() const {
		return Capacity;
	}

	std::size_t size() const {
		return _size;
	}

	bool empty() const {
		return _size == 0;
	}

	bool full() const {
		return _size == Capacity;
	}

	void clear() {
	    for(auto it = begin(); it != end(); ++it) {
	        it->~value_type();
	    }
	    _size = 0;
	}

	ValueType* begin() {
		return reinterpret_cast<ValueType*>(&_buffer[0]);
	}

	const ValueType* begin() const {
		return  reinterpret_cast<const ValueType*>(&_buffer[0]);
	}

	ValueType* end() {
		return reinterpret_cast<ValueType*>(&_buffer[_size]);
	}

	const ValueType* end() const {
		return reinterpret_cast<const ValueType*>(&_buffer[_size]);
	}

	ValueReference back() {
		return *reinterpret_cast<ValueType*>(&_buffer[_size - 1]);
	}

	ConstValueReference back() const {
		return *reinterpret_cast<const ValueType*>(&_buffer[_size - 1]);
	}

	template<typename ...Args>
	bool emplace_back(Args&&... args) {
		if(!full()) {
			::new(&_buffer[_size++]) ValueType(std::forward<Args>(args)...);
			return true;
		}
		return false;
	}

	bool push_back(ConstValueReference value) {
		if (!full()) {
			::new((void*)&_buffer[_size++]) ValueType(value);
			return true;
		}
		return false;
	}

	void erase(const std::size_t first, const std::size_t count) {
		std::size_t last = first + count;
		if ((last <= first) || (last > _size)) {
			return;
		}

		//move items
		auto writePtr = reinterpret_cast<ValueType*>(&_buffer[first]);
		for(std::size_t i = last; i < _size; ++i){
			*writePtr = std::move(*reinterpret_cast<ValueType*>(&_buffer[i]));
			++writePtr;
		}

		//destroy tail
		for(std::size_t i = first; i < last; ++i){
			writePtr->~value_type();
			++writePtr;
		}

		_size -= (last - first);
	}

	void erase(const ValueType& value) {
		for (std::size_t i = 0; i < _size; ++i) {
			if (_buffer[i] == value) {
				erase(i, 1);
				return;
			}
		}
	}

	void erase(const ValueType* itemPtr) {
		if(itemPtr < begin()){
			return;
		}
		std::size_t id = itemPtr - begin();
		erase(id, 1);
	}

	void pop_back() {
		if (!empty()) {
			reinterpret_cast<ValueType*>(&_buffer[--_size])->~value_type();
		}
	}

	ValueReference operator[](std::size_t i) {
		return *reinterpret_cast<ValueType*>(&_buffer[i]);
	}

	ConstValueReference operator[](std::size_t i) const {
		return *reinterpret_cast<const ValueType*>(&_buffer[i]);
	}

	bool contains(ConstValueReference value) {
		for (std::size_t i = 0; i < _size; ++i) {
			if (_buffer[i] == value) {
				return true;
			}
		}
		return false;
	}

	std::size_t resize(std::size_t newSize) {
		if (newSize > Capacity) {
			newSize = Capacity;
		}

		for(std::size_t i = _size; i < newSize; ++i){
			::new(reinterpret_cast<ValueType*>(&_buffer[i])) ValueType;
		}

		for(std::size_t i = newSize; i < _size; ++i){
			reinterpret_cast<ValueType*>(&_buffer[i])->~value_type();
		}

		_size = newSize;
		return newSize;
	}

private:
	typename std::aligned_storage<sizeof(ValueType), alignof(ValueType)>::type _buffer[Capacity];
	std::size_t _size = 0;
};

}
