#include <cstdio>
#include <cstdint>

class A
{
public:
	static __int32* print()
	{
		static __int32* pl;
		if (pl == nullptr)
		{
			pl = new __int32;
			*pl = 392e+05;
		}
		printf("%d\n", *pl);
		return pl;
	}
};

class B : public A
{
public:
	static __int32* print()
	{
		static __int32* zxc;
		if (zxc == nullptr)
		{
			zxc = new __int32;
			*zxc = 47e+01;
		}
		printf("%d\n", *zxc);
		return zxc;
	}
};

class C
{
public:
	C() = default;
	template<typename T> void foo(T param) = delete;
	void foo(double pt)
	{

	}

private:
	int* ptr;
};


int main()
{
	A a;
	B b;
	C ad;

	auto bc = a.print();
	auto dc = b.print();


	return 0;
}