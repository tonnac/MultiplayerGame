#pragma once


class InputMemoryBitStream
{
public:
	InputMemoryBitStream(char* inBuffer, uint32_t inBitCount) :
		mBuffer(inBuffer), mBitCapacity(inBitCount), mBitHead(0), mIsBufferOwner(false) {}

	InputMemoryBitStream(const InputMemoryBitStream& inOther) :
		mBitCapacity(inOther.mBitCapacity),
		mBitHead(inOther.mBitHead), mIsBufferOwner(inOther.mIsBufferOwner) 
	{
		int byteCount = mBitCapacity >> 3;
		void* temp = std::malloc(byteCount);
		if (nullptr != temp)
		{
			mBuffer = static_cast<char*>(temp);
			memcpy(mBuffer, inOther.mBuffer, byteCount);
		}
		else
		{
			throw std::bad_alloc();
		}
	}

	~InputMemoryBitStream() { if (mIsBufferOwner) { std::free(mBuffer); } }

	const char* GetBufferPtr() const { return mBuffer; }
	uint32_t GetRemainingBitCount() const { return mBitCapacity - mBitHead; }
	
	void ReadBits(void* outData, uint32_t inBitCount);
	void ReadBits(uint8_t& outData, uint32_t inBitCount);

	void ReadBytes(void* outData, uint32_t inByteCount) { ReadBits(outData, inByteCount << 3); }

	template<typename T>
	void Read(T& inData, uint32_t inBitCount = sizeof(T) * 8)
	{
		static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value,
			"Generic Read only supports primitive data types");
		ReadBits(&inData, inBitCount);

#if (STREAM_ENDIANNESS != PLATFORM_ENDIANNESS)
		if ((inBitCount >> 3) > 1)
		{
			inData = ByteSwap(inData);
		}
#endif
	}
	template<>
	void Read<bool>(bool& inData, uint32_t inBitCount)
	{
		ReadBits(&inData, 1);
	}
	template<>
	void Read<string>(string& inData, uint32_t inBitCount)
	{
		uint32_t elementCount;
		Read(elementCount);
		inData.resize(elementCount);
		for (char& element : inData)
		{
			Read(element);
		}
	}


	void Read(DirectX::XMFLOAT4& inQuat);
	void Read(DirectX::XMFLOAT3& InVector);

	void Read(DirectX::XMFLOAT3& inScale, DirectX::XMFLOAT4& inQuat, DirectX::XMFLOAT3& inTrans);

	void ResetToCapacity(uint32_t inByteCapacity) { mBitCapacity = inByteCapacity << 3; mBitHead = 0; }

private:
	char* mBuffer;
	uint32_t mBitHead;
	uint32_t mBitCapacity;
	bool mIsBufferOwner;
};