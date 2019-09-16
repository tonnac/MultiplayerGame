#include "byteswap.h"

int main()
{
	float epsilon = 1.192092896e-07F;
	float qp = 1.e-3F;

	uint16_t e = 42054;
	int d = ByteSwap(e);

	printf("%g, %f", epsilon, qp);


	TestByteSwap();
	return 0;
}