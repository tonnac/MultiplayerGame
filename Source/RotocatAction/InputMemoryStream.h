#pragma once
#include "MemoryStream.h"

class InputMemoryStream : public MemoryStream
{
public:
	InputMemoryStream(char* inBuffer, uint32_t inByteCount) :
		mBuffer(inBuffer), mCapacity(inByteCount), mHead(0)
	{}

	~InputMemoryStream()
	{ std::free(mBuffer); }

	uint32_t GetRemaningDataSize() const { return mCapacity - mHead; }

	template<typename T>
	void Serialize(T& ioData)
	{
		static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value,
			"Generic Read only supports primitive data types");

		Read(&ioData, sizeof(ioData));
	}

	template<>
	void Serialize(string& ioData)
	{
		Read(ioData);
	}

	void Serialize(const DataType* inDataType, uint8_t* inData)
	{
		for (const MemberVariable& mv : inDataType->GetMemberVariable())
		{
			void* mvData = inData + mv.GetOffset();
			switch (mv.GetPrimitiveType())
			{
			case EPrimitiveType::EPT_Int:
				Serialize(*(int*)mvData);
				break;
			case EPrimitiveType::EPT_String:
				Serialize(*(string*)mvData);
				break;
			case EPrimitiveType::EPT_Float:
				Serialize(*(float*)mvData);
				break;
			default:
				break;
			}
		}
	}

	void Read(void* outData, uint32_t inByteCount);

	template<typename T>
	void Read(vector<T>& outVector)
	{
		size_t elementCount = 0;
		Read(elementCount);
		outVector.resize(elementCount);
		for (T& element : outVector)
		{
			Read(element);
		}
	}

	template <typename X, typename T>
	void Read(unordered_map<X, T>& inMap)
	{
		size_t elementCount = inMap.size();
		Read(elementCount);
		for (size_t i = 0; i < elementCount; ++i)
		{
			X Key; 
			T Value;
			Read(Key);
			Read(Value);
			inMap.insert(std::make_pair(Key, Value));
		}
	}

	template<typename T>
	void Read(T& outData)
	{
		static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value,
			"Generic Read only supports primitive data types");

		Read(&outData, sizeof(outData));
	}

	template <>
	void Read(string& inString)
	{
		size_t elementCount;
		Read(elementCount);
		inString.resize(elementCount);
		Read(const_cast<char*>(inString.data()), static_cast<uint32_t>(elementCount));
	}

private:
	virtual bool IsInput() const override { return true; };

private:
	char* mBuffer;
	uint32_t mHead;
	uint32_t mCapacity;
};