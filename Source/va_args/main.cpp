#include <cstdio>
#include <cstdarg>

void PrintNumbers(int args, ...)
{
	va_list ap;

	va_start(ap, args);

	for (int i = 0; i < args; ++i)
	{
		int num = va_arg(ap, int);

		printf("%d ", num);
	}
	va_end(ap);

	printf("\n");
}

int main()
{
	PrintNumbers(1, 10);
	PrintNumbers(2, 10, 20);
	PrintNumbers(3, 10, 20, 30);
	PrintNumbers(4, 10, 20, 30, 40);

	return 0;
}