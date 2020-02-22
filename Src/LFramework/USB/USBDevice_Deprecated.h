/*
 * USBDevice.h
 *
 *  Created on: 29 Aug. 2017 г.
 *      Author: l-pro
 */

#ifndef LFRAMEWORK_MCU_USBDEVICE_USBDEVICE_H_
#define LFRAMEWORK_MCU_USBDEVICE_USBDEVICE_H_

#include "LFrameworkConfig.h"
#include <type_traits>

#include "../System.h"
#include <cstdint>
#include <array>
#include <tuple>
#include "../TupleUtils.h"
#include "USBTypes.h"

namespace LFramework {
namespace USB {

enum class UsbDeviceState {
	Stopped,
	ResetComplete,
	Suspended,
	Configured,
	Addressed
};

enum class UsbEndpoint0Stage {
	Idle,
	Setup,
	DataIn,
	DataOut,
	StatusIn,
	StatusOut,
	Error
};


template<typename T>
constexpr std::false_type HasChildDescriptorHelper(...) {
    return {};
};

template<typename T, typename = typename T::Descriptor>
constexpr std::true_type HasChildDescriptorHelper(const T& ) {
    return {};
};

template<typename T>
constexpr bool HasChildDescriptor = std::is_same_v<std::true_type, decltype(HasChildDescriptorHelper<T>(std::declval<T>()))>;


struct DescriptorSelector {
    template<typename T>
    auto operator () (const T& t) {
        if constexpr (HasChildDescriptor<T>) {
            return typename std::remove_reference_t<std::remove_cv_t<decltype(t)>>::Descriptor{};
        }
        else {
            return TupleSkipType{};
        }
    }
};

struct UsbEndpointState {
	uint32_t dataLength = 0;
	uint32_t dataRemainingLength = 0;
};

template<uint8_t Address, uint16_t MaxPacketSize, uint8_t PollingInterval, EndpointType type, EndpointSynchronizationType syncType, EndpointUsageType usageType>
class UsbEndpoint{
public:
	struct DescriptorEx : USB::EndpointDescriptor {
		constexpr DescriptorEx() {
			bEndpointAddress = Address;
			wMaxPacketSize = MaxPacketSize;
			bInterval = PollingInterval;
			bmAttributes = EndpointAttributes(type, syncType, usageType);
		}
	};

