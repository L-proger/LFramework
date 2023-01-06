#pragma once

#include <sys/ioctl.h>
#include <cstdint>
#include <type_traits>

#define IOCTL_USBFS_RESETEP	_IOR('U', 3, unsigned int)
#define IOCTL_USBFS_SETCONFIG	_IOR('U', 5, unsigned int)
#define IOCTL_USBFS_SUBMITURB	_IOR('U', 10, struct UsbUrb)
#define IOCTL_USBFS_DISCARDURB	_IO('U', 11)
#define IOCTL_USBFS_REAPURB	_IOW('U', 12, void *)
#define IOCTL_USBFS_REAPURBNDELAY	_IOW('U', 13, void *)
#define IOCTL_USBFS_CLAIMINTF	_IOR('U', 15, unsigned int)
#define IOCTL_USBFS_RELEASEINTF	_IOR('U', 16, unsigned int)
#define IOCTL_USBFS_RESET		_IO('U', 20)
#define IOCTL_USBFS_CLEAR_HALT	_IOR('U', 21, unsigned int)
#define IOCTL_USBFS_DISCONNECT	_IO('U', 22)
#define IOCTL_USBFS_CONNECT	_IO('U', 23)
#define IOCTL_USBFS_CLAIM_PORT	_IOR('U', 24, unsigned int)
#define IOCTL_USBFS_RELEASE_PORT	_IOR('U', 25, unsigned int)
#define IOCTL_USBFS_GET_CAPABILITIES	_IOR('U', 26, uint32_t)


#define USBFS_CAP_ZERO_PACKET		0x01
#define USBFS_CAP_BULK_CONTINUATION	0x02
#define USBFS_CAP_NO_PACKET_SIZE_LIM	0x04
#define USBFS_CAP_BULK_SCATTER_GATHER	0x08

namespace LFramework::USB {

struct UsbIsoPacketDesc {
	unsigned int length;
	unsigned int actual_length;
	unsigned int status;
};

namespace UsbCaps {
	static constexpr std::uint32_t ZeroPacket = 0x01;
	static constexpr std::uint32_t BulkContinuation = 0x02;
	static constexpr std::uint32_t NoPacketSizeLim = 0x04;
	static constexpr std::uint32_t BulkScatterGather = 0x08;
};

enum class UsbUrbType : unsigned char {
	UsbUrbTypeIso = 0x0,
	UsbUrbTypeInterrupt = 0x1,
	UsbUrbTypeControl = 0x2,
	UsbUrbTypeBulk = 0x3,
};


namespace UsbUrbFlags {
	static constexpr unsigned int ShortNotOk = 0x01;
	static constexpr unsigned int BulkContinuation = 0x04;
	static constexpr unsigned int QueueBulk = 0x10;
	static constexpr unsigned int ZeroPacket = 0x40;
};


struct UsbUrb {
    UsbUrb() = default;
	UsbUrbType type;
	unsigned char endpoint;
	int status;
	unsigned int flags;
	void *buffer;
	int bufferLength;
	int actualLength;
	int startFrame;
	int numberOfPackets;
	int errorCount;
	unsigned int signr;  
	void *userContext;
	struct UsbIsoPacketDesc isoFrameDesc[0];
};


inline uint32_t getUsbDeviceCapabilities(int usbDeviceFileDescriptor) {
    uint32_t result;
    auto errorCode = ioctl(usbDeviceFileDescriptor, IOCTL_USBFS_GET_CAPABILITIES, &result);
    if (errorCode < 0) {
        result = (USBFS_CAP_ZERO_PACKET | USBFS_CAP_BULK_CONTINUATION);
    }
    return result;
}


inline void claimInterface(int usbDeviceFileDescriptor, std::uint8_t interfaceNumber) {
	ioctl(usbDeviceFileDescriptor, IOCTL_USBFS_CLAIMINTF, interfaceNumber);
}

inline void releaseInterface(int usbDeviceFileDescriptor, std::uint8_t interfaceNumber) {
	ioctl(usbDeviceFileDescriptor, IOCTL_USBFS_RELEASEINTF, interfaceNumber);
}


}