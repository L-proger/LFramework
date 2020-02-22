#pragma once

#include <LFramework/USB/USBTypes.h>

namespace LFramework::USB {

class UsbEndpoint {
public:
    UsbEndpoint(const EndpointDescriptor descriptor):_descriptor(descriptor){

	}

    const EndpointDescriptor& getDescriptor() const {
        return _descriptor;
    }

private:
    EndpointDescriptor _descriptor;
};

}
