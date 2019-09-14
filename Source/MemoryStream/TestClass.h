#pragma once


class TestClass
{
public:
	TestClass() : mHealth(10), mMeowCount(3)
	{}

	TestClass(int32_t health, int32_t count, const char* name)
		: mHealth(health), mMeowCount(count)
	{
		memcpy(mName, name, _countof(mName));
	}

	void Write(OutputMemoryStream& inStream) const;
	void Read(InputMemoryStream& inStream);

private:
	int32_t mHealth;
	int32_t mMeowCount;
	char mName[128] = {};
};

void SendTestClass(TCPSocketPtr inSocket, const TestClass* inTest);
void ReceiveTestClass(TCPSocketPtr inSocket, TestClass* outTest);