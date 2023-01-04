#include "NetlinkClient.h"
#include "../UsbException.h"
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <linux/netlink.h>

#define NL_GROUP_KERNEL 1

namespace LFramework::USB {

NetlinkClient::NetlinkClient() {
    sockaddr_nl socketAddressClient = {};
    socketAddressClient.nl_family = AF_NETLINK;
    socketAddressClient.nl_groups = NL_GROUP_KERNEL;
    auto socketType = SOCK_RAW | SOCK_NONBLOCK | SOCK_CLOEXEC;
    _socket = socket(PF_NETLINK, socketType, NETLINK_KOBJECT_UEVENT);
    if ((_socket < 0) && errno == EINVAL) {
        socketType = SOCK_RAW;
        _socket = socket(PF_NETLINK, socketType, NETLINK_KOBJECT_UEVENT);
    }

    if (_socket < 0){
        throw UsbException(std::string("Failed to create Netlink socket: ") + strerror(errno));
    }

    //set non blocking mode
    int flags;
    if (!static_cast<bool>(socketType & SOCK_NONBLOCK)) {
        flags = fcntl(_socket, F_GETFL);
        fcntl(_socket, F_SETFL, flags | O_NONBLOCK);
    }
    
    if(bind(_socket, reinterpret_cast<sockaddr*>(&socketAddressClient), sizeof(socketAddressClient)) < 0) {
        close(_socket);
        throw UsbException(std::string("Failed to bing Netlink socket: ") + strerror(errno));
    }

    int optValue = 1;
    if (setsockopt(_socket, SOL_SOCKET, SO_PASSCRED, &optValue, sizeof(optValue)) < 0) {
        close(_socket);
        throw UsbException(std::string("Failed to set Netlink socket option: ") + strerror(errno));
    }
}

NetlinkClient::~NetlinkClient(){
    if(_socket >= 0){
        close(_socket);
    }
}

}