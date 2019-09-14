#pragma once
class InputMemoryStream
{
public:
	InputMemoryStream(char* inBuffer, uint32_t inByteCount) :
		mBuffer(inBuffer), mCapacity(inByteCount), mHead(0)
	{}

	~InputMemoryStream()
	{ std::free(mBuffer); }

	uint32_t GetRemaningDataSize() const { return mCapacity - mHead; }

	void Read(void* outData, uint32_t inByteCount);

	template<typename T>
	void Read(T& outData)
	{
		static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value,
			"Generic Read only supports primitive data types");

		Read(&outData, sizeof(outData));
	}

private:
	char* mBuffer;
	uint32_t mHead;
	uint32_t mCapacity;
};