#pragma once

#include <cstdint>
#include "../BitField.h"
#include <array>

namespace LFramework {
	namespace USB {

#pragma pack(push, 1)

		struct UsbAttributes {
			UsbAttributes(){
				value = 0;
				reservedBusPowered_1_0 = 1; //Reserved, set to 1. (USB 1.0 Bus Powered)
			}
			union {
				uint8_t value;
				struct{
					uint8_t reservedZero : 5;
					uint8_t remoteWakeup : 1;
					uint8_t selfPowered : 1;
					uint8_t reservedBusPowered_1_0 : 1;
				};
			};
		};

		enum class LanguageID : std::uint16_t {
			Invalid = 0x0000, // Invalid LANG ID
			Afrikaans = 0x0436, // Afrikaans
			Albanian = 0x041c, // Albanian
			ArabicSaudiArabia = 0x0401, // Arabic (Saudi Arabia)
			ArabicIrak = 0x0801, // Arabic (Irak)
			ArabicEgypt = 0x0c01, // Arabic (Egypt)
			ArabicLybya = 0x1001, // Arabic (Libya)
			ArabicAlgeria = 0x1401, // Arabic (Algeria)
			ArabicMorocco = 0x1801, // Arabic (Morocco)
			ArabicTunisia = 0x1c01, // Arabic (Tunisia)
			ArabicOman = 0x2001, // Arabic (Oman)
			ArabicYemen = 0x2401, // Arabic (Yemen)
			ArabicSyria = 0x2801, // Arabic (Syria)
			ArabicJordan = 0x2c01, // Arabic (Jordan)
			ArabicLebanon = 0x3001, // Arabic (Lebanon)
			ArabicKuwait = 0x3401, // Arabic (Kuwait)
			ArabicUae = 0x3801, // Arabic (U.A.E.)
			ArabicBahrain = 0x3c01, // Arabic (Bahrain)
			ArabicQatar = 0x4001, // Arabic (Qatar)
			Armenian = 0x042b, // Armenian
			Assamese = 0x044d, // Assamese
			AzeriLatin = 0x042c, // Azeri (Latin)
			AzeriCyrillic = 0x082c, // Azeri (Cyrillic)
			Basque = 0x042d, // Basque
			Belarussian = 0x0423, // Belarussian
			Bengali = 0x0445, // Bengali
			Bulgarian = 0x0402, // Bulgarian
			Burmese = 0x0455, // Burmese
			Catalan = 0x0403, // Catalan
			ChineseTaiwan = 0x0404, // Chinese (Taiwan)
			ChinesePrc = 0x0804, // Chinese (PRC = People Republic of Chinese)
			ChineseHongKong = 0x0c04, // Chinese (Hong Kong)
			ChineseSingapore = 0x1004, // Chinese (Singapore)
			ChineseMacauSar = 0x1404, // Chinese (Macau SAR)
			Croatian = 0x041a, // Croatian
			Czech = 0x0405, // Czech
			Danish = 0x0406, // Danish
			DutchNetherlands = 0x0413, // Dutch (Netherlands)
			DutchBelgium = 0x0813, // Dutch (Belgium)
			EnglishUnitedStates = 0x0409, // English (United States)
			EnglishUnitedKingdom = 0x0809, // English (United Kingdom)
			EnglishAustralian = 0x0c09, // English (Australian)
			EnglishCanadian = 0x1009, // English (Canadian)
			EnglishNewZealand = 0x1409, // English (New Zealand)
			EnglishIreland = 0x1809, // English (Ireland)
			EnglishSouthAfrica = 0x1c09, // English (South Africa)
			EnglishJamaica = 0x2009, // English (Jamaica)
			EnglishCaribbean = 0x2409, // English (Caribbean)
			EnglishBelize = 0x2809, // English (Belize)
			EnglishTrinidad = 0x2c09, // English (Trinidad)
			EnglishZimbabwe = 0x3009, // English (Zimbabwe)
			EnglishPhlippines = 0x3409, // English (Philippines)
			Estonian = 0x0425, // Estonian
			Faeroese = 0x0438, // Faeroese
			Farsi = 0x0429, // Farsi
			Finnish = 0x040b, // Finnish
			FrenchStandard = 0x040c, // French (Standard)
			FrenchBelgian = 0x080c, // French (Belgian)
			FrenchCanadian = 0x0c0c, // French (Canadian)
			FrenchSwitzerland = 0x100c, // French (Switzerland)
			FrenchLuxembourg = 0x140c, // French (Luxembourg)
			FrenchMonaco = 0x180c, // French (Monaco)
			Georgian = 0x0437, // Georgian
			GermanStandard = 0x0407, // German (Standard)
			GermanSwitzerland = 0x0807, // German (Switzerland)
			GermanAustria = 0x0c07, // German (Austria)
			GermanLuxembourg = 0x1007, // German (Luxembourg)
			GermanLiechtenstein = 0x1407, // German (Liechtenstein)
			Greek = 0x0408, // Greek
			Gujarati = 0x0447, // Gujarati
			Hebrew = 0x040d, // Hebrew
			Hindi = 0x0439, // Hindi
			Hungarian = 0x040e, // Hungarian
			Icelandic = 0x040f, // Icelandic
			Indonesian = 0x0421, // Indonesian
			ItalianStandard = 0x0410, // Italian (Standard)
			ItalianSwitzerland = 0x0810, // Italian (Switzerland)
			Japanese = 0x0411, // Japanese
			Kannada = 0x044b, // Kannada
			KashmiriIndia = 0x0860, // Kashmiri (India)
			Kazakh = 0x043f, // Kazakh
			Konkani = 0x0457, // Konkani
			KoreanJohab = 0x0412, // Korean (Johab)
			Latvian = 0x0426, // Latvian
			Lithuanian = 0x0427, // Lithuanian
			LithuanianClassic = 0x0827, // Lithuanian (Classic)
			Macedonian = 0x042f, // Macedonian
			MalayMalaysian = 0x043e, // Malay (Malaysian)
			MalayBruneiDarussalam = 0x083e, // Malay (Brunei Darussalam)
			Malayalam = 0x044c, // Malayalam
			Manipuri = 0x0458, // Manipuri
			Marathi = 0x044e, // Marathi
			NepaliIndia = 0x0861, // Nepali (India)
			NorwegianBokmal = 0x0414, // Norwegian (Bokmal)
			NorwegianNynorsk = 0x0814, // Norwegian (Nynorsk)
			Oriya = 0x0448, // Oriya
			Polish = 0x0415, // Polish
			PortugueseBrazil = 0x0416, // Portuguese (Brazil)
			PortugueseStandard = 0x0816, // Portuguese (Standard)
			Punjabi = 0x0446, // Punjabi
			Romanian = 0x0418, // Romanian
			Russian = 0x0419, // Russian
			Sanskrit = 0x044f, // Sanskrit
			SerbianCyrillic = 0x0c1a, // Serbian (Cyrillic)
			SerbianLatin = 0x081a, // Serbian (Latin)
			Sindhi = 0x0459, // Sindhi
			Slovak = 0x041b, // Slovak
			Slovenian = 0x0424, // Slovenian
			SpannishTraditionalSort = 0x040a, // Spannish (Traditional Sort)
			SpannishMexican = 0x080a, // Spannish (Mexican)
			SpannishModernSort = 0x0c0a, // Spannish (Modern Sort)
			SpannishGuatemala = 0x100a, // Spannish (Guatemala)
			SpannishCostaRica = 0x140a, // Spannish (Costa Rica)
			SpannishPanama = 0x180a, // Spannish (Panama)
			SpannishDominicanRepublic = 0x1c0a, // Spannish (Dominican Republic)
			SpannishVenezuela = 0x200a, // Spannish (Venezuela)
			SpannishColombia = 0x240a, // Spannish (Colombia)
			SpannishPeru = 0x280a, // Spannish (Peru)
			SpannishArgentina = 0x2c0a, // Spannish (Argentina)
			SpannishEcuador = 0x300a, // Spannish (Ecuador)
			SpannishChile = 0x340a, // Spannish (Chile)
			SpannishUruguay = 0x380a, // Spannish (Uruguay)
			SpannishParaguay = 0x3c0a, // Spannish (Paraguay)
			SpannishBolivia = 0x400a, // Spannish (Bolivia)
			SpannishElSalvador = 0x440a, // Spannish (El Salvador)
			SpannishHonduras = 0x480a, // Spannish (Honduras)
			SpannishNicaragua = 0x4c0a, // Spannish (Nicaragua)
			SpannishPuertoRico = 0x500a, // Spannish (Puerto Rico)
			Sutu = 0x0430, // Sutu
			SwahiliKenya = 0x0441, // Swahili (Kenya)
			Swedish = 0x041d, // Swedish
			SwedishFinland = 0x081d, // Swedish (Finland)
			Tamil = 0x0449, // Tamil
			TatarTatarstan = 0x0444, // Tatar (Tatarstan)
			Telugu = 0x044a, // Telugu
			Thai = 0x041e, // Thai
			Turkish = 0x041f, // Turkish
			Ukrainian = 0x0422, // Ukrainian
			UrduPakistan = 0x0420, // Urdu (Pakistan)
			UrduIndia = 0x0820, // Urdu (India)
			UzbekLatin = 0x0443, // Uzbek (Latin)
			UzbekCyrillic = 0x0843, // Uzbek (Cyrillic)
			Vietnamese = 0x042a, // Vietnamese
			HidUdd = 0x04ff, // HID (Usage Data Descriptor)
			Hid1 = 0xf0ff, // HID (Vendor Defined 1)
			Hid2 = 0xf4ff, // HID (Vendor Defined 2)
			Hid3 = 0xf8ff, // HID (Vendor Defined 3)
			Hid4 = 0xfcff  // HID (Vendor Defined 4)
		};

