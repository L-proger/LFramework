#pragma once

#include <memory>
#include <vector>
#include <LFramework/DeviceNetwork/NetworkInterface.h>
#include <LFramework/USB/Host/UsbHDevice.h>
#include <LFramework/Threading/Semaphore.h>
#include <LFramework/Threading/CriticalSection.h>
#include <thread>
#include <functional>
#include <LFramework/Debug.h>

namespace LFramework::DeviceNetwork::Host {

class UsbTransmitter : public IDataStream {
public:
    UsbTransmitter(std::shared_ptr<USB::UsbHDevice> device) : _device(device) {
        auto usbInterface = _device->getInterface(0);
        _txEndpoint = usbInterface->getEndpoint(false, 0);
        _rxEndpoint = usbInterface->getEndpoint(true, 0);
    }

    bool start() override {
        //Fill read chain
        for (size_t i = 0; i < 8; ++i) {
            auto chainItem = std::make_shared<ReadChainItem>(_rxEndpoint->getDescriptor().wMaxPacketSize);
            chainItem->readAsync(_rxEndpoint);
            _readChain.push_back(chainItem);
        }

        reset();
        _running = true;

        _rxThread = std::thread(std::bind(&UsbTransmitter::rxThreadHandler, this));
        _txThread = std::thread(std::bind(&UsbTransmitter::txThreadHandler, this));

        lfDebug() << "USB transmitter started";
        return true;
    }

    bool running() const {
        return _running;
    }
private:
    struct ReadChainItem {
       explicit ReadChainItem(size_t bufferSize) {
           buffer.resize(bufferSize);
       }
       std::shared_ptr<USB::WinUsbFuture> asyncResult;
       std::vector<uint8_t> buffer;

       void readAsync(USB::UsbHEndpoint* ep) {
            asyncResult = ep->transferAsync(buffer.data(), buffer.size());
       }
    };
    void onRemoteReset() override {

    }
    void onRemoteDataAvailable() override {
        _txJob.give();
    }
    void onReadBytes() override {
    }

    size_t usbTransmit(void* data, uint32_t size) {
        auto result = _txEndpoint->transferAsync(data, size)->wait();
        return result;
    }

    void sendSyncPacket() {
        usbTransmit(nullptr, 0);
    }

    void rxThreadHandler() {
        while(_running){
            try {
                for(auto& item : _readChain){
                    auto rxSize = item->asyncResult->wait();

                    lfDebug() << "Received USB packet: size=" << rxSize;
                    if(rxSize == 0){
                        _synchronized = true;
                        lfDebug() << "Sync received";
                    }else if(_synchronized) {
                        if( write(item->buffer.data(), rxSize) != rxSize){
                            lfDebug() << "RX buffer fail";
                            for(;;);
                        }else{
                            lfDebug() << "Received synced USB packet: size=" << rxSize;
                        }
                    }

                    if(_running){
                        item->readAsync(_rxEndpoint);
                    }
                }

            }catch (std::exception& ex){
                _running = false;
            }catch(...){
                _running = false;
            }
        }
    }

    void txThreadHandler() {
        std::vector<uint8_t> txBuffer;
        txBuffer.resize(_txEndpoint->getDescriptor().wMaxPacketSize);

        try {
            sendSyncPacket();
            lfDebug() << "Sync sent";
            while(_running){
                _txJob.take();

                while(true){
                    auto size = _remote->read(txBuffer.data(), txBuffer.size());
                    if(size == 0){
                        break;
                    }

                    if(usbTransmit(txBuffer.data(), size) != size){
                        throw std::runtime_error("USB TX fail");
                    }else{
                        lfDebug() << "USB transmitted packet: " << size;
                    }
                }

            }
        } catch (const std::exception & ex) {
            lfDebug() << "TX thread exception: " << ex.what();
            _running = false;
        }
    }


    bool _synchronized = false;

    bool _running = false;
    std::thread _rxThread;
    std::thread _txThread;

    LFramework::Threading::BinarySemaphore _txJob;
    USB::UsbHEndpoint* _txEndpoint = nullptr;
    USB::UsbHEndpoint* _rxEndpoint = nullptr;
    std::shared_ptr<USB::UsbHDevice> _device;
    std::vector<std::shared_ptr<ReadChainItem>> _readChain;
};

}
