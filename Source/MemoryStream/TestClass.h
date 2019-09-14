#pragma once
#include "SocketPCH.h"
class TestClass
{
public:
	TestClass() : mHealth(10), mMeowCount(3)
	{}

	void Write();
	void Read();

private:
	int32_t mHealth;
	int32_t mMeowCount;
	char mName[128];
};