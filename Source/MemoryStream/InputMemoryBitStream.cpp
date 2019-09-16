#include "MemoryPCH.h"

void InputMemoryBitStream::ReadBits(uint8_t& outData, uint32_t inBitCount)
{
	uint32_t byteOffset = mBitHead >> 3;
	uint32_t bitOffset = mBitHead & 0x7;

	outData = static_cast<uint8_t>(mBuffer[byteOffset]) >> bitOffset;

	uint32_t bitsFreeThisByte = 8 - bitOffset;
	if (bitsFreeThisByte < inBitCount)
	{
		outData |= static_cast<uint8_t>(mBuffer[byteOffset + 1]) << bitsFreeThisByte;
	}

	outData &= (~(0x00ff << inBitCount));

	mBitHead += inBitCount;
}

void InputMemoryBitStream::ReadBits(void* outData, uint32_t inBitCount)
{
	uint8_t* destByte = reinterpret_cast<uint8_t*>(outData);

	while (inBitCount > 8)
	{
		ReadBits(*destByte, 8);
		++destByte;
		inBitCount -= 8;
	}

	if (inBitCount > 0)
	{
		ReadBits(*destByte, inBitCount);
	}

}

void InputMemoryBitStream::Read(DirectX::XMFLOAT4& inQuat)
{
	uint16_t f = 0;
	Read(f, gPrecisionBit);
	inQuat.x = ConvertFromFixed(f, -1.f, gPrecision);
	Read(f, gPrecisionBit);
	inQuat.y = ConvertFromFixed(f, -1.f, gPrecision);
	Read(f, gPrecisionBit);
	inQuat.z = ConvertFromFixed(f, -1.f, gPrecision);

	inQuat.w = sqrtf(1.f - ((inQuat.x * inQuat.x) + (inQuat.y * inQuat.y) + (inQuat.z * inQuat.z)));

	bool isNegative;
	Read(isNegative);

	if (isNegative)
	{
		inQuat.w *= -1;
	}
}

void InputMemoryBitStream::Read(DirectX::XMFLOAT3& InVector)
{
	Read(InVector.x);
	Read(InVector.y);
	Read(InVector.z);
}

void InputMemoryBitStream::Read(DirectX::XMFLOAT3& inScale, DirectX::XMFLOAT4& inQuat, DirectX::XMFLOAT3& inTrans)
{
	bool isOne;
	Read(isOne);
	if (isOne)
	{
		inScale.x = inScale.y = inScale.z = 1.f;
	}
	else
	{
		constexpr float Precision = 5.f / gPrecisionNum;
		bool isUniform;
		uint16_t f = 0;
		Read(isUniform);
		if (isUniform)
		{
			Read(f);
			inScale.x = inScale.y = inScale.z = ConvertFromFixed(f, -1.f, Precision);
		}
		else
		{
			Read(f);
			inScale.x = ConvertFromFixed(f, -1.f, Precision);
			Read(f);
			inScale.y = ConvertFromFixed(f, -1.f, Precision);
			Read(f);
			inScale.z = ConvertFromFixed(f, -1.f, Precision);
		}
	}
	Read(inQuat);
	Read(inTrans);
}
