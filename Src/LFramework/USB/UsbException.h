#pragma once

#include <stdexcept>
#include <string>

namespace LFramework::USB {

class UsbException : public std::exception {
public:
    UsbException(const std::string& message):_message(message){ }
    char const* what() const override {
        return _message.c_str();
    }
private:
    std::string _message;
};

}
