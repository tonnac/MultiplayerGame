#include <iostream>
using namespace std;

class A
{
public:
	A()
	{
		cout << "Const" << endl;
	}

	A(const A& rhs)
	{
		cout << "Copy" << endl;
	}

	A(A&& rhs) noexcept
	{
		cout << "rh" << endl;
	}
	A operator =(A&& rhs) = delete;
};

void Foo(const A& a)
{

}

int main()
{
	A a;
	Foo(a);
	
//	A b(a);

	return 0;
}