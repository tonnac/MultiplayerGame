#pragma once

#define OffsetOf(c, mv) ((size_t) & static_cast<c*>(nullptr)->mv)

enum class EPrimitiveType : uint8_t
{
	EPT_Int,
	EPT_String,
	EPT_Float
};

class MemberVariable
{
public:
	MemberVariable(const char* inName, EPrimitiveType inPrimitiveType, uint32_t inOffset) :
		mName(inName), mPrimitiveType(inPrimitiveType), mOffset(inOffset)
	{}

	EPrimitiveType GetPrimitiveType() const { return mPrimitiveType; }
	uint32_t GetOffset() const { return mOffset; }

private:
	string mName;
	EPrimitiveType mPrimitiveType;
	uint32_t mOffset;
};

class DataType
{
public:
	DataType(std::initializer_list<MemberVariable> inMVs) :
		mMemberVariable(inMVs)
	{}

	const vector<MemberVariable>& GetMemberVariable() const
	{ return mMemberVariable; }
	
private:
	vector<MemberVariable> mMemberVariable;
};