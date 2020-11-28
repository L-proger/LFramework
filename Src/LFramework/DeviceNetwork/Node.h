#pragma once

#include "Packet.h"
#include "NetworkInterface.h"
#include <LFramework/Debug.h>
#include <LFramework/Assert.h>
#include <LFramework/DeviceNetwork/Node.h>
#include <LFramework/DeviceNetwork/Router.h>
#include <LFramework/DeviceNetwork/Packet.h>
#include <LFramework/DeviceNetwork/TaskManager.h>
#include <LFramework/Containers/ByteFifo.h>
#include <LFramework/DeviceNetwork/NetworkInterface.h>
#include <LFramework/Threading/CriticalSection.h>
#include <LFramework/Threading/Semaphore.h>
#include <LFramework/Threading/Thread.h>
#include "Device/ITaskContext.h"
#include "TaskManager.h"
#include <atomic>

namespace LFramework::DeviceNetwork {

class Node : public IDataStream {
public:
	class TaskContext : public ITaskContext {
	public:
		TaskContext(Node* node):_node(node){

		}
		bool receivePacketFromNetwork(const PacketHeader& header, const void* data){
			Threading::CriticalSection lock;
			if(_rxBuffer.sizeAvailable() < header.fullSize()){
				return false;
			}
			_rxBuffer.write(&header, sizeof(header));
			_rxBuffer.write(data, header.size);
			return true;
		}

		bool packet(PacketHeader header, const void* data) override {
			return _node->writePacket(header, data);
		}

		void readPackets() override {
			while(true){
				Threading::CriticalSection lock;
				if(!readPacket()){
					break;
				}else{
					_currentTask.load()->packet(_packet.header, _packet.payload.data());
				}
			}
		}

		void processTask(Task* task) {
			{
				Threading::CriticalSection lock;
				_rxBuffer.clear();
				_exitRequested = false;
				_currentTask = task;
			}
			_currentTask.load()->run(this);
		}

		void requestExit() {
			{
				Threading::CriticalSection lock;
				_exitRequested = true;
				auto task = _currentTask.load();
				if(task != nullptr){
					//task->
				}
			}
		}

		bool isExitRequested() override {
			return _exitRequested;
		}
	private:
		bool readPacket() {
			if(!_rxBuffer.peek(&_packet.header, sizeof(_packet.header))){
				return false;
			}
			if(_rxBuffer.size() < _packet.header.fullSize()) {
				return false;
			}
			_rxBuffer.read(&_packet, _packet.header.fullSize());
			return true;
		}
		std::atomic<Task*> _currentTask = nullptr;
		bool _exitRequested = false;
		Node* _node;
		MaxPacket _packet;
		ByteFifo<1024> _rxBuffer;
	};



	Node(TaskManager* taskManager) : _taskManager(taskManager){
		_taskContext = new TaskContext(this);
	}

    bool start() override {
        return true;
    }
    void process() {
        while(true){
        	//handle bind
        	auto v = Threading::CriticalSection::lock();
        	_taskTxEnabled = true;
        	if(_bindRequested){
        		lfDebug() << "Bind request detected";
        		_startRequested = false;
				_stopRequested = false;
				_taskTxEnabled = true;

                _txPacket.header.id = PacketId::Bind;
        		_bindRequested = false;
        		Threading::CriticalSection::unlock(v);
        		writePacketBlocking();
        		lfDebug() << "Bind response sent";

        	}else{
        		Threading::CriticalSection::unlock(v);
        	}

        	if(_startRequested && !_bindRequested){
        		_startRequested = false;
        		processTask();
        	}else{
        		Threading::ThisThread::sleepForMs(1);
        	}

        }
    }

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

    void writePacketBlocking() {
    	while(!writePacket(_txPacket.header, _txPacket.payload.data()) && _taskTxEnabled){
    		Threading::ThisThread::sleepForMs(1);
    	}
    }

    bool writePacket(PacketHeader header, const void* data) {
    	auto totalSize = sizeof(header) + header.size;
    	LFramework::Threading::CriticalSection lock;

    	if(!_taskTxEnabled){
    		return false;
    	}

    	if(freeSpace() >= totalSize){
    		write(&header, sizeof(header));
    		write(data, header.size);
    		return true;
    	}
    	return false;
    }


protected:

    void processTask() {
    	lfDebug() << "Creating task";
    	auto task = _taskManager->createTask();
    	lfDebug() << "Sending task start";
        _txPacket.header.id = PacketId::TaskStart;
    	writePacketBlocking();
    	lfDebug() << "Running task";
    	_taskContext->processTask(task);
    	lfDebug() << "Deleting task";
    	_taskManager->deleteTask(task);
        _txPacket.header.id = PacketId::TaskStop;
    	writePacketBlocking();
    	lfDebug() << "Task stopped";
    }

    void onRemoteDataAvailable() override {

    	while(true){
    		if(_remote->peek(&_rxPacket.header, sizeof(_rxPacket.header)) != sizeof(_rxPacket.header)){
    			break;
    		}
    		if(_remote->bytesAvailable() < _rxPacket.header.fullSize()){
    			break;
    		}

    		lfDebug() << "Received packet!";
			if(_rxPacket.header.id == PacketId::TaskStop){
				_remote->discard(_rxPacket.header.fullSize());
				lfDebug() << "Stop task requested";
				_stopRequested = true;

			}else if(_rxPacket.header.id == PacketId::TaskStart){
				_remote->discard(_rxPacket.header.fullSize());
				lfDebug() << "Start task requested";
				_startRequested = true;
			}else{
				_remote->peek(&_rxPacket, _rxPacket.header.fullSize());
				if(_taskContext->receivePacketFromNetwork(_rxPacket.header, _rxPacket.payload.data())){
					_remote->discard(_rxPacket.header.fullSize());
				}
			}
    	}
    }

    void onReadBytes() override {

    }
    void onRemoteReset() override {
    	lfDebug() << "Reset node";
    	Threading::CriticalSection lock;
    	_taskContext->requestExit();
    	_taskTxEnabled = false;
    	_bindRequested = true;
    	clear();
    }
private:
    TaskContext* _taskContext = nullptr;
    MaxPacket _rxPacket;
    MaxPacket _txPacket;
    TaskManager* _taskManager = nullptr;
    std::atomic<bool> _stopRequested = false;
    std::atomic<bool> _startRequested = false;
    std::atomic<bool> _bindRequested = false;
    std::atomic<bool> _taskTxEnabled = false;
};

}