        enum class DescriptorType : uint8_t {
            DeviceDescriptor = 0x01,
            ConfigurationDescriptor = 0x02,
            StringDescriptor = 0x03,
            InterfaceDescriptor = 0x04,
            EndpointDescriptor = 0x05,
            DeviceQualifierDescriptor = 0x06,
            InterfaceAssociationDescriptor = 0x0B,
            ClassSpecificInterface = 0x24,
            ClassSpecificEndpoint = 0x25
        };

		enum class SetupRequest : uint8_t {
			GetStatus = 0x00,
			ClearFeature = 0x01,
			SetFeature = 0x03,
			SetAddress = 0x05,
			GetDescriptor = 0x06,
			SetDescriptor = 0x07,
			GetConfiguration = 0x08,
			SetConfiguration = 0x09,
			GetInterface = 0x0A,
			SetInterface = 0x11,
			SynchFrame = 0x12
		};

		enum class EndpointType : uint8_t {
			Control = 0,
			Isochronous = 1,
			Bulk = 2,
			Interrupt = 3
		};

        enum class EndpointSynchronizationType : uint8_t {
            NoSynchonisation = 0x00,
            Asynchronous = 0x01,
            Adaptive = 0x02,
            Synchronous = 0x03
        };

        enum class EndpointUsageType : uint8_t {
            DataEndpoint = 0x00,
            FeedbackEndpoint = 0x01,
            ExplicitFeedbackDataEndpoint = 0x02
        };


