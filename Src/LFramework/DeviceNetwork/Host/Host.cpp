#include "Host.h"

namespace LFramework::DeviceNetwork::Host {

bool TaskContext::handlePacket(PacketHeader header, const void* data) {
    std::lock_guard<std::recursive_mutex> lock(_taskMutex);
    if(_userDataReceiver != nullptr){
        return _node->handleUserPacket(header, data);
    }else{
        return false;
    }
}

IDataReceiver* NodeContext::startTask(IDataReceiver* userDataReceiver) {
    {
        std::lock_guard<std::recursive_mutex> lock(_taskMutex);
        if((_currentTask != nullptr) || (_nextTask != nullptr)){
            return nullptr;
        }
        _nextTask = std::make_shared<TaskContextConstructor>();
    }

    MaxPacket packet;
    packet.header.id = PacketId::TaskStart;
    packet.header.size = 0;
    _host->blockingWritePacket(packet.header, packet.payload.data());

    bool started = _nextTask->wait();

    std::lock_guard<std::recursive_mutex> lock(_taskMutex);
    _nextTask = nullptr;
    if(!started) {
        return nullptr;
    }else{
        _currentTask = new TaskContext(this);
        _currentTask->setUserDataReceiver(userDataReceiver);
        _currentTask->addref(); //User ref
         return _currentTask;
    }
}


bool NodeContext::handleUserPacket(PacketHeader header, const void* data) {
    return _host->blockingWritePacket(header, data);
}


}
