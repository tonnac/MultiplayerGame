#pragma once
#include "MemoryStream.h"

class OutputMemoryStream : public MemoryStream
{
public:
	OutputMemoryStream() :
		mBuffer(nullptr), mHead(0), mCapacity(0)
	{ ReallocBuffer(32); }

	~OutputMemoryStream()
	{ std::free(mBuffer); }

	const char* GetBufferPtr() const { return mBuffer; }
	uint32_t GetLength() const { return mHead; }

	template<typename T>
	void Serialize(T& ioData)
	{
		static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value,
			"Generic Read only supports primitive data types");

		Write(&ioData, sizeof(ioData));
	}

	template<>
	void Serialize(string& ioData)
	{
		Write(ioData);
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

	void Write(const void* inData, size_t inByteCount);

	template <typename T> 
	void Write(const vector<T>& inVector)
	{
		size_t elementCount = inVector.size();
		Write(elementCount);
		for (const T& element : inVector)
		{
			Write(element);
		}
	}

	template <typename X, typename T>
	void Write(const unordered_map<X, T>& inMap)
	{
		size_t elementCount = inMap.size();
		Write(elementCount);
		for (const std::pair<X, T>& element : inMap)
		{
			Write(element.first);
			Write(element.second);
		}
	}

	template <typename T>
	void Write(const T& inData)
	{
		static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value,
			"Generic Write only supports primitive data types");

		Write(&inData, sizeof(inData));
	}

	template <> 
	void Write(const string& inString)
	{
		size_t elementCount = inString.length();
		Write(elementCount);
		Write(inString.data(), static_cast<uint32_t>(elementCount));
	}

private:
	virtual bool IsInput() const override { return false; };

private:
	void ReallocBuffer(uint32_t inNewLength);

	char* mBuffer;
	uint32_t mHead;
	uint32_t mCapacity;
};