		enum class RequestRecipient : uint8_t {
			Device = 0,
			Interface = 1,
			Endpoint = 2,
			Other = 3
		};

		enum class RequestType : uint8_t {
			Standard = 0,
			Class = 1,
			Vendor = 2,
			Reserved = 3
		};

		enum class RequestDirection : uint8_t {
			HostToDevice = 0,
			DeviceToHost = 1
		};

		struct SetupPacket {
			union {
				uint8_t value;
				BitFieldMember<uint8_t, 0, 5, RequestRecipient> Recipient;
				BitFieldMember<uint8_t, 5, 2, RequestType> Type;
				BitFieldMember<uint8_t, 7, 1, RequestDirection> Direction;
			}bmRequestType;
			SetupRequest bRequest;
			uint16_t wValue;
			uint16_t wIndex;
			uint16_t wLength;
		};


		static_assert(sizeof(SetupPacket) == 8, "Invalid packet size");

#pragma pack(push, 1)
		struct EndpointAddress {
			uint8_t value = 0;

            constexpr EndpointAddress():value{0}{}

			constexpr EndpointAddress(uint8_t address):value{address}{}

			static EndpointAddress makeIn(uint8_t id){
				return EndpointAddress(0x80 | id);
			}

			static EndpointAddress makeOut(uint8_t id){
				return EndpointAddress(id & ~0x80);
			}

            constexpr operator uint8_t(){
                return value;
            }

			void setIn(uint8_t id){
				value = 0x80 | id;
			}
			void setOut(uint8_t id){
				value = id & ~0x80;
			}

