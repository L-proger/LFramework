#pragma once

#include <stdexcept>
#include <string>
#include <Windows.h>

namespace LFramework::USB {

class UsbHException : public std::exception{
public:
    UsbHException(const std::string& message):_message(message){

    }
    char const* what() const override {
        return _message.data();
    }
private:
    std::string _message;
};


class UsbHWinusbErrorCodeException : public UsbHException {
public:
    UsbHWinusbErrorCodeException(const std::string& customDescription):UsbHException (customDescription + " " + getErrorText()){

    }
    UsbHWinusbErrorCodeException():UsbHException (getErrorText()){

    }
    static std::string getErrorText(){
        auto errorCode = GetLastError();
        LPSTR messageBuffer = nullptr;
        auto messageLength = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, nullptr, errorCode,
                       MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT), LPSTR(&messageBuffer), 0, nullptr);

        std::string message(messageBuffer, messageLength);
        LocalFree(messageBuffer);
        return "[" + std::to_string(errorCode) + "] " + message;
    }
};


}