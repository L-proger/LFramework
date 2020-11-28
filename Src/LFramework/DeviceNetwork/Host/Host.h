#pragma once

#include <memory>
#include <vector>
#include <LFramework/DeviceNetwork/NetworkInterface.h>
#include <LFramework/USB/Host/UsbHDevice.h>
#include <LFramework/Threading/Semaphore.h>
#include <LFramework/Threading/CriticalSection.h>
#include <LFramework/DeviceNetwork/Packet.h>
#include <LFramework/Assert.h>
#include <thread>
#include <mutex>
#include <functional>
#include <LFramework/Debug.h>

namespace LFramework::DeviceNetwork::Host {

class IDataReceiver {
public:
    virtual ~IDataReceiver() = default;
    virtual bool handlePacket(PacketHeader header, const void* data) = 0;
    virtual void release() = 0;
};

class Host;
class NodeContext;

class TaskContext : public IDataReceiver {
public:
    TaskContext(NodeContext* node) : _node(node) {

    }

    void onTaskStopped() {
        if(_userDataReceiver != nullptr){
            _userDataReceiver->release();
            _userDataReceiver = nullptr;
        }
    }
    void handleNetworkPacket(PacketHeader header, const void* data) {
        std::lock_guard<std::recursive_mutex> lock(_taskMutex);
        if(_userDataReceiver != nullptr){
            _userDataReceiver->handlePacket(header, data);
        }
    }

    bool handlePacket(PacketHeader header, const void* data) override;
    void release() override {
        auto prevCount =_refcount.fetch_sub(1);
        if(prevCount == 1){
            lfDebug() << "Delete TaskContext";
            delete this;
        }
    }
    void setUserDataReceiver(IDataReceiver* userDataReceiver) {
        _userDataReceiver = userDataReceiver;
    }
    void addref() {
        _refcount++;
    }
private:
    std::atomic<std::uint32_t> _refcount = 1;
    std::recursive_mutex _taskMutex;
    IDataReceiver* _userDataReceiver = nullptr;
    NodeContext* _node;
};

class TaskContextConstructor {
public:
    void finalize(bool success){
        _success = success;
        _completionSemaphore.give();
    }
    bool wait() {
        _completionSemaphore.take();
        return _success;
    }
private:
    bool _success = false;
    Threading::BinarySemaphore _completionSemaphore;
};

class NodeContext {
public:
    NodeContext(std::uint8_t realId, std::uint32_t virtualId, Host* host) : _realId(realId), _virtualId(virtualId), _host(host){

    }
    void handleNetworkPacket(PacketHeader header, const void* data) {
        lfDebug() << "Node context received packet: id=" << header.id << " size=" << header.size;

        if(header.id == PacketId::TaskStop){
            if(_currentTask != nullptr){
                _currentTask->onTaskStopped();
                _currentTask->release();
                _currentTask = nullptr;
            }
        }else if(header.id == PacketId::TaskStart){
            std::lock_guard<std::recursive_mutex> lock(_taskMutex);
            if(_nextTask != nullptr){
                _nextTask->finalize(true);
            }
        }else{
            std::lock_guard<std::recursive_mutex> lock(_taskMutex);
            _currentTask->handleNetworkPacket(header, data);
        }
    }
    bool handleUserPacket(PacketHeader header, const void* data);
    std::uint8_t getRealId() const {
        return _realId;
    }
    std::uint32_t getVirtualId() const {
        return _virtualId;
    }

    IDataReceiver* startTask(IDataReceiver* userDataReceiver);

private:
    std::uint8_t _realId;
    std::uint32_t _virtualId;
    Host* _host = nullptr;
    std::recursive_mutex _taskMutex;
    std::shared_ptr<TaskContextConstructor> _nextTask = nullptr;
    TaskContext* _currentTask = nullptr;
};




class Host : public IDataStream {
public:

    bool start() override {
        _txAvailable.give();
        return true;
    }

    IDataReceiver* startTask(std::uint32_t node, IDataReceiver* userDataReceiver){
        if(_node != nullptr){
            return _node->startTask(userDataReceiver);
        }
    }

    std::vector<std::uint32_t> getNodes() {
        std::vector<std::uint32_t> result;
        std::lock_guard<std::mutex> lock(_nodeContextMutex);
        if(_node != nullptr) {
            result.push_back(_node->getVirtualId());
        }
        return result;
    }

    std::uint32_t getState() {
        return _state;
    }
    bool blockingWritePacket(PacketHeader header, const void* data) {
        while(true){
            _txAvailable.take();
            Threading::CriticalSection lock;
            if(freeSpace() >= (sizeof(header) + header.size)){
                write(&header, sizeof(header));
                write(data, header.size);
                return true;
            }
        }
    }
protected:
    std::atomic<std::uint32_t> _state = 0;
    std::mutex _nodeContextMutex;
    NodeContext* _node = nullptr;

    bool readPacket(MaxPacket& packet) {
        LFramework::Threading::CriticalSection lock;
        if(_remote == nullptr){
            return false;
        }
        if(!_remote->peek(&packet.header, sizeof(packet.header))){
            return false;
        }

        auto packetFullSize = sizeof(packet.header) + packet.header.size;

        if(_remote->bytesAvailable() < packetFullSize){
            return false;
        }

        lfAssert(_remote->read(&packet, packetFullSize) == packetFullSize);
        return true;
    }

    void onRemoteReset() override {
        //Send bind packet
        MaxPacket packet;
        packet.header.id = PacketId::Bind;
        packet.header.size = 0;
        write(&packet, sizeof(packet.header));
    }
    void onRemoteDataAvailable() override {
        MaxPacket packet;
        if(readPacket(packet)){
            if(packet.header.id == PacketId::Bind){
                std::lock_guard<std::mutex> lock(_nodeContextMutex);

                lfDebug() << "Bind response received";
                lfAssert(_node == nullptr);
                _node = new NodeContext(0, 0, this);
                lfDebug() << "Node context created";
                _state++;

            }else if(_node != nullptr){
                _node->handleNetworkPacket(packet.header, packet.payload.data());
            }else{
                lfDebug() << "Drop packet";
            }
        }
    }
    void onReadBytes() override {
        _txAvailable.give();
    }

private:

    LFramework::Threading::BinarySemaphore _txAvailable;
};

}
