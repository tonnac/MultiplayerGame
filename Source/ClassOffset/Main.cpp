#include <string>
#include <cstdint>
#include <vector>

using namespace std;

#define OffsetOf(c, mv) ((size_t) & (static_cast<c*>(nullptr)->mv))

enum class EPrimitiveType : uint8_t
{
	EPT_Int,
	EPT_String,
	EPT_float
};

class MemberVariable
{
public:
	MemberVariable(const char* name, EPrimitiveType primitive, uint32_t offset)
		: mName(name), mPrimitiveType(primitive), mOffset(offset)
	{}

private:
	string mName;
	EPrimitiveType mPrimitiveType;
	uint32_t mOffset;
};

class DataType
{
public:
	DataType(initializer_list<MemberVariable> inMvs)
		: mMembervariables(inMvs)
	{}

private:
	vector<MemberVariable> mMembervariables;
};


class A
{
public:
	A(const char* name, int legcount, int headcount, float health)
		: mLegCount(legcount), mHeadCount(headcount), mHealth(health)
	{
		int Length = strlen(name);
		memcpy(mName, name, Length);
	}		 

	static DataType* GetDataType()
	{
		static DataType* sDataType = nullptr;
		if (sDataType == nullptr)
		{
			InitDataType(&sDataType);
		}
		return sDataType;
	}

private:
	static void InitDataType(DataType** inDataType)
	{
		*inDataType = new DataType
		(
			{
				MemberVariable("mName", EPrimitiveType::EPT_String, OffsetOf(A, mName)),
				MemberVariable("mLegCount", EPrimitiveType::EPT_Int, OffsetOf(A, mLegCount)),
				MemberVariable("mHeadCount", EPrimitiveType::EPT_Int, OffsetOf(A, mHeadCount)),
				MemberVariable("mHealth", EPrimitiveType::EPT_float, OffsetOf(A, mHealth))
			}
		);		
	}

private:
	char mName[256];
	int mLegCount, mHeadCount;
	float mHealth;
};



int main()
{
	A a("ABC", 10, 5, 923.523f);

	auto b = a.GetDataType();
	auto c = a.GetDataType();

	return 0;
}