#include "MemoryPCH.h"

void InputMemoryStream::Read(void* outData, uint32_t inByteCount)
{
	uint32_t resultHead = mHead + inByteCount;
	if (resultHead > mCapacity)
	{
		//handle error, no data to read!
	}

	std::memcpy(outData, mBuffer + mHead, inByteCount);

	mHead = resultHead;
}
