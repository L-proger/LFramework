#pragma once

#include <LFramework/USB/Device/UsbDBulkInterface.h>
#include "../NetworkInterface.h"
#include <LFramework/Threading/CriticalSection.h>
#include <atomic>

namespace LFramework::DeviceNetwork::Device {

class UsbTransmitter : public IDataStream {
public:
	static constexpr std::size_t MaxPacketSize = 64;

	UsbTransmitter(USB::UsbDBulkInterface* usbInterface) : _usbInterface(usbInterface) {
		_rxTransfer.buffer = _rxBuffer;
		_rxTransfer.size = MaxPacketSize;
		_rxTransfer.callback = &onReceivedPacket_;
		_rxTransfer.context = this;

		_txTransfer.buffer = _txBuffer;
		_txTransfer.callback = &onTramsmittedPacket_;
		_txTransfer.context = this;
	}

	bool start() override {
		_rxEndpoint = reinterpret_cast<USB::UsbDEndpoint*>(_usbInterface->getEndpoint(false, 0));
		_txEndpoint = reinterpret_cast<USB::UsbDEndpoint*>(_usbInterface->getEndpoint(true, 0));
		return (_rxEndpoint != nullptr) && (_txEndpoint != nullptr) && startRxTransfer();
	}

protected:
	  void onRemoteDataAvailable() override {
		  Threading::CriticalSection lock;
		  if(!_isTransmitting){
			  startTxTransfer();
		  }

	  }
	  void onReadBytes() override {
		  Threading::CriticalSection lock;
		  if(freeSpace() >= MaxPacketSize){
			  startRxTransfer();
		  }
	  }
	  void onRemoteReset() override {
		  for(;;);
	  }
private:

	void onReceivedPacket(USB::UsbTransfer * transfer, bool isOk) {
		//lfDebug() << "USB rx";
		if(transfer->actualSize == 0) {
			reset();
			if(!_isTransmitting) {
				_synchronized = sendSyncPacket();
			} else {
				_synchronized = false;
			}
		} else {
			if(_synchronized) {
				if(write(reinterpret_cast<const uint8_t*>(transfer->buffer), transfer->actualSize) != transfer->actualSize){
					for(;;);
				}
			}
		}
		startRxTransfer();
	}

	bool sendSyncPacket() {
		_txTransfer.size = 0;
		_isTransmitting = _txEndpoint->transferAsync(&_txTransfer);
		return _isTransmitting;
	}

	bool startTxTransfer() {
			Threading::CriticalSection lock;


			auto txSize = _remote->read(_txBuffer, sizeof(_txBuffer));
			if(txSize != 0) {
				_txTransfer.size = txSize;
				_isTransmitting = true;
				auto status = _txEndpoint->transferAsync(&_txTransfer);
				if(!status){
					lfDebug() << "Assert: failed to start TX transfer";
					for(;;);
				}

				return true;
			}
			return false;
		}


	void onTramsmittedPacket(USB::UsbTransfer* transfer, bool isOk) {
		//lfDebug() << "USB tx";
		Threading::CriticalSection lock;
		if(!_synchronized) {
			_synchronized = sendSyncPacket();
		} else {
			_isTransmitting = startTxTransfer();
		}

	}
	bool startRxTransfer() {
		bool result = _rxEndpoint->transferAsync(&_rxTransfer);
		//lfDebug() << "startRxTransfer: " << result;
		return result;
	}


	static void onReceivedPacket_(USB::UsbTransfer * transfer, bool isOk) {
		auto _this = reinterpret_cast<UsbTransmitter *>(transfer->context);
		_this->onReceivedPacket(transfer, isOk);
	}

	static void onTramsmittedPacket_(USB::UsbTransfer * transfer, bool isOk) {
		auto _this = reinterpret_cast<UsbTransmitter *>(transfer->context);
		_this->onTramsmittedPacket(transfer, isOk);
	}

	bool _synchronized = false;
	std::atomic<bool> _isTransmitting = false;

	uint8_t _rxBuffer[MaxPacketSize];
	uint8_t _txBuffer[MaxPacketSize];

	USB::UsbTransfer _rxTransfer;
	USB::UsbTransfer _txTransfer;

	USB::UsbDEndpoint* _rxEndpoint = nullptr;
	USB::UsbDEndpoint* _txEndpoint = nullptr;

	USB::UsbDBulkInterface* _usbInterface = nullptr;
};

}

