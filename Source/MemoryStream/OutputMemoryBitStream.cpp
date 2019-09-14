#include "MemoryPCH.h"

void OutputMemoryBitStream::WriteBits(uint8_t inData, uint32_t inBitCount)
{
	uint32_t nextBitHead = mBitHead + static_cast<uint32_t>(inBitCount);

	if (nextBitHead > mBitCapacity)
	{
		ReallocBuffer(std::max(mBitCapacity * 2, nextBitHead));
	}

	uint32_t byteOffset = mBitHead >> 3;
	uint32_t bitOffset = mBitHead & 0x7;

	uint8_t currentMask = ~(0xff << bitOffset);
	mBuffer[byteOffset] = (mBuffer[byteOffset] & currentMask) | (inData << bitOffset);

	uint32_t bitsFreeThisByte = 8 - bitOffset;

	if (bitsFreeThisByte < inBitCount)
	{
		mBuffer[byteOffset + 1] = inData >> bitsFreeThisByte;
	}

	mBitHead = nextBitHead;
}

void OutputMemoryBitStream::WriteBits(const void* inData, uint32_t inBitCount)
{
	const char* srcByte = static_cast<const char*>(inData);

	while (inBitCount > 8)
	{
		WriteBits(*srcByte, 8);
		++srcByte;
		inBitCount -= 8;
	}

	if (inBitCount > 0)
	{
		WriteBits(*srcByte, inBitCount);
	}
}

void OutputMemoryBitStream::Write(const DirectX::XMFLOAT3& inVector)
{
	Write(inVector.x);
	Write(inVector.y);
	Write(inVector.z);
}

void OutputMemoryBitStream::Write(const DirectX::XMFLOAT4& inQuat)
{
	float precision = (2.f / gPrecision);
	Write(ConvertToFixed(inQuat.x, -1.0f, precision), gPrecisionBit);
	Write(ConvertToFixed(inQuat.x, -1.0f, precision), gPrecisionBit);
	Write(ConvertToFixed(inQuat.x, -1.0f, precision), gPrecisionBit);
	Write(inQuat.w < 0);
}

void OutputMemoryBitStream::ReallocBuffer(uint32_t inNewBitCapacity)
{
	if (mBuffer == nullptr)
	{
		void* temp = std::calloc(inNewBitCapacity >> 3, sizeof(char));
		if (nullptr != temp)
		{
			mBuffer = static_cast<char*>(temp);
		}
		else
		{
			throw std::bad_alloc();
		}
	}
	else
	{
		void* temp = std::calloc(inNewBitCapacity >> 3, sizeof(char));
		if (nullptr != temp)
		{
			memcpy(temp, mBuffer, mBitCapacity >> 3);
			std::free(mBuffer);
			mBuffer = static_cast<char*>(temp);
		}
		else
		{
			throw std::bad_alloc();
		}
	}
	mBitCapacity = inNewBitCapacity;
}
