#pragma once

class TCPSocket;

using TCPSocketPtr = shared_ptr<TCPSocket>;

class TCPSocket
{
public:
	~TCPSocket();
	int Connect(const SocketAddress& inAddress);
	int Bind(const SocketAddress& inToAddress);
	int Listen(int inBackLog = SOMAXCONN);
	TCPSocketPtr Accept(SocketAddress& inFromAddress);
	int32_t Send(const void* inData, size_t inLen);
	int32_t Receive(void* inBuffer, size_t inLen);
private:
	friend class SocketUtil;
	TCPSocket(SOCKET inSocket) : mSocket(inSocket) {}
	SOCKET mSocket;
};