			constexpr bool isIn() const{
				return value == 0 ? true : ((value & 0x80) != 0);
			}
			static bool isIn(uint8_t value) {
				return value == 0 ? true : ((value & 0x80) != 0);
			}

			constexpr bool isOut() const {
				return value == 0 ? true : ((value & 0x80) == 0);
			}
			static bool isOut(uint8_t value) {
				return value == 0 ? true : ((value & 0x80) == 0);
			}

			constexpr bool isValid() const {
				return ((value & 0x70) == 0) && 
					((value & 0x7f) == 0 ? ((value & 0x80) == 0) : true);
			}

            static constexpr uint8_t inAddress(uint8_t index){
                return index | 0x80;
            }

            static constexpr uint8_t outAddress(uint8_t index){
                return index & 0x7f;
            }

			constexpr uint8_t index() const {
                return static_cast<uint8_t>(value & 0x7f);
			}
		};
#pragma pack(pop)

		static_assert(EndpointAddress(0x00).isIn()  == true, "Fail");
		static_assert(EndpointAddress(0x00).isOut() == true, "Fail");
		static_assert(EndpointAddress(0x00).isValid() == true, "Fail");

		static_assert(EndpointAddress(0x81).isIn()  == true,  "Fail");
		static_assert(EndpointAddress(0x81).isOut() == false, "Fail");
		static_assert(EndpointAddress(0x81).isValid() == true, "Fail");
		static_assert(EndpointAddress(0x01).isIn()  == false, "Fail");
		static_assert(EndpointAddress(0x01).isOut() == true,  "Fail");
		static_assert(EndpointAddress(0x01).isValid() == true, "Fail");

		static_assert(EndpointAddress(0x70).isValid() == false, "Fail");
		static_assert(EndpointAddress(0xf0).isValid() == false, "Fail");

        //Descriptors
        struct USBDescriptor {
            constexpr USBDescriptor(uint8_t length, DescriptorType type) : bLength(length), bDescriptorType(type) {}
            uint8_t bLength;
            DescriptorType bDescriptorType;
        };

        template<typename Derived>
        struct USBDescriptorInit : USBDescriptor {
            constexpr USBDescriptorInit() : USBDescriptor(sizeof(Derived), Derived::Type) {}
        };

		struct UsbClassDescriptor {
			constexpr UsbClassDescriptor():classId(0), subClassId(0), protocol(0){}
			constexpr UsbClassDescriptor(uint8_t classId, uint8_t subClassId, uint8_t protocol)
			:classId(classId), subClassId(subClassId), protocol(protocol){}
			uint8_t classId = 0;
			uint8_t subClassId = 0;
			uint8_t protocol = 0;
		};

        template<uint8_t ClassId, uint8_t SubClassId, uint8_t Protocol>
        struct UsbClassDescriptorInit : UsbClassDescriptor{
            constexpr UsbClassDescriptorInit() : UsbClassDescriptor(ClassId, SubClassId, Protocol) {}
        };

		namespace UsbClass {
			namespace Device {
                using UseInterfaceClass = UsbClassDescriptorInit<0, 0, 0>;

                template<uint8_t SubClassId, uint8_t Protocol>
                using CDC = UsbClassDescriptorInit<0x02, SubClassId, Protocol>;

                using HubFullSpeed = UsbClassDescriptorInit<0x09, 0x00, 0x00>;
                using HubHiSpeedSingleTT = UsbClassDescriptorInit<0x09, 0x00, 0x01>;
                using HubHiSpeedMultipleTT = UsbClassDescriptorInit<0x09, 0x00, 0x02>;

                using Billboard = UsbClassDescriptorInit<0x11, 0x00, 0x00>;

                template<uint8_t SubClassId, uint8_t Protocol>
                using Diagnostic = UsbClassDescriptorInit<0xDC, SubClassId, Protocol>;



				namespace Miscellaneous {
                    using ActiveSync = UsbClassDescriptorInit<0xEF, 0x01, 0x01>;
                    using PalmSync = UsbClassDescriptorInit<0xEF, 0x01, 0x02>;


                    using InterfaceAssociation = UsbClassDescriptorInit<0xEF, 0x02, 0x01>;
                    using WireAdapterMFProgramming = UsbClassDescriptorInit<0xEF, 0x02, 0x02>;

