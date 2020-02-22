#pragma once

#include <cstdint>
#include "USBTypes.h"

namespace LFramework::USB::Microsoft {

	namespace Os20{
		enum class DescriptorType : std::uint16_t {
			HeaderDescriptor = 0x00,
			HeaderConfiguration = 0x01,
			HeaderFunction = 0x02,
			FeatureCompatibleId = 0x03,
			FeatureRegProperty = 0x04,
			FeatureMinResumeTime = 0x05,
			FeatureModelId = 0x06,
			FeatureCcgpDevice = 0x07,
			FeatureVendorRevision = 0x08
		};

		enum class PropertyDataType : uint32_t{
			String 						= 1, //A NULL-terminated Unicode String (REG_SZ)
			EnvironmentVariablesString 	= 2, //A NULL-terminated Unicode String that includes environment variables
			Binary 						= 3, //Free-form binary (REG_BINARY)
			Int32LittleEndian 			= 4, //A little-endian 32-bit integer
			Int32BigEndian 				= 5, //A big-endian 32-bit integer
			SymLinkString 				= 6, //A NULL-terminated Unicode string that contains a symbolic link
			StringArray 				= 7  //Multiple NULL-terminated Unicode strings
		};
	}



#pragma pack(push, 1)

	//"Entry point" descriptor for microsoft-compatible devices, should be accessible at index 0xEE
	struct MicrosoftStringDescriptor : public USBDescriptor {
		MicrosoftStringDescriptor(uint8_t bMS_VendorCode):
			USBDescriptor(sizeof(MicrosoftStringDescriptor), DescriptorType::StringDescriptor),
			qwSignature{
				'M', 0x00,
				'S', 0x00,
				'F', 0x00,
				'T', 0x00,
				'1', 0x00,
				'0', 0x00,
				'0', 0x00
			},
			bMS_VendorCode(bMS_VendorCode){
		}
		uint8_t qwSignature[14];
		uint8_t bMS_VendorCode;
		uint8_t bPad = 0;
	};
	static_assert(sizeof(MicrosoftStringDescriptor) == 18, "Invalid packet size");


	namespace CompatId {
		struct Header { //Header for MSOS feature descriptors

			uint32_t dwLength;
			uint16_t bcdVersion = 0x0100;
			uint16_t wIndex = 0x0004; //Extended compat ID descriptor identifier (0x0004)
			uint8_t bCount; //number of function sections that follow
			std::array<uint8_t, 7> RESERVED = {};
		};
		//TODO: validate
		static_assert(sizeof(Header) == 16, "Invalid packet size");

		struct Function { //Function descriptor section
			uint8_t bFirstInterfaceNumber = {};
			uint8_t RESERVED0 = {};
			std::array<uint8_t, 8> compatibleID = {};
			std::array<uint8_t, 8> subCompatibleID = {};
			std::array<uint8_t, 6> RESERVED1 = {};
		};
		//TODO: validate
		static_assert(sizeof(Function) == 24, "Invalid packet size");

		struct WinUSBFunction {
			Header header;
			Function function;

			WinUSBFunction(uint8_t firstInterfaceNumber){
				header.dwLength = sizeof(WinUSBFunction);
				header.bCount = 1;
				function.bFirstInterfaceNumber = firstInterfaceNumber;
				function.compatibleID = {0x57, 0x49, 0x4E, 0x55, 0x53, 0x42, 0x00, 0x00}; //WINUSB
			}
		};
	}

	namespace Property{
		struct Header {
			uint32_t dwLength;
			uint16_t bcdVersion = 0x0100;
			uint16_t wIndex = 0x0005; //extended properties OS feature descriptor
			uint16_t wCount;
		};
		//TODO: validate
		static_assert(sizeof(Header) == 10, "Invalid packet size");

		template<typename Name, typename Data>
		struct Section{
			uint32_t dwSize = sizeof(Section);
			Os20::PropertyDataType dwPropertyDataType;
			uint16_t wPropertyNameLength = sizeof(Name);
			Name bPropertyName;
			uint32_t dwPropertyDataLength = sizeof(Data);
			Data bPropertyData;
		};


		static uint32_t ansiToWide(const char* src, char* dst, uint32_t dst_size){
			uint32_t dst_ready = 0;
			while((*src) != '\0' && ((dst_size - dst_ready) >= 2)){
				dst[dst_ready++] = *src;
				dst[dst_ready++] = '\0';
				src++;
			}
			if((dst_size - dst_ready) >= 2){
				dst[dst_ready++] = '\0';
				dst[dst_ready++] = '\0';
			}
			return dst_ready;
		}

		struct InterfaceGuid {
			Header header;
			Section<std::array<uint8_t, 40>, std::array<uint8_t, 78>> guidSection;

			InterfaceGuid(){
				header.dwLength = sizeof(InterfaceGuid);
				header.wCount = 1;

				guidSection.dwPropertyDataType = Os20::PropertyDataType::String;
				ansiToWide("DeviceInterfaceGUID", (char*)guidSection.bPropertyName.data(), sizeof(guidSection.bPropertyName));
				ansiToWide("{EEEEEEE3-50F3-4888-84B4-74E50E1649DC}", (char*)guidSection.bPropertyData.data(), sizeof(guidSection.bPropertyData));
			}
		};
	}

#pragma pack(pop)

}

