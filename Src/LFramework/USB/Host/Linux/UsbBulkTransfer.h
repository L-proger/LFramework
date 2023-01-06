#pragma once

#include "../IUsbTransfer.h"
#include <stdexcept>
#include "UsbIoctl.h"
#include <vector>
#include <mutex>
#include <condition_variable>

#include <iostream>
#include <iomanip>

namespace LFramework::USB {

class UsbBulkTransfer : public IUsbTransfer {
public:

    UsbBulkTransfer(int usbDeviceFileDescriptor, std::uint32_t deviceCaps, std::uint8_t endpointAddress, const void* data, std::size_t size) 
        : _usbDeviceFileDescriptor(usbDeviceFileDescriptor){
    
        std::cout << "BulkTransfer: address = 0x" << std::hex << (uint32_t)endpointAddress  << std::dec << " size = " << size  << std::endl;

        bool supportsBulkContinuation = (deviceCaps & UsbCaps::BulkContinuation) != 0;

        std::size_t blockSize = calculateMaxUrbSize(deviceCaps, size);
        std::size_t urbCount = size == 0 ? 1 : ((size + (blockSize - 1)) / blockSize);
        bool isLastUrbPartial = ((size != 0) && (size % blockSize != 0));
       
        _urb.reserve(urbCount);

        const std::uint8_t* buffer = reinterpret_cast<const std::uint8_t*>(data);

        //Create URBs
        for(std::size_t i = 0; i < urbCount; ++i){

            UsbUrb urb = {}; 

            urb.userContext = this;
            urb.type = UsbUrbType::UsbUrbTypeBulk;
            urb.endpoint = endpointAddress;
            urb.buffer = const_cast<std::uint8_t*>(buffer);

            if(size == 0){
                urb.flags |= UsbUrbFlags::ZeroPacket;
            }

            bool isLastUrb = i == (urbCount - 1);
            if(((endpointAddress & 0x80) != 0) && !isLastUrb){
                urb.flags |= UsbUrbFlags::ShortNotOk;
            }

            //If multiple URBs and bulk continuation supported
            if((i != 0) && supportsBulkContinuation){ 
                urb.flags |= UsbUrbFlags::BulkContinuation;
            }


            if(isLastUrb && isLastUrbPartial){
                urb.bufferLength = static_cast<uint32_t>(size % blockSize);
            }else {
                urb.bufferLength = static_cast<uint32_t>((size == 0) ? 0 : blockSize);
            }


            auto ioctlResult = ioctl(_usbDeviceFileDescriptor, IOCTL_USBFS_SUBMITURB, &_urb.emplace_back(urb));
            if(ioctlResult < 0){
                discard();
                throw std::runtime_error("Failed to submit USB URB");
            }


            buffer += blockSize;
        }
    }

    void notify(UsbUrb* urb){
        std::lock_guard<std::mutex> lock(_urbMutex);

        if(urb->status == 0){
            _finishedBytes += urb->actualLength;
            _finishedUrbCount++;
            if(_finishedUrbCount == _urb.size()){
                _urbCv.notify_one();
            }
        }else{
            _error = true;
            discard();
            _urbCv.notify_one();
        }
    }

    std::size_t wait() override {
        
        std::unique_lock<std::mutex> lock(_urbMutex);

        while(!_error && (_finishedUrbCount != _urb.size())){
            _urbCv.wait(lock, [this](){
                return _error || (_finishedUrbCount == _urb.size());
            });
        }
        
        if(_error) {
            throw std::runtime_error("UbsBulkTransfer error");
        }

        return _finishedBytes;
    }
private:
    std::vector<UsbUrb> _urb;
    int _usbDeviceFileDescriptor;
    std::size_t _finishedUrbCount = 0;
    std::size_t _finishedBytes = 0;
    bool _error = false;

    std::mutex _urbMutex;
    std::condition_variable _urbCv;
    

    void discard() {
        auto urbIterator = _urb.begin();
        auto iterator = std::next(urbIterator, _finishedUrbCount);
        while (iterator != _urb.end()) {
            auto& urb = (*iterator);
            ioctl(_usbDeviceFileDescriptor, IOCTL_USBFS_DISCARDURB, &urb);
            urb.userContext = nullptr;
            iterator++;
        }
    }

    static std::size_t calculateMaxUrbSize(std::uint32_t deviceCaps, std::size_t fullTransferSize) {
        constexpr std::size_t maxBlockSize = 16384;

        if((deviceCaps & (UsbCaps::BulkScatterGather | UsbCaps::NoPacketSizeLim)) != 0){
            return fullTransferSize;
        }
        
        return std::min(maxBlockSize, fullTransferSize);
    }
};

}