                    using CableBasedAssociationFramework = UsbClassDescriptorInit<0xEF, 0x03, 0x01>;

                    using RNDISoverEthernet = UsbClassDescriptorInit<0xEF, 0x04, 0x01>;
                    using RNDISoverWiFi = UsbClassDescriptorInit<0xEF, 0x04, 0x02>;
                    using RNDISoverWiMAX = UsbClassDescriptorInit<0xEF, 0x04, 0x03>;
                    using RNDISoverWWAN = UsbClassDescriptorInit<0xEF, 0x04, 0x04>;
                    using RNDISforRawIP4 = UsbClassDescriptorInit<0xEF, 0x04, 0x05>;
                    using RNDISforRawIP6 = UsbClassDescriptorInit<0xEF, 0x04, 0x06>;
                    using RNDISforGPRS = UsbClassDescriptorInit<0xEF, 0x04, 0x07>;

                    using USB3VisionControl = UsbClassDescriptorInit<0xEF, 0x05, 0x00>;
                    using USB3VisionEvent = UsbClassDescriptorInit<0xEF, 0x05, 0x01>;
                    using USB3VisionStreaming = UsbClassDescriptorInit<0xEF, 0x05, 0x02>;

                    using STEP = UsbClassDescriptorInit<0xEF, 0x06, 0x01>;
                    using STEP_RAW = UsbClassDescriptorInit<0xEF, 0x06, 0x02>;

                    using CmdInterfaceInIAD = UsbClassDescriptorInit<0xEF, 0x07, 0x01>;
                    using CmdInterfaceInInterface = UsbClassDescriptorInit<0xEF, 0x07, 0x01>;
                    using MediaInterfaceInInterface = UsbClassDescriptorInit<0xEF, 0x07, 0x02>;
				}

                template<uint8_t SubClassId, uint8_t Protocol>
                using VendorSpecific = UsbClassDescriptorInit<0xFF, SubClassId, Protocol>;

			}
			namespace Interface {
                template<uint8_t SubClassId, uint8_t Protocol>
                using Audio = UsbClassDescriptorInit<0x01, SubClassId, Protocol>;

                template<uint8_t SubClassId, uint8_t Protocol>
                using CDC = Device::CDC<SubClassId, Protocol>;

                template<uint8_t SubClassId, uint8_t Protocol>
                using HID = UsbClassDescriptorInit<0x03, SubClassId, Protocol>;

                template<uint8_t SubClassId, uint8_t Protocol>
                using Physical = UsbClassDescriptorInit<0x05, SubClassId, Protocol>;

                using StillImaging = UsbClassDescriptorInit<0x06, 0x01, 0x01>;

                template<uint8_t SubClassId, uint8_t Protocol>
                using Printer = UsbClassDescriptorInit<0x07, SubClassId, Protocol>;

                template<uint8_t SubClassId, uint8_t Protocol>
                using MassStorage = UsbClassDescriptorInit<0x08, SubClassId, Protocol>;

                template<uint8_t SubClassId, uint8_t Protocol>
                using CDCData = UsbClassDescriptorInit<0x0A, SubClassId, Protocol>;

                template<uint8_t SubClassId, uint8_t Protocol>
                using SmartCard = UsbClassDescriptorInit<0x0B, SubClassId, Protocol>;
                using ContentSecurity = UsbClassDescriptorInit<0x0D, 0x00, 0x00>;
                template<uint8_t SubClassId, uint8_t Protocol>
                using Video = UsbClassDescriptorInit<0x0E, SubClassId, Protocol>;
                template<uint8_t SubClassId, uint8_t Protocol>
                using PersonalHealthcare = UsbClassDescriptorInit<0x0F, SubClassId, Protocol>;

                using AVControl = UsbClassDescriptorInit<0x10, 0x01, 0x00>;
                using AVData0 = UsbClassDescriptorInit<0x10, 0x02, 0x00>;
                using AVData1 = UsbClassDescriptorInit<0x10, 0x03, 0x00>;

                using TypeCBridge = UsbClassDescriptorInit<0x12, 0x00, 0x00>;

                template<uint8_t SubClassId, uint8_t Protocol>
                using Diagnostic = UsbClassDescriptorInit<0xDC, SubClassId, Protocol>;