	using Descriptor = DescriptorEx;
};

template<uint8_t Index, uint16_t MaxPacketSize, uint8_t PollingInterval, EndpointType type, EndpointSynchronizationType syncType, EndpointUsageType usageType>
class UsbInEndpoint : public UsbEndpoint<EndpointAddress::inAddress(Index), MaxPacketSize, PollingInterval, type, syncType, usageType>{};

template<uint8_t Index, uint16_t MaxPacketSize, uint8_t PollingInterval, EndpointType type, EndpointSynchronizationType syncType, EndpointUsageType usageType>
class UsbOutEndpoint : public UsbEndpoint<EndpointAddress::outAddress(Index), MaxPacketSize, PollingInterval, type, syncType, usageType>{};

template<uint8_t Index, uint16_t MaxPacketSize, uint8_t PollingInterval = 0>
class UsbBulkInEndpoint : public UsbInEndpoint<EndpointAddress::inAddress(Index), MaxPacketSize, PollingInterval, USB::EndpointType::Bulk, USB::EndpointSynchronizationType::NoSynchonisation, USB::EndpointUsageType::DataEndpoint>{};

template<uint8_t Index, uint16_t MaxPacketSize, uint8_t PollingInterval = 0>
class UsbBulkOutEndpoint : public UsbOutEndpoint<EndpointAddress::inAddress(Index), MaxPacketSize, PollingInterval, USB::EndpointType::Bulk, USB::EndpointSynchronizationType::NoSynchonisation, USB::EndpointUsageType::DataEndpoint>{};

template<uint8_t Index, uint16_t MaxPacketSize, uint8_t PollingInterval, EndpointSynchronizationType syncType, EndpointUsageType usageType>
class UsbIsoInEndpoint : public UsbInEndpoint<EndpointAddress::inAddress(Index), MaxPacketSize, PollingInterval, USB::EndpointType::Isochronous, syncType, usageType>{};

template<uint8_t Index, uint16_t MaxPacketSize, uint8_t PollingInterval, EndpointSynchronizationType syncType, EndpointUsageType usageType>
class UsbIsoOutEndpoint : public UsbOutEndpoint<EndpointAddress::inAddress(Index), MaxPacketSize, PollingInterval, USB::EndpointType::Isochronous, syncType, usageType>{};

template<uint8_t Index, uint16_t MaxPacketSize, uint8_t PollingInterval>
class UsbInterruptInEndpoint : public UsbInEndpoint<EndpointAddress::inAddress(Index), MaxPacketSize, PollingInterval, USB::EndpointType::Interrupt, USB::EndpointSynchronizationType::NoSynchonisation, USB::EndpointUsageType::DataEndpoint>{};

template<uint8_t Index, uint16_t MaxPacketSize, uint8_t PollingInterval>
class UsbInterruptOutEndpoint : public UsbOutEndpoint<EndpointAddress::inAddress(Index), MaxPacketSize, PollingInterval, USB::EndpointType::Interrupt, USB::EndpointSynchronizationType::NoSynchonisation, USB::EndpointUsageType::DataEndpoint>{};

template<uint8_t bInterfaceNumber, uint8_t indexOfStringDescriptor, typename classDescriptor, typename ... Endpoints>
class UsbInterface {
public:
	static constexpr uint32_t getEndpointsCount() {
		return sizeof...(Endpoints);
	}

	struct InterfaceDescriptorEx : USB::InterfaceDescriptor {
		constexpr InterfaceDescriptorEx() {
			bAlternateSetting = 0; //TODO: handle alternate interfaces
			bNumEndpoints = getEndpointsCount();
			this->bInterfaceNumber = bInterfaceNumber;
			iInterface = indexOfStringDescriptor;
			classDescription = classDescriptor{};
		}
	};

	using epDesc = tupleSelectType<DescriptorSelector, Tuple<Endpoints...>>;

	using Descriptor = Tuple<InterfaceDescriptorEx, epDesc>;
};

template<typename ... Interfaces>
class InterfaceAssociation {
public:

};

template<typename Interface>
class UsbFunction {
public:
    using Descriptor = typename Interface::Descriptor;
    static constexpr size_t interfacesCount = 1;
};

template<typename...Interfaces>
class UsbFunction<USB::InterfaceAssociation<Interfaces...>> {
public:
    using Descriptor = float;
    static constexpr size_t interfacesCount = sizeof...(Interfaces);
};



template<typename ... Functions>
class UsbConfiguration {
public:
	static constexpr uint32_t FunctionsCount = sizeof...(Functions);
    Tuple<Functions...> _functions;

    void onEndpointTransferComplete(uint8_t epId, size_t size) {

    }

    using Descriptor = tupleSelectType<DescriptorSelector, Tuple<Functions...>>;

