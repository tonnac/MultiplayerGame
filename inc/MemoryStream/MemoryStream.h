#pragma once
#include <cstdint>

class MemoryStream
{
public:
	//template<typename T> 
	//void Serialize(T& ioData)
	//{
	//	static_assert(std::is_arithmetic<T>::value || std::is_enum<T>::value,
	//		"Generic Read only supports primitive data types");

	//	Serialize(&ioData, sizeof(ioData));
	//}

private:
	//virtual void Serialize(void* ioData, uint32_t inByteCount) = 0;
	virtual bool IsInput() const = 0;
};