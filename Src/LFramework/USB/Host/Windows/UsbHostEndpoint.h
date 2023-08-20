#pragma once

#include "../IUsbHostEndpoint.h"
#include <Windows.h>
#include "UsbHException.h"
#include <winusb.h>
#include <memory>

namespace LFramework::USB {

//class Future {
//public:
//    virtual ~Future() = default;
//    virtual size_t wait() = 0;
//};
class WinUsbFuture : public IUsbTransfer {
public:
    WinUsbFuture(HANDLE deviceHandle, std::unique_ptr<OVERLAPPED> &&overlapped):_deviceHandle(deviceHandle), _overlapped(std::move(overlapped)){

    }
    size_t wait() override {
        ULONG bytesTransferred = 0;
        auto isContinuousWaiting = TRUE;
        auto overlappedResult = false;

        if(_overlapped == nullptr) {
            throw UsbHException("Can not wait multiple times");
        }

        auto waitResult = WaitForSingleObjectEx(_overlapped->hEvent, INFINITE, true);
        if(waitResult == WAIT_OBJECT_0){
            overlappedResult = static_cast<bool>(GetOverlappedResult(_deviceHandle, _overlapped.get(), &bytesTransferred, isContinuousWaiting));
        }

        CloseHandle(_overlapped->hEvent);

        if(!overlappedResult) {
           throw UsbHWinusbErrorCodeException();
        }

        return static_cast<size_t>(bytesTransferred);
    }
private:
    HANDLE _deviceHandle;
    std::unique_ptr<OVERLAPPED> _overlapped;
};

class UsbHostEndpoint : public IUsbHostEndpoint {
public:
    UsbHostEndpoint(const USB::EndpointDescriptor& descriptor, HANDLE deviceHandle, WINUSB_INTERFACE_HANDLE interfaceHandle):IUsbHostEndpoint(descriptor), _interfaceHandle(interfaceHandle),_deviceHandle(deviceHandle){

    }

    std::shared_ptr<IUsbTransfer> transferAsync(void* buffer, size_t size){
        if(getDescriptor().bEndpointAddress.isIn()){
            return readAsync(buffer, size);
        }else{
            return writeAsync(buffer, size);
        }
    }

private:
    std::shared_ptr<WinUsbFuture> readAsync(void* buffer, size_t size){
        //auto index = _endpointsMap.at(endpoint.getAddress());
        ULONG bytesRead;
        auto overlapped = std::make_unique<OVERLAPPED>();
        ZeroMemory(overlapped.get(), sizeof(OVERLAPPED));
        overlapped->hEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);

        if (overlapped->hEvent == nullptr) {
            throw UsbHWinusbErrorCodeException();
        }
        auto asyncReadResult = static_cast<bool>(WinUsb_ReadPipe(_interfaceHandle, getDescriptor().bEndpointAddress.value, reinterpret_cast<UCHAR*>(buffer), static_cast<ULONG>(size), &bytesRead, overlapped.get()));
        if (overlapped->hEvent == nullptr) {
            throw UsbHWinusbErrorCodeException();
        }
        if(!asyncReadResult && (GetLastError() != ERROR_IO_PENDING)){
            throw UsbHWinusbErrorCodeException();
        }
        return std::make_shared<WinUsbFuture>(_deviceHandle, std::move(overlapped));
    }

    std::shared_ptr<WinUsbFuture> writeAsync(const void* buffer, size_t size){

        ULONG bytesWritten;
        auto overlapped = std::make_unique<OVERLAPPED>();
        ZeroMemory(overlapped.get(), sizeof(OVERLAPPED));
        overlapped->hEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
        overlapped->Internal = STATUS_PENDING;

        auto asyncWriteResult = static_cast<bool>(WinUsb_WritePipe(_interfaceHandle, getDescriptor().bEndpointAddress.value,
            reinterpret_cast<UCHAR*>(const_cast<void*>(buffer)), static_cast<ULONG>(size), &bytesWritten, overlapped.get()));

        if(!asyncWriteResult && (GetLastError() != ERROR_IO_PENDING)) {
            throw UsbHWinusbErrorCodeException();
        }

        return std::make_shared<WinUsbFuture>(_deviceHandle, std::move(overlapped));
    }

    WINUSB_INTERFACE_HANDLE _interfaceHandle;
    HANDLE _deviceHandle;
};

}
