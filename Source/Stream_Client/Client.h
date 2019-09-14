#pragma once
#include "MemoryPCH.h"
#include <io.h>
#include <fcntl.h>

class Client;
using ClientPtr = shared_ptr<Client>;

class Client
{
public:
	static ClientPtr getInstance();

	void Initialize(const tString& IPAddr, uint16_t portNum, SocketAddressFamily socketType);
	void Run();

	~Client();

private:
	static ClientPtr mClient;
	Client();

	WSADATA mWsadata;
	SocketAddressPtr mAddress;
	TCPSocketPtr mTCPSocket;
	UDPSocketPtr mUDPSocket;
};

#define CLIENT Client::getInstance()