    USBDescriptor* getDescriptor(){
        return reinterpret_cast<USBDescriptor*>(&_descriptor);
    }
	/*static constexpr uint32_t getConfigurationDescriptorSize() {
		return (Interfaces::getDescriptorSize() + ...);
	}*/
private:
    Descriptor _descriptor;
	//std::array<uint8_t, getConfigurationDescriptorSize()> _configurationDescriptor;
};





template<typename T>
struct IsUsbInterface : std::false_type{};

template<uint8_t bInterfaceNumber, uint8_t indexOfStringDescriptor, typename classDescriptor, typename... T>
struct IsUsbInterface<UsbInterface<bInterfaceNumber, indexOfStringDescriptor, classDescriptor, T...>> : std::true_type {};

template<typename T>
struct IsUsbConfiguration : std::false_type{};

template<typename ... Functions>
struct IsUsbConfiguration<UsbConfiguration<Functions...>> : std::true_type {};


/*template<typename ... Interfaces>
class AlternateInterfaces {
public:
	static constexpr uint32_t getDescriptorSize() {
		return (Interfaces::getDescriptorSize() + ...);
	}
};


using GenericInterface = UsbInterface<UsbEndpoint<0x01>, UsbEndpoint<0x81>>;
using GenericInterfaceA = AlternateInterfaces<UsbInterface<UsbEndpoint<0x01>, UsbEndpoint<0x81>>>;
using GenericConfig = UsbConfiguration<GenericInterface>;
using GenericConfigA = UsbConfiguration<GenericInterfaceA>;

static_assert(GenericConfig::getConfigurationDescriptorSize() == EndpointDescriptor::Size * 2 + InterfaceDescriptor::Size, "WTF");
static_assert(GenericConfigA::getConfigurationDescriptorSize() == EndpointDescriptor::Size * 2 + InterfaceDescriptor::Size, "WTF");


static_assert(IsUsbInterface<UsbInterface<UsbEndpoint<0x01>, UsbEndpoint<0x81>>>::value, "Fail");
*/


template<class UsbPhy, typename ... Configurations>
class UsbDevice : public UsbPhy {
public:
	DeviceDescriptor descriptor;
    using Phy = UsbPhy;

    using ConfigurationsList = Tuple<Configurations...>;

	static constexpr size_t ConfigurationsCount = sizeof...(Configurations);

    uint8_t _currentConfig = 0x0;



    std::tuple<Configurations...> _configurations;

	//static_assert((IsUsbConfiguration<Configurations>::value && ...) , "Only UsbConfiguration types allowed");

	template<size_t ID>
    auto& getConfiguration(){
		return std::get<ID>(_configurations);
	}

    UsbDevice(){
		//usb = this;
	}

/*	void connect(){
		HAL_PCD_DevConnect(_pcd);
	}

	void disconnect(){
		HAL_PCD_DevDisconnect(_pcd);
	}*/

	/*void initPHY(){

		_pcd->Instance = USB_OTG_FS;
		_pcd->Init.dev_endpoints = 1;
		_pcd->Init.speed = PCD_SPEED_FULL;
		_pcd->Init.dma_enable = DISABLE;
		_pcd->Init.Sof_enable = DISABLE;
		_pcd->Init.ep0_mps = 64;
		_pcd->Init.phy_itface = PCD_PHY_EMBEDDED;
		_pcd->Init.low_power_enable = DISABLE;
		_pcd->Init.lpm_enable = DISABLE;
		_pcd->Init.vbus_sensing_enable = DISABLE;
		_pcd->Init.use_dedicated_ep1 = DISABLE;
		//_pcd->Init.battery_charging_enable = DISABLE;
		HAL_PCD_Init(_pcd);

		//HAL_PCD_SetRxFiFo(&USBD_PCD, 0x80);
		//HAL_PCD_SetTxFiFo(&USBD_PCD, 0, 0x20);
		//HAL_PCD_SetTxFiFo(&USBD_PCD, 1, 0x40);
	
		HAL_PCD_SetRxFiFo(_pcd, 0x80);
		HAL_PCD_SetTxFiFo(_pcd, 0, 0x40);
		HAL_PCD_SetTxFiFo(_pcd, 1, 0x80);
	}*/

	/*void start(){
		HAL_PCD_Start(_pcd);
	}*/

    template<typename Functor>
    void executeOnCurrentConfig(Functor f){
        executeOnIndex(_configurations, _currentConfig, f);
    }

    void onEndpointTransferComplete(uint8_t epId, size_t size) override {
        executeOnCurrentConfig([epId, size](auto& config){config.onEndpointTransferComplete(epId, size);});
    }


