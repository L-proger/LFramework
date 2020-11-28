#pragma once

#include <LFramework/Containers/StaticVector.h>
#include <LFramework/DeviceNetwork/NetworkInterface.h>
#include "Packet.h"
#include <cstddef>

/*
namespace LFramework::DeviceNetwork {

template<std::size_t NetworkInterfacesCount>
class Router {
public:
	struct Record {
        NetworkAddress interfaceAddress = NetworkAddress::Broadcast();
        NetworkAddress routerAddress = NetworkAddress::Broadcast();
		NetworkInterface* networkInterface;
	};

	bool addNetworkInterface(NetworkInterface* networkInterface){
		return _networkInterfaces.push_back(networkInterface);
	}

	void process() {
		while(true){

		}
	}
private:
	StaticVector<NetworkInterface*, NetworkInterfacesCount> _networkInterfaces;
    StaticVector<Record, 256> _routingTable;
};

}
*/
