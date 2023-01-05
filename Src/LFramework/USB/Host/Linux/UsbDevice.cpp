#include "UsbDevice.h"
#include "UsbInterface.h"
#include "SysfsUtils.h"

#include <cstdint>

#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>

#include <sys/types.h>
#include <dirent.h>

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

    enumerateInterfaces(path);
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

        _interfaces.push_back(std::make_shared<UsbInterface>(_deviceFileDescriptor, sysfsDevicePath, interfaceDirectory) );

        //UsbInterfaceParameters interfaceParameters;
        //initInterface(interfaceDirectory, &interfaceParameters);
        //interfaces.emplace_back(interfaceParameters);
    }
    closedir(directory);
}


std::shared_ptr<IUsbInterface> UsbDevice::getInterface(std::size_t id) {
    throw std::runtime_error("Not implemented");
}
std::size_t UsbDevice::getInterfaceCount() const {
    throw std::runtime_error("Not implemented");
}


IUsbDevice* openUsbDevice(const std::string& path){
    return new UsbDevice(path);
}

}