	void onReset(){
		Phy::openEndpoint(0x00, 64, EndpointType::Control);
		Phy::openEndpoint(0x80, 64, EndpointType::Control);
		//open ep0
		//deinit previous user config if any...

		//Control endpoint init
		//TODO: move implementation in abstract PHY code
		/*HAL_PCD_EP_Open(_pcd, 0x00, 64, (uint8_t)EndpointType::Control);
		HAL_PCD_EP_Open(_pcd, 0x80, 64, (uint8_t)EndpointType::Control);

		setState(UsbDeviceState::ResetComplete);*/

		//TODO: close previous config

		setState(UsbDeviceState::ResetComplete);
	}

	void onSuspend(){
		/* __HAL_PCD_GATE_PHYCLOCK(_pcd);

		_stateBeforeSuspend = _state;
		setState(UsbDeviceState::Suspended);

		//Suspend whole MCU device
		if (_pcd->Init.low_power_enable){
			//TODO: maybe move somewhere else (to user code?)
			SCB->SCR |= (uint32_t)(SCB_SCR_SLEEPDEEP_Msk | SCB_SCR_SLEEPONEXIT_Msk);
		}*/
	}

	void onResume(){
		setState(_stateBeforeSuspend);
	}

    const USBDescriptor* getDescriptor(USB::DescriptorType type, uint8_t id) override{
		switch(type){
        case USB::DescriptorType::DeviceDescriptor:
			return &descriptor;
			break;
        case USB::DescriptorType::ConfigurationDescriptor:
            if(id < ConfigurationsCount){
                USBDescriptor* result = nullptr;
                executeOnCurrentConfig([&result](auto& config){result = config.getDescriptor();});
                return result;
            }
            break;
		}

		return nullptr;
	}

	void stdRequestGetDescriptor(const SetupPacket* setup){
        auto desc = getDescriptor(static_cast<USB::DescriptorType>(setup->wValue >> 8), setup->wValue & 0xff);

		if(desc == nullptr){
			System::setBreakpoint();
		}

		if(setup->wLength != 0){
			auto len = desc->bLength;
			len = std::min<uint8_t>(len, (uint8_t)setup->wLength);
			controlSendData((uint8_t*)desc, len);
		}
	}

	void stdRequestSetAddress(const SetupPacket* setup){
		if(_state == UsbDeviceState::Configured){
			controlError();
		}else {
			//send status before set address TODO: maybe handle in some different manner (wait for status really sent??)
			controlSendStatus();
			auto address = setup->wValue & 0x7f;
			
			//HAL_PCD_SetAddress(_pcd, address);

			_state = address == 0 ? UsbDeviceState::Stopped : UsbDeviceState::Addressed;
		}
	}

    void setConfiguration(uint8_t id){
        if(_currentConfig != id){
            if(_currentConfig != 0xff){
                executeOnCurrentConfig([](auto& config){
                    //config.deinit();
                });
            }

            _currentConfig = id;

            if(_currentConfig != 0xff){
                executeOnCurrentConfig([](auto& config){
                    //config.init();
                });
            }
        }
    }

	bool stdDeviceRequest(const SetupPacket* setup){
		switch(setup->bRequest){
		case SetupRequest::GetDescriptor:
			stdRequestGetDescriptor(setup);
			break;
		case SetupRequest::SetAddress:
			stdRequestSetAddress(setup);
			break;
		default:
			System::setBreakpoint();
			break;
		}
		return true;
	}

