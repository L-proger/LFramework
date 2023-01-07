#pragma once

#include "../IUsbDevice.h"

#include <vector>
#include <memory>
#include <thread>
#include <sys/epoll.h>
#include <unistd.h>
#include <optional>
#include <stdexcept>

namespace LFramework::USB {

struct FileDescriptor {
    FileDescriptor() = default;
    explicit FileDescriptor(int value) : _value(value){}
    FileDescriptor(const FileDescriptor&) = delete;

    FileDescriptor& operator =(int newValue){
        if(_value != newValue){
            close();
            _value = newValue;
        }
        return *this;
    }

    void close() {
        if(_value >= 0){
            ::close(_value);
            _value = -1;
        }
    }

    int value() const {
        return _value;
    }

    ~FileDescriptor() {
        close();
    }
private:
    int _value = -1;
};

struct EpollContext {
public:
    EpollContext(int pollTargetDescriptor, uint32_t events) {
        _epollFileDescriptor.emplace(epoll_create(1)); 
        if(_epollFileDescriptor.value().value() < 0) {
            throw std::runtime_error("epoll_create failed");
        }
     
        _epollEvent.events = events; //EPOLLIN | EPOLLOUT | EPOLLERR;
        _epollEvent.data.fd = pollTargetDescriptor;
        auto epollCtlResult = epoll_ctl(_epollFileDescriptor.value().value(), EPOLL_CTL_ADD, pollTargetDescriptor, &_epollEvent);

        if (epollCtlResult < 0){
            _epollFileDescriptor.reset();
           throw std::runtime_error("epoll_ctl failed");
        }
    }

    ~EpollContext() {
        epoll_ctl(_epollFileDescriptor.value().value(), EPOLL_CTL_DEL, _epollEvent.data.fd , &_epollEvent);
        _epollFileDescriptor.reset();
    }
    const std::optional<FileDescriptor>& fileDescriptor() const {
        return _epollFileDescriptor;
    }
private:
    epoll_event _epollEvent = {};
    std::optional<FileDescriptor> _epollFileDescriptor;
};


class UsbDevice : public IUsbDevice {
public:
    UsbDevice(const std::string& path);
    ~UsbDevice();
    std::shared_ptr<IUsbInterface> getInterface(std::size_t id) override;
    std::size_t getInterfaceCount() const override;
private:
    FileDescriptor _deviceFileDescriptor;
    std::vector<std::shared_ptr<IUsbInterface>> _interfaces;
    std::uint32_t _deviceCaps;

    std::thread _epollThread;
    std::optional<EpollContext> _epoll;
   
    bool _epollThreadRunning = false;

    void enumerateInterfaces(const std::string& sysfsDevicePath);

    void epollThreadHandler();


};

}