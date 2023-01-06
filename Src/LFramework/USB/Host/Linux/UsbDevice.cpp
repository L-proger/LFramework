#include "UsbDevice.h"
#include "UsbInterface.h"
#include "SysfsUtils.h"
#include "UsbIoctl.h"
#include "UsbBulkTransfer.h"

#include <cstdint>

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include <sys/types.h>
#include <dirent.h>

#include <iostream>

namespace LFramework::USB {

static bool fileExist(const std::string &filePath) {
    auto result = false;
    std::ifstream attributeFile;
    attributeFile.open(filePath);
    if(attributeFile.is_open()) {
        result = true;
    }
    attributeFile.close();
    return result;
}


static void prependZeroes(std::string &str) {
    const uint8_t desiredLength = 3;
    const auto valueToInsert = '0';
    auto len = static_cast<uint>(str.length());
    str.insert(str.begin(), desiredLength - len, valueToInsert);
}

static std::string getDeviceNodePath(const std::string &path) {
    auto busnum = SysfsUtils::readDeviceAttribute(path, "busnum");
    auto devnum = SysfsUtils::readDeviceAttribute(path, "devnum");
    prependZeroes(busnum);
    prependZeroes(devnum);
    auto nodePath = "/dev/bus/usb/" + busnum + '/' + devnum;
    return nodePath;
}


UsbDevice::UsbDevice(const std::string& path) {
    auto deviceNodePath = getDeviceNodePath(path);

    _deviceFileDescriptor = open(deviceNodePath.c_str(), O_RDWR | O_NONBLOCK | O_ASYNC);
    if(_deviceFileDescriptor.value() < 0){
        throw std::runtime_error("Failed to open USB device");
    }

    _epoll.emplace(_deviceFileDescriptor.value(), EPOLLIN | EPOLLOUT | EPOLLERR);

    _deviceCaps = getUsbDeviceCapabilities(_deviceFileDescriptor.value());

    enumerateInterfaces(path);

    _epollThreadRunning = true;
    _epollThread = std::thread([this](){
        epollThreadHandler();
    });
}

UsbDevice::~UsbDevice(){
    _interfaces.clear();
    _epoll.reset();
    _deviceFileDescriptor.close();
    _epollThreadRunning = false;

    if(_epollThread.joinable()) {
        _epollThread.join();
    }
}

void UsbDevice::enumerateInterfaces(const std::string& sysfsDevicePath){
    dirent *directoryEntry;
    DIR *directory = nullptr;
    directory = opendir(sysfsDevicePath.c_str());
    while ((directoryEntry = readdir(directory)) != nullptr) {
        if (directoryEntry->d_type != DT_DIR) {
            continue;
        }
        if (!static_cast<bool>(isdigit(directoryEntry->d_name[0]))) {
            continue;
        }
        auto interfaceDirectory = std::string(sysfsDevicePath + directoryEntry->d_name + '/');
        auto interfaceClassFilePath = interfaceDirectory + "bInterfaceClass";
        if (!fileExist(interfaceClassFilePath)) {
            continue;
        }

        _interfaces.push_back(std::make_shared<UsbInterface>(_deviceFileDescriptor.value(), sysfsDevicePath, interfaceDirectory, _deviceCaps) );

        //UsbInterfaceParameters interfaceParameters;
        //initInterface(interfaceDirectory, &interfaceParameters);
        //interfaces.emplace_back(interfaceParameters);
    }
    closedir(directory);
}

void UsbDevice::epollThreadHandler(){
    UsbUrb *context = nullptr;
    epoll_event event{};
    while(_epollThreadRunning){
        auto readyFileDescriptors = epoll_wait(_epoll->fileDescriptor()->value(), &event, 1, 100); 

        //TODO: check if device file descriptor is still valid?
        if(readyFileDescriptors < 0){
            int err = errno;
            if(err == EINTR){
                continue;
            }else{
                break;
            }
            
        }

        if(((event.events & EPOLLERR) != 0) ||
                (errno == EBADF) ||
                (errno == EFAULT) ||
                (errno == EINVAL) ) {
            break;
        }

        if(errno == EAGAIN) { //Timeout 
            continue;
        }

        if((readyFileDescriptors > 0) && ((event.events & EPOLLOUT) != 0)){
            int urbCompletionStatus = ioctl(_deviceFileDescriptor.value(), IOCTL_USBFS_REAPURB, &context);

            int err = errno;
            if(urbCompletionStatus < 0){
                if (errno == EAGAIN)
                    continue;
                if (errno == ENODEV)
                    continue;
            }

            if (context != nullptr) {
                auto transfer = static_cast<UsbBulkTransfer*>(context->userContext);
                if(transfer != nullptr){
                    transfer->notify(context);
                }
            }
        }
    }

    std::cout << "Device Epoll thread exit" << std::endl;
}


std::shared_ptr<IUsbInterface> UsbDevice::getInterface(std::size_t id) {
    if(id >= _interfaces.size()){
        throw std::runtime_error("Invalid interface ID");
    }
    return _interfaces[id];
}
std::size_t UsbDevice::getInterfaceCount() const {
    return _interfaces.size();
}


IUsbDevice* openUsbDevice(const std::string& path){
    return new UsbDevice(path);
}

}