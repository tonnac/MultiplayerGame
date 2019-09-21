#pragma once
#include <functional>

#define OffsetOf(c, mv) ((size_t) & static_cast<c*>(nullptr)->mv)

enum class EPrimitiveType : uint8_t
{
	EPT_Int,
	EPT_String,
	EPT_Float
};

class GameObject;
using GameObjectCreationFunc = std::function<GameObject* ()>;

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
	DataType(std::initializer_list<MemberVariable> inMVs, GameObjectCreationFunc inFunc) :
		mMemberVariable(inMVs), mCreateFunc(inFunc)
	{}

	const vector<MemberVariable>& GetMemberVariable() const
	{ return mMemberVariable; }
	
	const GameObjectCreationFunc GetCreationFunc() const
	{ return mCreateFunc; }

private:
	vector<MemberVariable> mMemberVariable;
	GameObjectCreationFunc mCreateFunc;
};