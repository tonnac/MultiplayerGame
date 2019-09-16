#include "MemoryPCH.h"

void TestClass::Write(OutputMemoryStream& inStream) const
{
	inStream.Write(mHealth);
	inStream.Write(mMeowCount);
	inStream.Write(mName);
}

void TestClass::Read(InputMemoryStream& inStream)
{
	inStream.Read(mHealth);
	inStream.Read(mMeowCount);
	inStream.Read(mName);
}

void SendTestClass(TCPSocketPtr inSocket, const TestClass* inTest)
{
	OutputMemoryStream stream;
	inTest->Write(stream);
	send(inSocket->getSocket(), stream.GetBufferPtr(), stream.GetLength(), 0);
}

void ReceiveTestClass(TCPSocketPtr inSocket, TestClass* outTest)
{
	void* temp = std::malloc(kMaxPacketSize);
	if (nullptr != temp)
	{
		char* temporaryBuffer = static_cast<char*>(temp);

		int receivedByteCount = recv(inSocket->getSocket(), temporaryBuffer, kMaxPacketSize, 0);

		if (receivedByteCount > 0)
		{
			InputMemoryStream stream(temporaryBuffer, static_cast<uint32_t>(receivedByteCount));
			outTest->Read(stream);
		}
		else
		{
			std::free(temporaryBuffer);
		}
	}
	else
	{
		throw std::bad_alloc();
	}
}
