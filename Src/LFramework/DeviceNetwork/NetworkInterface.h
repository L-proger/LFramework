#pragma once

#include <cstdint>
#include <cstddef>
#include <LFramework/Containers/ByteFifo.h>
#include <algorithm>

namespace LFramework::DeviceNetwork {

struct IDataStream {
    using Buffer = ByteFifo<1024>;

    virtual ~IDataStream() = default;
    void bind(IDataStream* remote){
        _remote = remote;
    }
    virtual bool start() = 0;
    //virtual bool getPacket(std::uint8_t& id, std::uint8_t& size, void* buffer) = 0;

    std::size_t write(const void* data, std::size_t size){
        if(size == 0){
            return 0;
        }

        size = (std::min)(size, Buffer::Capacity - _buffer.size());

        if(_buffer.write(data, size)){
            _remote->onRemoteDataAvailable();
            return size;
        }
        return 0;
    }
    std::size_t read(void* data, std::size_t maxSize){
        if(_buffer.size() == 0){
            return 0;
        }
        auto size = (std::min)(_buffer.size(), maxSize);
        _buffer.read(data, size);
        onReadBytes();
        return size;
    }

    std::size_t discard(std::size_t count){
    	auto result = _buffer.discard(count);
    	if(result != 0){
    		onReadBytes();
    	}
    	return result;
    }

    std::size_t peek(void* data, std::size_t maxSize){
        if(_buffer.size() == 0){
            return 0;
        }
        auto size = (std::min)(_buffer.size(), maxSize);
        _buffer.peek(data, size);;
        return size;
    }
    std::size_t bytesAvailable() const {
        return _buffer.size();
    }
    std::size_t freeSpace() const {
	   return Buffer::Capacity - _buffer.size();
    }
    void reset() {
    	clear();
    	_remote->onRemoteReset();
    }
protected:
    virtual void onRemoteReset() = 0;
    virtual void onRemoteDataAvailable() = 0;
    virtual void onReadBytes() = 0;
    IDataStream* _remote = nullptr;
    void clear() {
    	_buffer.clear();
    }
private:
    Buffer _buffer;
};

}
