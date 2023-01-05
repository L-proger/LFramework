#pragma once

#include <algorithm>
#include <string>
#include <fstream>

namespace LFramework::USB::SysfsUtils {

    inline bool isUsbDeviceName(const std::string& name){
        return !( 
            std::any_of(name.begin(), name.end(), [](char c){ return std::isalpha(c); }) 
            || name == "." || name == ".."
            || name.find(':') != std::string::npos
            || name.find("usb") != std::string::npos);
    }

    inline std::string readDeviceAttribute(const std::string &devicePath, const char *attributeName) {
        std::string attributeValue = "";
        auto attributeFilePath = devicePath + attributeName;
        std::ifstream attributeFile;
        attributeFile.open(attributeFilePath);
        if(attributeFile.is_open()) {
            attributeFile >> attributeValue;
        }
        attributeFile.close();
        return attributeValue;
    }

    template<class T>
    inline T readIntDeviceAttribute(const std::string &devicePath, const char *attributeName, uint base){
        auto strValue = readDeviceAttribute(devicePath, attributeName);
        return static_cast<T>(std::strtol(strValue.c_str(), nullptr, base));
    }
}