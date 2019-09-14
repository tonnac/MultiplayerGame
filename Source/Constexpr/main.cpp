#include <iostream>
#include <cmath>

template <int x>
struct mlog2 { enum { value = 1 + mlog2<x / 2>::value }; };

template<>
struct mlog2<1> { enum { value = 1 }; };

//constexpr size_t mlog2(size_t n)
//{
//	return (n < 2) ? 1 : 1 + mlog2(n / 2);
//}

int main()
{
//	constexpr size_t ee = mlog2(8);

	constexpr int qwe = mlog2<8>::value;

	return 0;
}