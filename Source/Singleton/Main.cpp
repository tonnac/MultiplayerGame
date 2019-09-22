#include <memory>

#define SINGLETON(x)						\
public:										\
	static std::unique_ptr<x> sInstance;	\
	static void StaticInit()				\
	{										\
		sInstance.reset(new x());			\
	}


class A
{
	SINGLETON(A);

public:
	void foo()
	{
		printf("Hello World");
	}

};

std::unique_ptr<A> A::sInstance = nullptr;


int main()
{
	A::StaticInit();
	A::sInstance->foo();

	return 0;
}