#include <vector>
#include <iostream>

using namespace std;

class A
{
public:
	A()
	{
		cout << "Construct" << endl;
	}

	void Print()
	{
		static int i = 0;
		cout << i++ << endl;
	}

private:
	int a = 123;
};

int main()
{
	vector<A> qwe;

	qwe.reserve(165);

	for (int i = 0; i < 165; ++i)
	{
		qwe.push_back(A());
	}

	for (int i = 0; i < 165; ++i)
	{
		qwe[i].Print();
	}

	return 0;
}