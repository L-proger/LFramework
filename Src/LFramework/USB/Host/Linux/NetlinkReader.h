#pragma once

#include "NetlinkClient.h"
#include <memory>
#include <sys/epoll.h>
#include <stdexcept>
#include <fcntl.h>
#include <unistd.h>
#include <thread>
#include <sys/socket.h>
#include <linux/netlink.h>
#include <optional>
#include <cstring>
#include <functional>

namespace LFramework::USB {

#define NL_GROUP_KERNEL 1

class NetlinkReader {
public:
    struct DeviceEvent {
        bool removed;
        std::uint8_t busNumber;
        std::uint8_t deviceNumber;
        std::string sysfsFileName;
    };

    using EventCallback = std::function<void(const DeviceEvent&)>;

    NetlinkReader(std::shared_ptr<NetlinkClient> client, EventCallback callback) : _client(client), _callback(callback){
        _epoll = epoll_create(1);

        epoll_event epollEvent = {};
        epollEvent.events = EPOLLIN | EPOLLERR;
        epollEvent.data.fd = client->getSocketFileDescriptor();

        if(epoll_ctl(_epoll, EPOLL_CTL_ADD, epollEvent.data.fd, &epollEvent) < 0){
            close(_epoll);
            throw std::runtime_error("epoll_ctl failed on Netlink socket");
        }

        _isRunning = true;
        _pollingThread = std::thread([this](){
            readNetlinkThreadHandler();
        });
    }

    ~NetlinkReader(){
        _isRunning = false;
        epoll_ctl(_epoll, EPOLL_CTL_DEL, _client->getSocketFileDescriptor(), nullptr);
        close(_epoll);
        if(_pollingThread.joinable()){
            _pollingThread.join();
        }
    }

private:
    std::shared_ptr<NetlinkClient> _client;
    EventCallback _callback;
    int _epoll = -1;
    std::thread _pollingThread;
    bool _isRunning = false;


    void readNetlinkThreadHandler() {
        epoll_event event{};
        while(_isRunning) {
            auto epollFileDescriptors = epoll_wait(_epoll, &event, 1, 100);
            if(epollFileDescriptors <= 0){
                continue;
            }
            if (static_cast<bool>(event.events & EPOLLIN)){
                std::vector<std::string> message;
                if(readNetlinkMessage(_client->getSocketFileDescriptor(), message)){

                    auto subsystem = getNetlinkVariableValue("SUBSYSTEM", message);
                    if(!subsystem.has_value() || subsystem.value() != "usb"){
                        continue; //Not USB subsystem message
                    }

                    auto devtype = getNetlinkVariableValue("DEVTYPE", message);
                    if(!devtype.has_value() || (devtype != "usb_device")){
                        continue; //Not USB device ?
                    }

                    auto action = getNetlinkVariableValue("ACTION", message);
                    bool removed = false;
                    if(!action.has_value()){
                        continue;
                    }else if(action.value() == "remove"){
                        removed = true;
                    }else if(action.value() != "add"){
                        continue; //Not interesting message
                    }

                    DeviceEvent event;
                    event.removed = removed;


                    auto busnum = getNetlinkVariableValue("BUSNUM", message);
                    if(busnum.has_value()){
                        event.busNumber = std::atoi(busnum.value().c_str());
                        auto devnum = getNetlinkVariableValue("DEVNUM", message);
                        if(!devnum.has_value()){
                            //Well, this is not expected. Maybe report error here!?
                            continue;
                        }
                        event.deviceNumber = std::atoi(devnum.value().c_str());
                    }else{
                        //Some old format device path?

                        auto devicePath = getNetlinkVariableValue("DEVICE", message);
                        if(!devicePath.has_value()){
                            continue;
                        }

                        auto slashPos = devicePath.value().find_last_of('/');
                        auto devNumStr = devicePath.value().substr(slashPos + 1);
                        auto busNumStr = devicePath.value().substr(slashPos - 3);

                        event.busNumber = std::atoi(busNumStr.c_str());
                        event.deviceNumber = std::atoi(devNumStr.c_str());

                    }
                    
                    auto devicePath = getNetlinkVariableValue("DEVPATH", message);
                    if(devicePath.has_value()){
                        auto slashPos = devicePath.value().find_last_of('/');
                        event.sysfsFileName = devicePath.value().substr(slashPos + 1);
                    }else{
                        continue;
                    }
                   
                    if(_callback != nullptr){
                        _callback(event);
                    }
                }
            }
        }
    }

    std::optional<std::string> getNetlinkVariableValue(const char* key, const std::vector<std::string>& data){
        auto keyLength = strlen(key);
        for(const auto& s : data){
            if((strncmp(key, s.c_str(), keyLength) == 0) && (s[keyLength] == '=')){
                return s.substr(keyLength + 1);
            }
        }
        return std::nullopt;
    }

    static std::vector<std::string> parseNetlinkMessageBuffer(const char* buffer, int bufferLength){
        std::vector<std::string> result;
        const char *end = buffer + bufferLength;

        while (buffer < end && *buffer) {
            result.push_back(buffer);
		    buffer += strlen(buffer) + 1;
        }

        return result;
    }

    bool readNetlinkMessage(int netlinkFd, std::vector<std::string>& messageData){
        auto result = true;
        char buffer[CMSG_SPACE(sizeof(struct ucred))];
        constexpr size_t messageBufferLength = 2048;
        char messageBuffer[messageBufferLength];
        sockaddr_nl socketAddress{};
        iovec iov {};
        iov.iov_base = messageBuffer;
        iov.iov_len = sizeof(messageBuffer);
        msghdr msg{};
        msg.msg_iov = &iov;
        msg.msg_iovlen = 1;
        msg.msg_control = buffer;
        msg.msg_controllen = sizeof(buffer);
        msg.msg_name = &socketAddress;
        msg.msg_namelen = sizeof(socketAddress);
        ssize_t length = recvmsg(netlinkFd, &msg, 0);
        if (length == -1 && errno != EAGAIN && errno != EINTR)
            result = false;
        if ((msg.msg_flags & MSG_TRUNC) != 0)
            result = false;
        if (socketAddress.nl_groups != NL_GROUP_KERNEL || socketAddress.nl_pid != 0)
            result = false;
        auto cmsg = CMSG_FIRSTHDR(&msg);
        if (!cmsg || cmsg->cmsg_type != SCM_CREDENTIALS)
            result = false;
        auto cred = (ucred *)CMSG_DATA(cmsg);
        if (cred->uid != 0)
            result = false;
        if(result)
            messageData = parseNetlinkMessageBuffer(messageBuffer, length);
        return result;
    }
};

}