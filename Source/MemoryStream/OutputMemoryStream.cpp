#include "MemoryPCH.h"

void OutputMemoryStream::Write(const void* inData, size_t inByteCount)
{
	uint32_t resultHead = mHead + static_cast<uint32_t>(inByteCount);

	if (resultHead > mCapacity)
	{
		ReallocBuffer(std::max(mCapacity * 2, resultHead));
	}

	std::memcpy(mBuffer + mHead, inData, inByteCount);

	mHead = resultHead;
}

void OutputMemoryStream::ReallocBuffer(uint32_t inNewLength)
{
	if (inNewLength == 0)
	{
		std::free(mBuffer);
		mBuffer = nullptr;
	}
	else
	{
		if (void* temp = std::realloc(mBuffer, inNewLength))
		{
			mBuffer = static_cast<char*>(temp);
		}
		else
		{
			throw std::bad_alloc();
		}
	}
	mCapacity = inNewLength;
}
