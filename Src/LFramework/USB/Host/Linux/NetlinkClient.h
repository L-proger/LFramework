#pragma once

namespace LFramework::USB {

class NetlinkClient {
public:
    NetlinkClient();
    ~NetlinkClient();
    int getSocketFileDescriptor() const {
        return _socket;
    }
private:
    int _socket = -1;
};

}