    void onSetup(const SetupPacket* header) override{
		//SetupPacket setupHeader;
		//setupHeader = *((SetupPacket*)(&_pcd->Setup[0]));
		_ep0Stage = UsbEndpoint0Stage::Setup;
		_ep0DataSize = header->wLength;

		switch(header->bmRequestType.Recipient){
		case RequestRecipient::Device:
			stdDeviceRequest(header);
			break;
		case RequestRecipient::Interface:
			System::setBreakpoint();
			break;
		case RequestRecipient::Endpoint:
			System::setBreakpoint();
			break;
		default:
			System::setBreakpoint();
			break;
		}

		if(_ep0Stage == UsbEndpoint0Stage::Setup){
			if(header->bmRequestType.Direction == RequestDirection::HostToDevice){
				controlSendStatus();
			}else {
				controlReceiveStatus();
			}
		}
	}

	void onDataOut(uint8_t endpointNumber, uint8_t size){
		if(endpointNumber == 0){
			if(_ep0Stage == UsbEndpoint0Stage::DataOut){
				System::setBreakpoint();
			}
		}else {
			System::setBreakpoint();
		}
	}

	void onDataIn(uint8_t endpointNumber){
		if(endpointNumber == 0){
			if(_ep0Stage == UsbEndpoint0Stage::DataIn){
				//send next packet
				/*auto& ep = _inEndpoints[0];
				if(ep.dataRemainingLength > _pcd->IN_ep[0].maxpacket){
					ep.dataRemainingLength -= _pcd->IN_ep[0].maxpacket;
					HAL_PCD_EP_Transmit(_pcd, 0, _pcd->IN_ep[0].xfer_buff, ep.dataRemainingLength);

				}else{ //handle last packet
					if((_ep0DataSize != 0) && ((ep.dataLength % _pcd->IN_ep[0].maxpacket) == 0) && (ep.dataLength != _ep0DataSize)){
						_ep0DataSize = 0;
						HAL_PCD_EP_Transmit(_pcd, 0, nullptr, 0);
					}else{
						_ep0DataSize = 0;
						//event: data in stage complete
						controlReceiveStatus();
					}
				}*/
			}
		}else {
			System::setBreakpoint();
		}
	}

	void controlReceiveStatus(){
		_ep0Stage = UsbEndpoint0Stage::StatusOut;
		Phy::beginReceive(0x00, nullptr, 0);
	}

	void controlSendStatus(){
		_ep0Stage = UsbEndpoint0Stage::StatusIn;
		Phy::beginTransmit(0x00, nullptr, 0);
		//HAL_PCD_EP_Transmit(_pcd, 0, nullptr, 0);
	}

	void controlSendData(uint8_t* data, uint32_t size){
		_ep0Stage = UsbEndpoint0Stage::DataIn;
		_inEndpoints[0].dataLength = size;
		_inEndpoints[0].dataRemainingLength = size;
		Phy::beginTransmit(0x80, data, size);

		//HAL_PCD_EP_Transmit(_pcd, 0, data, size);
	}

	void controlError(){
		_ep0Stage = UsbEndpoint0Stage::Error;
		//HAL_PCD_EP_SetStall(_pcd, 0x80);
		//HAL_PCD_EP_SetStall(_pcd, 0x00);
	}


	void onSOF(){System::setBreakpoint();}
	void onIsochronousOutIncomplete(uint8_t endpointNumber){System::setBreakpoint();}
	void onIsochronousInIncomplete(uint8_t endpointNumber){System::setBreakpoint();}
	void onConnect(){System::setBreakpoint();}
	void onDisconnect(){System::setBreakpoint();}

protected:

	void setState(UsbDeviceState state){
		_state = state;
	}
	UsbDeviceState _stateBeforeSuspend = UsbDeviceState::Stopped;
	UsbDeviceState _state = UsbDeviceState::Stopped;
	//PCD_HandleTypeDef* _pcd;

	UsbEndpoint0Stage _ep0Stage = UsbEndpoint0Stage::Idle;
	uint32_t _ep0DataSize = 0;

	UsbEndpointState _inEndpoints[15];
	UsbEndpointState _outEndpoints[15];
};

}}

#endif /* LFRAMEWORK_MCU_USBDEVICE_USBDEVICE_H_ */
