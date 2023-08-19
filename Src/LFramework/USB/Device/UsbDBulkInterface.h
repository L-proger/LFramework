#pragma once

#include "UsbDInterface.h"
#include <optional>

namespace LFramework::USB {

class UsbDBulkInterface : public UsbDInterface {
public:
	UsbDBulkInterface(){
		_descriptor.bNumEndpoints = 2;
		_descriptor.classDescription = USB::UsbClass::Interface::VendorSpecific<0,0>();
		_descriptor.iInterface = USBD_IDX_INTERFACE_STR;

		USB::EndpointDescriptor desc;
		desc.bEndpointAddress = USB::EndpointAddress::makeIn(1);
		desc.bmAttributes.setType(EndpointType::Bulk);
		desc.wMaxPacketSize = 64;
		_inEp.emplace(desc);

		desc.bEndpointAddress = USB::EndpointAddress::makeOut(1);
		_outEp.emplace(desc);
	}
	bool init(USBD_HandleTypeDef* pdev) override final {
		auto status = UsbDInterface::init(pdev);

		/*if(status &) {
			_initListener->onBulkInterfaceInitComplete();
		}*/

		return status;
	}
	bool deinit(USBD_HandleTypeDef* pdev) override final {
		return UsbDInterface::deinit(pdev);
	}
	UsbEndpoint* getEndpoint(bool isInEndpoint, uint8_t id)override final {
		if(id != 0){
			return nullptr;
		}
		return isInEndpoint ? &_inEp.value() : &_outEp.value();
	}
	const UsbEndpoint* getEndpoint(bool isInEndpoint, uint8_t id) const override final {
		if(id != 0){
			return nullptr;
		}
		return isInEndpoint ? &_inEp.value() : &_outEp.value();
	}

	const USB::InterfaceDescriptor& getInterfaceDescriptor() const override final {
		return _descriptor;
	}

private:
	USB::InterfaceDescriptor _descriptor;
	std::optional<UsbDEndpoint> _inEp;
	std::optional<UsbDEndpoint> _outEp;
};

}
