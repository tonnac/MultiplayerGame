#pragma once

class TestClass
{
public:
	TestClass() : mHealth(10), mMeowCount(3)
	{}

	TestClass(int32_t health, int32_t count, const char* name)
		: mName(name), mHealth(health), mMeowCount(count)
	{}

	void Write(OutputMemoryStream& inStream) const;
	void Read(InputMemoryStream& inStream);

	static const DataType* GetDataType()
	{
		static DataType* sDatatype = nullptr;
		if (sDatatype == nullptr)
		{
			InitDataType(&sDatatype);
		}
		return sDatatype;
	}
private:
	static void InitDataType(DataType** inDataType)
	{
		*inDataType = new DataType
		(
			{
				MemberVariable("mHealth", EPrimitiveType::EPT_Int, OffsetOf(TestClass, mHealth)),
				MemberVariable("mMeowCount", EPrimitiveType::EPT_Int, OffsetOf(TestClass, mMeowCount)),
				MemberVariable("mName", EPrimitiveType::EPT_String, OffsetOf(TestClass, mName))
			},
			nullptr
		);
	}

private:
	int32_t mHealth;
	int32_t mMeowCount;
	string mName;
};

void SendTestClass(TCPSocketPtr inSocket, const TestClass* inTest);
void ReceiveTestClass(TCPSocketPtr inSocket, TestClass* outTest);