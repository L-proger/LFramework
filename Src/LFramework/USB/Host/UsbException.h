#pragma once

#include <exception>
#include <string>

namespace LFramework::USB {

class UsbException : public std::exception{
public:
    UsbException(const std::string& message):_message(message){

    }
    char const* what() const noexcept override {
        return _message.data();
    }
private:
    std::string _message;
};

}
