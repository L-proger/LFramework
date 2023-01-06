#include "UsbHostEndpoint.h"
#include "UsbBulkTransfer.h"
#include <stdexcept>

namespace LFramework::USB {


UsbHostEndpoint::UsbHostEndpoint(const USB::EndpointDescriptor& descriptor, int deviceFileDescriptor, std::uint32_t deviceCaps)
    :IUsbHostEndpoint(descriptor), _deviceFileDescriptor(deviceFileDescriptor), _deviceCaps(deviceCaps){

}   

std::shared_ptr<IUsbTransfer> UsbHostEndpoint::transferAsync(void* buffer, size_t size) {
    if(getDescriptor().bEndpointAddress.isIn()){
        return readAsync(buffer, size);
    }else{
        return writeAsync(buffer, size);
    }
}


std::shared_ptr<IUsbTransfer> UsbHostEndpoint::readAsync(void* buffer, size_t size){
    return std::make_shared<UsbBulkTransfer>(_deviceFileDescriptor, _deviceCaps, getDescriptor().bEndpointAddress.value, buffer, size);
}
std::shared_ptr<IUsbTransfer> UsbHostEndpoint::writeAsync(void* buffer, size_t size){
    return std::make_shared<UsbBulkTransfer>(_deviceFileDescriptor, _deviceCaps, getDescriptor().bEndpointAddress.value, buffer, size);
}


}