				namespace WirelessController {
                    using BluetoothProgramming = UsbClassDescriptorInit<0xE0, 0x01, 0x01>;
                    using UWBRadioControl = UsbClassDescriptorInit<0xE0, 0x01, 0x02>;
                    using RemoteNDIS = UsbClassDescriptorInit<0xE0, 0x01, 0x03>;
                    using BluetoothAMPController = UsbClassDescriptorInit<0xE0, 0x01, 0x04>;

                    using HostWireAdapterControlData = UsbClassDescriptorInit<0xE0, 0x02, 0x01>;
                    using DeviceWireAdapterControlData = UsbClassDescriptorInit<0xE0, 0x02, 0x02>;
                    using DeviceWireAdapterIsochronous = UsbClassDescriptorInit<0xE0, 0x02, 0x03>;
				}

				namespace ApplicationSpecific {
                    using DFU = UsbClassDescriptorInit<0xFE, 0x01, 0x01>;
                    using IRDABridge = UsbClassDescriptorInit<0xFE, 0x02, 0x00>;

                    using TestAndMeasurement = UsbClassDescriptorInit<0xFE, 0x03, 0x00>;
                    using TestAndMeasurement_USBTMC_USB488 = UsbClassDescriptorInit<0xFE, 0x03, 0x01>;
				}

				namespace Miscellaneous = UsbClass::Device::Miscellaneous;

                template<uint8_t SubClassId, uint8_t Protocol>
                using VendorSpecific = UsbClassDescriptorInit<0xFF, SubClassId, Protocol>;
			}
		}

		struct UsbVersion {
			using SubMinor = BitFieldMember<uint16_t, 0, 4>;
			using Minor = BitFieldMember<uint16_t, 4, 4>;
			using Major = BitFieldMember<uint16_t, 8, 8>;

			constexpr UsbVersion(const uint8_t major, const uint8_t minor = 0, const uint8_t subMinor = 0)
				:value(SubMinor::assign(Minor::assign(Major::assign(0, major), minor), subMinor)){
			}
			constexpr UsbVersion() : value(0) {}
			constexpr UsbVersion& operator=(const uint16_t value) {
				this->value = value;
				return *this;
			}
			union {
				uint16_t value;
				Major major;
				Minor minor;
				SubMinor subMinor;
			};
		};

	

		template<uint8_t _MajorVersion, uint8_t _MinorVersion, uint8_t _SubMinorVersion>
		struct UsbVersionInit : public UsbVersion
		{
			static constexpr uint8_t MajorVersion = _MajorVersion;
			static constexpr uint8_t MinorVersion = _MinorVersion;
			static constexpr uint8_t SubMinorVersion = _SubMinorVersion;
			constexpr UsbVersionInit() :UsbVersion{ MajorVersion, MinorVersion, SubMinorVersion } {}
		};
		
		template<size_t LanguageCount = 1>
		struct LanguageIDStringDescriptor : public USBDescriptorInit<LanguageIDStringDescriptor<LanguageCount>> {
			static constexpr DescriptorType Type = DescriptorType::StringDescriptor;
			std::array<LanguageID, LanguageCount> languages;
		};

		inline uint8_t* MakeStringDescriptor(const char* ansiString, void* buffer, std::size_t bufferLength, std::uint16_t* outSize){
			auto outPtr = reinterpret_cast<uint8_t*>(buffer);
			uint16_t result = 0U;
			//TODO: check bufferLength overflow
			if (ansiString != NULL){
				outPtr[result++] = 0;
				outPtr[result++] = static_cast<std::uint8_t>(DescriptorType::StringDescriptor);

				while (*ansiString != '\0'){
					outPtr[result++] = *ansiString++;
					outPtr[result++] =  0U;
				}
				outPtr[0] = result;
			}
			*outSize = result;
			return outPtr;
		}

        struct DeviceDescriptor : public USBDescriptorInit<DeviceDescriptor> {
            static constexpr DescriptorType Type = DescriptorType::DeviceDescriptor;
			UsbVersion bcdUSB;
			UsbClassDescriptor classDescription;
			uint8_t bMaxPacketSize0 = 64; //8, 16, 32, 64
			uint16_t idVendor = 0;
			uint16_t idProduct = 0;
			UsbVersion bcdDevice;
			uint8_t iManufacturer = 0;
			uint8_t iProduct = 0;
			uint8_t iSerialNumber = 0;
			uint8_t bNumConfigurations = 0;
		};
		static_assert(sizeof(DeviceDescriptor) == 18, "Invalid packet size");


