#include "SocketPCH.h"

TCPSocket::~TCPSocket()
{
	closesocket(mSocket);
}

int TCPSocket::Connect(const SocketAddress& inAddress)
{
	int err = connect(mSocket, &inAddress.mSockAddr, inAddress.GetSize());
	if (err < 0)
	{
		REPORT;
		return -SocketUtil::GetLastError();
	}
	return NO_ERROR;
}

int TCPSocket::Bind(const SocketAddress& inToAddress)
{
	int error = bind(mSocket, &inToAddress.mSockAddr, inToAddress.GetSize());
	if (error != 0)
	{
		REPORT;
		return SocketUtil::GetLastError();
	}

	return NO_ERROR;
}

int TCPSocket::Listen(int inBackLog)
{
	int err = listen(mSocket, inBackLog);
	if (err < 0)
	{
		REPORT;
		return -SocketUtil::GetLastError();
	}
	return NO_ERROR;
}

TCPSocketPtr TCPSocket::Accept(SocketAddress& inFromAddress)
{
	socklen_t length = inFromAddress.GetSize();
	SOCKET newSocket = accept(mSocket, &inFromAddress.mSockAddr, &length);

	if (newSocket != INVALID_SOCKET)
	{
		return TCPSocketPtr(new TCPSocket(newSocket));
	}
	else
	{
		REPORT;
		return nullptr;
	}
}

int32_t TCPSocket::Send(const void* inData, size_t inLen)
{
	int bytesSentCount = send(mSocket, static_cast<const char*>(inData), (int32_t)inLen, 0);
	if (bytesSentCount < 0)
	{
		REPORT;
		return -SocketUtil::GetLastError();
	}
	return bytesSentCount;
}

int32_t TCPSocket::Receive(void* inBuffer, size_t inLen)
{
	int bytesReceivedCount = recv(mSocket, static_cast<char*>(inBuffer), (int32_t)inLen, 0);
	if (bytesReceivedCount < 0)
	{
		SocketUtil::ReportError("d");
		REPORT;
		return -SocketUtil::GetLastError();
	}
	return bytesReceivedCount;
}
