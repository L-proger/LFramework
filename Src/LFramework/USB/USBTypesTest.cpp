#ifndef _WIN32

#include "USBTypes.h"
#include "../UnitTest/UnitTest.h"

using namespace LFramework;
using namespace LFramework::USB;

TEST(USB_Setup_bmRequestType) {
	SetupPacket setup;

	setup.bmRequestType.Direction = RequestDirection::HostToDevice;
	TEST_TRUE(setup.bmRequestType.Direction == RequestDirection::HostToDevice);
	setup.bmRequestType.Direction = RequestDirection::DeviceToHost;
	TEST_TRUE(setup.bmRequestType.Direction == RequestDirection::DeviceToHost);

	setup.bmRequestType.Type = RequestType::Standard;
	TEST_TRUE(setup.bmRequestType.Type == RequestType::Standard);
	setup.bmRequestType.Type = RequestType::Class;
	TEST_TRUE(setup.bmRequestType.Type == RequestType::Class);
	setup.bmRequestType.Type = RequestType::Vendor;
	TEST_TRUE(setup.bmRequestType.Type == RequestType::Vendor);
	setup.bmRequestType.Type = RequestType::Reserved;
	TEST_TRUE(setup.bmRequestType.Type == RequestType::Reserved);

	setup.bmRequestType.Recipient = RequestRecipient::Device;
	TEST_TRUE(setup.bmRequestType.Recipient == RequestRecipient::Device);
	setup.bmRequestType.Recipient = RequestRecipient::Interface;
	TEST_TRUE(setup.bmRequestType.Recipient == RequestRecipient::Interface);
	setup.bmRequestType.Recipient = RequestRecipient::Endpoint;
	TEST_TRUE(setup.bmRequestType.Recipient == RequestRecipient::Endpoint);
	setup.bmRequestType.Recipient = RequestRecipient::Other;
	TEST_TRUE(setup.bmRequestType.Recipient == RequestRecipient::Other);
}
#endif
