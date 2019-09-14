#include "SocketPCH.h"

UDPSocket::~UDPSocket()
{
	closesocket(mSocket);
}

int UDPSocket::Bind(const SocketAddress& inToAddress)
{
	int err = bind(mSocket, &inToAddress.mSockAddr, inToAddress.GetSize());

	if (err == SOCKET_ERROR)
	{
		REPORT;
		return SocketUtil::GetLastError();
	}
	return NO_ERROR;
}

int UDPSocket::SendTo(const void* inData, int inLen, const SocketAddress& inTo)
{
	int byteSentCount = sendto(mSocket, static_cast<const char*>(inData), inLen, 0, &inTo.mSockAddr,
		inTo.GetSize());

	if (byteSentCount <= 0)
	{
		REPORT;
		return -SocketUtil::GetLastError();
	}
	return byteSentCount;
}

int UDPSocket::ReceiveFrom(void* inBuffer, int inLen, SocketAddress& outFrom)
{
	socklen_t fromLength = outFrom.GetSize();

	int readByteCount = recvfrom(mSocket, static_cast<char*>(inBuffer), inLen, 0, &outFrom.mSockAddr,
		&fromLength);

	if (readByteCount >= 0)
	{
		return readByteCount;
	}
	else
	{
		int error = SocketUtil::GetLastError();

		if (error == WSAEWOULDBLOCK)
		{
			return 0;
		}
		else if (error == WSAECONNRESET)
		{
			LOG(L"Connection reset from %s", outFrom.ToString().c_str());
			return -WSAECONNRESET;
		}
		else
		{
			REPORT;
			return -error;
		}
	}
}

int UDPSocket::SetNonBlockingMode(bool inShouldBeNonBlocking)
{
	u_long arg = inShouldBeNonBlocking ? 1 : 0;
	int result = ioctlsocket(mSocket, FIONBIO, &arg);

	if (result != SOCKET_ERROR)
		return NO_ERROR;

	REPORT;
	return SocketUtil::GetLastError();
}
