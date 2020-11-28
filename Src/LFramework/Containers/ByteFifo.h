#pragma once

#include <array>
#include <cstdint>
#include <cstring>

template<std::size_t _Capacity>
class ByteFifo {
public:
    static constexpr std::size_t Capacity = _Capacity;

    bool write(const void* data, std::size_t size){
        std::size_t availableSize = getAvailableSize();
        if(availableSize >= size){
            auto writeOffset = (_begin + _size) % Capacity;
            auto distanceToEnd = Capacity - writeOffset;
            if(distanceToEnd >= size){
                memcpy(_buffer.data() + writeOffset, data, size);
            }else{
                memcpy(_buffer.data() + writeOffset, data, distanceToEnd);
                memcpy(_buffer.data(), reinterpret_cast<const std::uint8_t*>(data) + distanceToEnd, size - distanceToEnd);
            }
            _size += size;
            if(_size >= 100000){
                   	for(;;);
                   }
            return true;
        }
        return false;
    }

    bool write(std::uint8_t value){
        return write(&value, 1);
    }

    bool read(void* data, std::uint8_t size) {
    	 if(_size >= 100000){
    	        	for(;;);
    	        }
        if(_size < size){
            return false;
        }

        auto distanceToEnd = Capacity - _begin;

        if(distanceToEnd >= size){
            memcpy(data, _buffer.data() + _begin, size);
            _begin = (_begin + size) % Capacity;
        }else{
            memcpy(data, _buffer.data() + _begin, distanceToEnd);
            memcpy(reinterpret_cast<std::uint8_t*>(data) + distanceToEnd, _buffer.data(),  size - distanceToEnd);
            _begin = size - distanceToEnd;
        }
        if(_size >= 100000){
               	for(;;);
               }
        _size -= size;
        if(_size >= 100000){
        	for(;;);
        }
        return true;
    }

    std::size_t discard(std::size_t count){
    	if(count > _size){
    		count = _size;
    	}

    	_begin = (_begin + count) % Capacity;
    	_size -= count;
    	 if(_size >= 100000){
    	        	for(;;);
    	        }
    	return count;
    }

    bool peek(void* data, std::size_t size) {
        if(_size < size){
            return false;
        }

        auto distanceToEnd = Capacity - _begin;

        if(distanceToEnd >= size){
            memcpy(data, _buffer.data() + _begin, size);
        }else{
            memcpy(data, _buffer.data() + _begin, distanceToEnd);
            memcpy(reinterpret_cast<std::uint8_t*>(data) + distanceToEnd, _buffer.data(),  size - distanceToEnd);
        }
        return true;
    }


    bool read(std::uint8_t* value) {
        return read(value, 1);
    }

    std::size_t size() const {
        return _size;
    }
    std::size_t sizeAvailable() const {
		return Capacity - _size;
	}
    bool empty() const {
        return _size == 0;
    }
    void clear() {
    	_begin = 0;
    	_size = 0;
    }
private:
    std::size_t getAvailableSize() const {
        return Capacity - _size;
    }
    std::size_t _size = 0;
    std::size_t _begin = 0;
    std::array<std::uint8_t, Capacity> _buffer;
};
