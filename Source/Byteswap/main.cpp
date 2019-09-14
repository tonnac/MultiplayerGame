#include "byteswap.h"

int main()
{
	int64_t e = 0x12345678abcdabcd;
	auto d = ByteSwap(e);

	printf("0x%llx\n0x%llx", e, d);

	TestByteSwap();
	return 0;
}