		class UsbCurrent {
		public:
			UsbCurrent(){
				set(500);
			}
			void set(uint16_t currentMa){
				_value = currentMa >> 1;
			}
			UsbCurrent& operator = (uint16_t value) {
				set(value);
				return *this;
			}
		private:
			std::uint8_t _value;
		};

        struct ConfigurationDescriptor : public USBDescriptorInit<ConfigurationDescriptor> {
            static constexpr DescriptorType Type = DescriptorType::ConfigurationDescriptor;
			uint16_t wTotalLength;
			uint8_t bNumInterfaces;
			uint8_t bConfigurationValue;
			uint8_t iConfiguration;
			uint8_t bmAttributes;
			UsbCurrent bMaxPower;
		};
		static_assert(sizeof(ConfigurationDescriptor) == 9, "Invalid packet size");

        struct InterfaceDescriptor : public USBDescriptorInit<InterfaceDescriptor> {
            static constexpr DescriptorType Type = DescriptorType::InterfaceDescriptor;
			uint8_t bInterfaceNumber;
			uint8_t bAlternateSetting;
			uint8_t bNumEndpoints;
			UsbClassDescriptor classDescription;
			uint8_t iInterface;
		};
		static_assert(sizeof(InterfaceDescriptor) == 9, "Invalid packet size");

        struct EndpointAttributes{
            uint8_t value;

            constexpr EndpointAttributes():value(0){}

            constexpr EndpointAttributes(EndpointType type, EndpointSynchronizationType syncType = EndpointSynchronizationType::NoSynchonisation, EndpointUsageType usageType = EndpointUsageType::DataEndpoint)
                : value(static_cast<uint8_t>(static_cast<uint8_t>(type) | (static_cast<uint8_t>(syncType) << 2) | (static_cast<uint8_t>(usageType) << 4))){

            }

            constexpr EndpointType getType() const {
                return static_cast<EndpointType>(value & 0x03);
            }

            constexpr void setType(EndpointType type) {
                value = (value & ~0x03) | static_cast<uint8_t>(type);
            }

            constexpr EndpointSynchronizationType getSynchronizationType() const {
                return static_cast<EndpointSynchronizationType>((value >> 2) & 0x03);
            }

            constexpr EndpointUsageType getUsageType() const {
                return static_cast<EndpointUsageType>((value >> 4) & 0x03);
            }
        };

        struct EndpointDescriptor : public USBDescriptorInit<EndpointDescriptor> {
            static constexpr DescriptorType Type = DescriptorType::EndpointDescriptor;
            EndpointAddress bEndpointAddress;
            EndpointAttributes bmAttributes;
			uint16_t wMaxPacketSize = 0;
			uint8_t bInterval = 0;
		};
		static_assert(sizeof(EndpointDescriptor) == 7, "Invalid packet size");

        struct DeviceQualifierDescriptor : public USBDescriptorInit<DeviceQualifierDescriptor> {
            static constexpr DescriptorType Type = DescriptorType::DeviceQualifierDescriptor;
            UsbVersion bcdUSB;
			UsbClassDescriptor classDescription;
			uint8_t bMaxPacketSize0;
			uint8_t bNumConfigurations;
			uint8_t bReserved;

			DeviceQualifierDescriptor(UsbVersion bcdUSB, UsbClassDescriptor classDescription, uint8_t bMaxPacketSize0, uint8_t bNumConfigurations, uint8_t bReserved)
			:bcdUSB(bcdUSB), classDescription(classDescription), bMaxPacketSize0(bMaxPacketSize0), bNumConfigurations(bNumConfigurations), bReserved(bReserved)
			{

			}
		};
		//TODO: validate
		static_assert(sizeof(DeviceQualifierDescriptor) == 10, "Invalid packet size");

        struct InterfaceAssociationDescriptor : public USBDescriptorInit<InterfaceAssociationDescriptor> {
            static constexpr DescriptorType Type = DescriptorType::InterfaceAssociationDescriptor;
			uint8_t bFirstInterface;
			uint8_t bInterfaceCount;
			UsbClassDescriptor classDescription;
			uint8_t iFunction;
		};
		//TODO: validate
		static_assert(sizeof(InterfaceAssociationDescriptor) == 8, "Invalid packet size");



#pragma pack(pop)
	}
}
