#ifndef UsbDeviceSoftPhy_h__
#define UsbDeviceSoftPhy_h__

#include <iostream>
#include <vector>
#include <queue>

namespace LFramework {
	namespace USB {

		static const char* StageStrings[] = { "WaitForConnection", "Reset1", "GetDeviceDescriptor", "Reset2" };

		class SoftEndpoint {
		public:
			bool isIn = false;
			bool isOpen = false;
			uint8_t maxPacketSize = 0;
			bool receiving = false;
			std::queue<std::vector<uint8_t>> packets;
		};

		template<typename Device>
		class UsbDeviceSoftPhy {
		public:
			static constexpr uint8_t MaxEndpointsCount = 16;
			static constexpr uint8_t MaxPacketSize = 64;
			

			void connect() {
				_connected = true;
			}

			void start() {
				if (_running) {
					return;
				}

				_running = true;
				setStage(Stage::WaitForConnection);
				_usbThread = std::thread(std::bind(&UsbDeviceSoftPhy::threadHandler, this));
			}

			void stop() {

				if (_running) {
					_running = false;
					_usbThread.join;
				}
			}

			enum class Stage {
				WaitForConnection,
				Reset1,
				GetDeviceDescriptor,
				Reset2,
				StageCount
			};

			

			UsbDeviceSoftPhy() {
				_stageHandlers[static_cast<int>(Stage::WaitForConnection)] = &UsbDeviceSoftPhy::stageWaitForConnection;
				_stageHandlers[static_cast<int>(Stage::Reset1)] = &UsbDeviceSoftPhy::stageReset1;
				_stageHandlers[static_cast<int>(Stage::GetDeviceDescriptor)] = &UsbDeviceSoftPhy::stageGetDeviceDescriptor;
				_stageHandlers[static_cast<int>(Stage::Reset2)] = &UsbDeviceSoftPhy::stageReset2;
			}

			Device* asDevice() {
				return static_cast<Device*>(this);
			}

			static bool isValidPacketSize(uint8_t maxPacketSize, EndpointAddress endpointAddress) {
				return true;
			}

			bool openEndpoint(EndpointAddress address, uint8_t maxPacketSize, EndpointType type) {
				if(!isValidPacketSize(maxPacketSize, address)) {
					std::cout << "Failed to open endpoint with address " << address.value << std::endl;
					return false;
				}

				SoftEndpoint& endpoint = address.isIn() ? _inEndpoints[address.index()] : _outEndpoints[address.index()];
				endpoint.maxPacketSize = maxPacketSize;
				endpoint.isOpen = true;
				std::cout << "Open endpoint with address " << (int)address.value << std::endl;
				return true;
			}

			void closeEndpoint(EndpointAddress address) {
				SoftEndpoint& endpoint = address.isIn() ? _inEndpoints[address.index()] : _outEndpoints[address.index()];
				endpoint.isOpen = false;
				endpoint.packets.clear();
			}

			void beginReceive(EndpointAddress address, void* buffer, uint8_t size) {
				auto& ep = _outEndpoints[address.index()];
				ep.receiving = true;
			}

			void beginTransmit(EndpointAddress address, void* buffer, uint8_t size) {
				auto& ep = _inEndpoints[address.index()];
				size = (std::min)(size, ep.maxPacketSize);
				std::vector<uint8_t> packet;
				if(size != 0) {
					packet.insert(packet.end(), (uint8_t*)buffer, (uint8_t*)buffer + size);
				}
				ep.packets.push(packet);
			}
		private:

			void setStage(Stage stage) {
				std::cout << "Change stage from " << StageStrings[(int)_currentStage] << " to " << StageStrings[(int)stage] << std::endl;
				_currentStage = stage;

				if(_currentStage == Stage::WaitForConnection) {
					for(auto& ep : _inEndpoints) {
						ep.isOpen = false;
						ep.receiving = false;
						ep.packets = {};
					}
					for (auto& ep : _outEndpoints) {
						ep.isOpen = false;
						ep.receiving = false;
						ep.packets = {};
					}
				}
			}

			SoftEndpoint _inEndpoints[MaxEndpointsCount];
			SoftEndpoint _outEndpoints[MaxEndpointsCount];

			Stage _currentStage = Stage::WaitForConnection;

			void stageWaitForConnection() {
				if (_connected) {
					std::cout << "Device connected" << std::endl;
					setStage(Stage::Reset1);
					
				}
			}
			void stageReset1() {
				std::cout << "Device reset1 begin" << std::endl;
				asDevice()->onReset();
				std::cout << "Device reset1 end" << std::endl;
				setStage(Stage::GetDeviceDescriptor);
			}

		

			std::vector<uint8_t> controlIn(SetupPacket setup) {
				/*SetupPacket setup;
				setup.bmRequestType.Direction = RequestDirection::DeviceToHost;
				setup.bmRequestType.Recipient = RequestRecipient::Device;
				setup.bmRequestType.Type = RequestType::Standard;*/

				asDevice()->onSetup(&setup);


				std::vector<uint8_t> rxBuffer;
				auto& ep = _inEndpoints[0];
				for (;;) {
					if(ep.isOpen) {
						if(!ep.packets.empty()) {
							auto packet = ep.packets.front();
							ep.packets.pop();
							if(packet.empty()) {
								return rxBuffer;
							}else {
								rxBuffer.insert(rxBuffer.end(), packet.begin(), packet.end());
							}
						}
						
					}
					Sleep(1);
				}
			}
			void stageGetDeviceDescriptor() {
				SetupPacket setup;
				setup.bmRequestType.Direction = RequestDirection::DeviceToHost;
				setup.bmRequestType.Recipient = RequestRecipient::Device;
				setup.bmRequestType.Type = RequestType::Standard;
				setup.bRequest = SetupRequest::GetDescriptor;
				setup.wIndex = 0;
				setup.wValue = ((uint8_t)DescriptorType::DeviceDescriptor << 8) | 0;
				setup.wLength = sizeof(DeviceDescriptor);

				auto descriptorData = controlIn(setup);

				for (;;);
			}
			void stageReset2() {

			}
			void threadHandler() {
				while (_running) {
					(this->*_stageHandlers[static_cast<int>(_currentStage)])();
					Sleep(1);
				}
			}

			std::array<void(UsbDeviceSoftPhy::*)(), static_cast<int>(Stage::StageCount)> _stageHandlers;

			std::thread _usbThread;
			bool _connected = false;
			bool _running = false;
		};

	}
}

#endif // UsbDeviceSoftPhy_h__
